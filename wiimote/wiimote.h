#ifndef WIIMOTE_H
#define WIIMOTE_H

#include <QObject>
#include <QVector>
#include <QVector3D>
#include "wiimote_messages.h"
#include "angles.h"
#include <QStringList>


#define CALIBRATION_STEPS 15

class QMutex;
class QTimer;
class Wiimote : public QObject
{
    Q_OBJECT
public:
    explicit Wiimote(QObject *parent = 0);
    ~Wiimote();
    inline int calibrationTotalSteps() const { return m_calibrationTotalSteps; }
signals:
    void angleChanged(WiimoteVector3 angles);
    void deltaChanged(WiimoteVector3 angles);
    void accelChanged(WiimoteVector3 values);
    void accelCalibratedChanged(WiimoteVector3 values);
    void calibrationStep(int remainingSeconds);
    void calibrated();
    void needsCalibration();
    void buttonsDown(const QStringList &buttons);

public slots:
    void motionPlusEvent(WiimoteMessage message);
    void calibrate(WiimoteMessage message);
    void calibrate();
    void calibrationFinished();
    void reset();
    void reset(double yaw, double pitch, double roll);
    void reset(WiimoteVector3 angles);
    void setCalibrationSteps(int seconds);
    void buttons(int buttonsMask);

private:
    WiimoteVector3  m_calibration_gyro;
    WiimoteVector3  m_current_angles;
    QVector<WiimoteMessage> m_calibration_messages;
    bool m_is_calibrating;
    int m_calibrationStep;
    QTimer *m_calibrationTimer;
    int m_calibrationTotalSteps;
    QMutex *mutex;
};

#endif // WIIMOTE_H
