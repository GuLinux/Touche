#include "cwiidconnectionworker.h"
#include <QDebug>
#include <iostream>
#include "wiimote_messages.h"
#include <time.h>
#include "timeutils.h"
#include <QMap>
#include <QThread>
#include <unistd.h>
#include <math.h>
#include <QMutex>
#include <QMutexLocker>


QMap<cwiid_wiimote_t *, CWiidConnectionWorker*> workers;
QMap<cwiid_wiimote_t *, timespec> lastMessageTime;

static const double lowSpeedDivider(8192.0/595.0);
//static const double highSpeedMultiplier(2000.0/440.0);
static const double highSpeedMultiplier(2000.0/440.0);

CWiidConnectionWorker::CWiidConnectionWorker(QObject *parent) :
    QObject(parent)
{
    mutex = new QMutex();
}

CWiidConnectionWorker::~CWiidConnectionWorker()
{
    delete mutex;
}

void CWiidConnectionWorker::emitMessage(WiimoteMessage message)
{
    WiimoteVector3 accelData = message.accel();
    WiimoteVector3 calibrated = (accelData - accelCalibZero) / (accelCalibOne - accelCalibZero);
    message.setAccel(calibrated);

    double yaw = sqrt(pow(calibrated.x(),2)+pow(calibrated.y(),2)+pow(calibrated.z(),2));
    double roll = atan(calibrated.x()/calibrated.z());
    if (calibrated.z() <= 0.0) {
        roll += M_PI * ((calibrated.x() > 0.0) ? 1 : -1);
    }
    roll *= -1;

    double pitch = atan(calibrated.y()/calibrated.z()*cos(roll));

    message.setAccelCalibrated(WiimoteVector3(yaw, pitch, roll));
    emit wiimoteMessage(message);
}


#define curMessage mesg[message]
#define curMesgGyroAngleRate curMessage.motionplus_mesg.angle_rate
#define curMesgAccel curMessage.acc_mesg.acc
//#define curMesgGyroLowSpeed(x) (curMessage.motionplus_mesg.low_speed[x] == 1)

void cwiidCallback(cwiid_wiimote_t *wiimote, int mesg_count, union cwiid_mesg mesg[], timespec *timestamp)
{
    if(!lastMessageTime.contains(wiimote)) {
        lastMessageTime[wiimote]=*timestamp;
        qDebug() << "Skipping first message";
        return;
    }
    CWiidConnectionWorker *worker = workers[wiimote];
    if(!worker) return;
    qint64 deltaTime = TimeUtils::nsecDifference(lastMessageTime[wiimote], *timestamp);
    lastMessageTime[wiimote]=*timestamp;

    WiimoteMessage wiimoteMessage(deltaTime);

    for(int message = 0; message<mesg_count; message++) {
#ifdef CWIID_MESG_MOTIONPLUS 
        if(curMessage.type == CWIID_MESG_MOTIONPLUS) {
            WiimoteVector3 gyro(    curMesgGyroAngleRate[CWIID_X],
                              curMesgGyroAngleRate[CWIID_Y],
                              curMesgGyroAngleRate[CWIID_Z] );
//            gyro /= lowSpeedDivider;
//            WiimoteVector3 multiplyForHighSpeed(
//                        curMesgGyroLowSpeed(CWIID_X) ? 1 : highSpeedMultiplier,
//                        curMesgGyroLowSpeed(CWIID_Y) ? 1 : highSpeedMultiplier,
//                        curMesgGyroLowSpeed(CWIID_Z) ? 1 : highSpeedMultiplier
//                        );
//            gyro *= multiplyForHighSpeed;
            wiimoteMessage.setGyro(gyro);
        }
        else
#endif
          if(curMessage.type == CWIID_MESG_ACC) {
            WiimoteVector3 accel(
              curMesgAccel[CWIID_X],
              curMesgAccel[CWIID_Y],
              curMesgAccel[CWIID_Z]
              );
            wiimoteMessage.setAccel(accel);
        } else if(curMessage.type == CWIID_MESG_STATUS) {
            qDebug() << "Got status message: " <<
                        curMessage.status_mesg.battery
                     << ", " << curMessage.status_mesg.ext_type;
        } else if(curMessage.type == CWIID_MESG_BTN) {
            worker->emitButtons(curMessage.btn_mesg.buttons);
        }
    }
    worker->emitMessage(wiimoteMessage);
}


void CWiidConnectionWorker::wiimoteDisconnect()
{
    QMutexLocker locker(mutex); Q_UNUSED(locker)
    if(cwiid_close(wiimote) == 0) {
        emit disconnected(bluetoothAddress);
        workers.remove(wiimote);
        qDebug() << "Disconnected";
    }
}

QString addressToString(bdaddr_t address) {
    return QString("%1:%2:%3:%4:%5:%6")
            .arg(address.b[5], 2, 16, QChar('0'))
            .arg(address.b[4], 2, 16, QChar('0'))
            .arg(address.b[3], 2, 16, QChar('0'))
            .arg(address.b[2], 2, 16, QChar('0'))
            .arg(address.b[1], 2, 16, QChar('0'))
            .arg(address.b[0], 2, 16, QChar('0'));
}

void CWiidConnectionWorker::wiimoteConnect()
{
    QMutexLocker locker(mutex); Q_UNUSED(locker)
    bluetoothAddress = QString();
    bdaddr_t address = (bdaddr_t) {{0, 0, 0, 0, 0, 0}};
    qDebug() << "Trying to connect wiimote...";
    emit deviceMessage("Connecting to wiimote...", 0);
    wiimote =  cwiid_open(&address, CWIID_FLAG_MESG_IFC);
    if(wiimote) {
        bluetoothAddress = addressToString(address);
        emit deviceMessage("Connected.", 5000);
        qDebug() << "Connected!";
        int ok = cwiid_get_acc_cal(wiimote, CWIID_EXT_NONE, &accelerometerCalibration);
        if(ok!=0) qDebug() << "error getting accel calibration";
        else {
            accelCalibZero = WiimoteVector3(accelerometerCalibration.zero[CWIID_X], accelerometerCalibration.zero[CWIID_Y], accelerometerCalibration.zero[CWIID_Z]);
            accelCalibOne = WiimoteVector3(accelerometerCalibration.one[CWIID_X], accelerometerCalibration.one[CWIID_Y], accelerometerCalibration.one[CWIID_Z]);
            qDebug() << "Got calibration: \nzero=" << accelCalibZero.toString() << "\none =" << accelCalibOne.toString();
        }

//        if(cwiid_set_rpt_mode(wiimote, CWIID_RPT_MOTIONPLUS | CWIID_RPT_ACC | CWIID_RPT_STATUS )) qDebug() << "Error setting report ";
        if(cwiid_set_rpt_mode(wiimote, CWIID_RPT_BTN | CWIID_RPT_STATUS )) qDebug() << "Error setting report ";
//        if(cwiid_enable(wiimote, CWIID_FLAG_MESG_IFC)) qDebug() << "Error enabling messages";
//        if(cwiid_enable(wiimote, CWIID_FLAG_CONTINUOUS)) qDebug() << "Error enabling continuous";
        cwiid_set_mesg_callback(wiimote, &cwiidCallback);
#ifdef CWIID_FLAG_MOTIONPLUS
        if(cwiid_enable(wiimote, CWIID_FLAG_MOTIONPLUS)) qDebug() << "Error enabling motionplus";
#endif
        setLeds(CWIID_LED1_ON);
        cwiid_request_status(wiimote);
        workers.insert(wiimote, this);

        emit connected(bluetoothAddress);
    } else {
        emit deviceMessage("Error connecting", 5000);
        emit disconnected(QString());
    }
}

void CWiidConnectionWorker::setLeds(int ledsMask)
{
  if(cwiid_command(wiimote, CWIID_CMD_LED, ledsMask)) qDebug() << "Error setting led1 on";
}





void CWiidConnectionWorker::emitButtons(quint16 buttonsMask)
{
    emit buttons(buttonsMask);
}
