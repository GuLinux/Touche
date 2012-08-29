#include "wiimote.h"
#include <QDebug>
#include <QTimer>
#include <QMetaType>
#include <QMutex>
#include <QMutexLocker>
#include <math.h>
#include <cwiid.h>

Wiimote::Wiimote(QObject *parent) :
    QObject(parent), m_calibration_gyro(true),
    m_is_calibrating(false),
    m_calibrationTimer(new QTimer(this)),
    m_calibrationTotalSteps(CALIBRATION_STEPS)
{
    m_calibrationTimer->setInterval(1000);
    connect(m_calibrationTimer, SIGNAL(timeout()), this, SLOT(calibrationFinished()));
    mutex = new QMutex();
}

Wiimote::~Wiimote()
{
    delete mutex;
}


void Wiimote::calibrate(WiimoteMessage message)
{
    m_calibration_messages << message;
}

void Wiimote::calibrate()
{
    m_is_calibrating = true;
    m_calibrationStep = 0;
    m_calibrationTimer->start();
}

WiimoteVector3 getAverageData(const QVector<WiimoteMessage> &messages) {
    WiimoteVector3 gyro;
    quint64 gyro_valid_messages = 0;
    foreach(WiimoteMessage message, messages) {
        if(!message.gyro().isNull()) {
            gyro += WiimoteVector3(message.gyro().yaw(),message.gyro().pitch(),message.gyro().roll());
            gyro_valid_messages++;
        }
    }
    gyro /= gyro_valid_messages;
    return gyro;
}

void Wiimote::calibrationFinished()
{

    if(m_calibrationStep < m_calibrationTotalSteps ) {
        m_calibrationStep++;
        emit(calibrationStep(m_calibrationTotalSteps - m_calibrationStep));
        return;
    }
    m_calibrationTimer->stop();
    m_calibration_gyro = getAverageData(m_calibration_messages);

    qDebug() << "zero for gyro =" << m_calibration_gyro.toString() << " (" << m_calibration_messages.size() << " reads)";
//    qDebug() << "zero for accel=" << m_calibration_accel.toString() << " (" << accel_valid_messages << " reads)";
    m_is_calibrating = false;
    m_calibration_messages.clear();
    emit calibrated();
}

void Wiimote::reset()
{
    reset(0,0,0);
}

void Wiimote::reset(double yaw, double pitch, double roll)
{
    reset(WiimoteVector3(yaw, pitch, roll));
}


void Wiimote::reset(WiimoteVector3 angles)
{
    m_current_angles=angles;
}

void Wiimote::setCalibrationSteps(int seconds)
{
    m_calibrationTotalSteps = seconds;
}

double roundTo(double number, double digits) {
    double multiplier = pow(10, digits);
    double result = qRound64(number * multiplier);
    return result/multiplier;
}


void Wiimote::motionPlusEvent(WiimoteMessage message)
{
    QMutexLocker lock(mutex);
    Q_UNUSED(lock);
//    emit accelChanged(message.accel());
//    emit accelCalibratedChanged(message.accelCalibrated());
    if(m_is_calibrating) {
        calibrate(message);
        return;
    }
    if(m_calibration_gyro.isNull() ) {
        emit needsCalibration();
        return;
    }

    WiimoteVector3 anglesSpeed = message.gyro() - m_calibration_gyro;
    double deltaT = ((double)message.deltaTime()) / 1000000000.0;
    WiimoteVector3 delta = anglesSpeed * deltaT;
    delta /= 1000;

    double roundFactor = 3.5;
    WiimoteVector3 roundDelta = WiimoteVector3(roundTo(delta.yaw(), roundFactor), roundTo(delta.pitch(), roundFactor), roundTo(delta.roll(), roundFactor));

//    qDebug() << "got angular speed (raw) " << anglesSpeed.toString();
//    qDebug() << "got angular delta " << roundDelta.toString();
    if(roundDelta != WiimoteVector3(0,0,0))
        emit deltaChanged(roundDelta);
    WiimoteVector3 previousAngles = m_current_angles;
    m_current_angles += roundDelta;
//    qDebug() << "current angles: " << m_current_angles.toString();
    if(previousAngles != m_current_angles) emit angleChanged(m_current_angles);
}




void Wiimote::buttons(int buttonsMask)
{
    QStringList buttons;
    if(buttonsMask & CWIID_BTN_1) buttons << "WiimoteButton_1";
    if(buttonsMask & CWIID_BTN_2) buttons << "WiimoteButton_2";
    if(buttonsMask & CWIID_BTN_A) buttons << "WiimoteButton_A";
    if(buttonsMask & CWIID_BTN_B) buttons << "WiimoteButton_B";
    if(buttonsMask & CWIID_BTN_DOWN) buttons << "WiimoteButton_Down";
    if(buttonsMask & CWIID_BTN_HOME) buttons << "WiimoteButton_Home";
    if(buttonsMask & CWIID_BTN_LEFT) buttons << "WiimoteButton_Left";
    if(buttonsMask & CWIID_BTN_MINUS) buttons << "WiimoteButton_Minus";
    if(buttonsMask & CWIID_BTN_PLUS) buttons << "WiimoteButton_Plus";
    if(buttonsMask & CWIID_BTN_UP) buttons << "WiimoteButton_Up";
    if(buttonsMask & CWIID_BTN_RIGHT) buttons << "WiimoteButton_Right";
    emit buttonsDown(buttons);
}
