#ifndef WIIMOTE_MESSAGES_H
#define WIIMOTE_MESSAGES_H

#include <Qt>
#include <QMetaType>
#include "angles.h"


class WiimoteMessage {
public:
    WiimoteMessage();
    WiimoteMessage(qint64 deltaTime);
    inline qint64 deltaTime() { return m_deltaTime; }
    WiimoteMessage *setGyro(WiimoteVector3 message);
    WiimoteMessage *setAccel(WiimoteVector3 message);
    WiimoteMessage *setAccelCalibrated(WiimoteVector3 message);
    inline WiimoteVector3 gyro() { return gyro_coordinates; }
    inline WiimoteVector3 accel() { return accel_coordinates; }
    inline WiimoteVector3 accelCalibrated() { return accel_calibrated; }
private:
    qint64 m_deltaTime;
    WiimoteVector3 gyro_coordinates;
    WiimoteVector3 accel_coordinates;
    WiimoteVector3 accel_calibrated;
};


#endif // WIIMOTE_MESSAGES_H
