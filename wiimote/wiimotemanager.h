#ifndef WIIMOTEMANAGER_H
#define WIIMOTEMANAGER_H

#include <QObject>
#include "angles.h"

class Wiimote;
class CWiidConnectionWorker;
class WiimoteManager : public QObject
{
    Q_OBJECT
public:
    explicit WiimoteManager(QObject *parent = 0);
    ~WiimoteManager();
    int calibrationTotalSteps() const;

signals:
    void connected(const QString &address);
    void disconnected(const QString &address);
    void deviceMessage(const QString &message, int timeout);
    void angleChanged(WiimoteVector3 angles);
    void deltaChanged(WiimoteVector3 angles);
    void accelChanged(WiimoteVector3 angles);
    void accelCalibratedChanged(WiimoteVector3 angles);
    void calibrationStep(int remainingSeconds);
    void calibrated();
    void needsCalibration();
    void buttonsDown(const QStringList &buttons);

public slots:
    void setCalibrationSteps(int seconds);
    void connectWiimote();
    void disconnectWiimote();
    void calibrateWiimote();
    void resetAngles();
    void resetAngles(double yaw, double pitch, double roll);
    
private:
    CWiidConnectionWorker *worker;
    QThread *thread;
    Wiimote *wiimote;
};

#endif // WIIMOTEMANAGER_H
