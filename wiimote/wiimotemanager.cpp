#include "wiimotemanager.h"
#include "wiimote.h"
#include "cwiidconnectionworker.h"
#include <QThread>

WiimoteManager::WiimoteManager(QObject *parent) :
    QObject(parent), wiimote(new Wiimote(this))
{
    worker = new CWiidConnectionWorker();
    thread = new QThread(this);
    worker->moveToThread(thread);
    connect(worker, SIGNAL(wiimoteMessage(WiimoteMessage)), wiimote, SLOT(motionPlusEvent(WiimoteMessage)));
    connect(worker, SIGNAL(connected()), this, SIGNAL(connected()));
    connect(worker, SIGNAL(disconnected()), this, SIGNAL(disconnected()));
    connect(worker, SIGNAL(deviceMessage(QString,int)), this, SIGNAL(deviceMessage(QString,int)));

    connect(wiimote, SIGNAL(angleChanged(WiimoteVector3)), this, SIGNAL(angleChanged(WiimoteVector3)));
    connect(wiimote, SIGNAL(deltaChanged(WiimoteVector3)), this, SIGNAL(deltaChanged(WiimoteVector3)));
    connect(wiimote, SIGNAL(accelChanged(WiimoteVector3)), this, SIGNAL(accelChanged(WiimoteVector3)));
    connect(wiimote, SIGNAL(accelCalibratedChanged(WiimoteVector3)), this, SIGNAL(accelCalibratedChanged(WiimoteVector3)));
    connect(wiimote, SIGNAL(calibrationStep(int)), this, SIGNAL(calibrationStep(int)));
    connect(wiimote, SIGNAL(calibrated()), this, SIGNAL(calibrated()));
    connect(wiimote, SIGNAL(needsCalibration()), this, SIGNAL(needsCalibration()));
}

WiimoteManager::~WiimoteManager()
{
    thread->quit();
    thread->wait();
    delete worker;
    thread->terminate();
    delete thread;
}

int WiimoteManager::calibrationTotalSteps() const
{
    return wiimote->calibrationTotalSteps();
}


void WiimoteManager::connectWiimote()
{
    thread->start();
    QMetaObject::invokeMethod(worker, "wiimoteConnect", Qt::QueuedConnection);
}

void WiimoteManager::disconnectWiimote()
{
    QMetaObject::invokeMethod(worker, "wiimoteDisconnect", Qt::QueuedConnection);
    thread->quit();
    thread->wait();
}

void WiimoteManager::calibrateWiimote()
{
    wiimote->calibrate();
}

void WiimoteManager::resetAngles()
{
    resetAngles(0,0,0);
}

void WiimoteManager::resetAngles(double yaw, double pitch, double roll)
{
    wiimote->reset(yaw, pitch, roll);
}


void WiimoteManager::setCalibrationSteps(int seconds)
{
    wiimote->setCalibrationSteps(seconds);
}
