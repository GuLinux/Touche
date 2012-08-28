#include "wiimote_messages.h"


Q_DECLARE_METATYPE(WiimoteMessage)

class _RegisterWiimoteMessagesMetatypes {
public:
    _RegisterWiimoteMessagesMetatypes() {
        qRegisterMetaType<WiimoteMessage>();
    }
};

static _RegisterWiimoteMessagesMetatypes registerWiimoteMessagesMetatypes;

WiimoteMessage::WiimoteMessage()
{
}

WiimoteMessage::WiimoteMessage(qint64 deltaTime)
    : m_deltaTime(deltaTime)
{
}

WiimoteMessage *WiimoteMessage::setGyro(WiimoteVector3 message)
{
    this->gyro_coordinates = message;
    return this;
}

WiimoteMessage *WiimoteMessage::setAccel(WiimoteVector3 message)
{
    this->accel_coordinates = message;
    return this;
}

WiimoteMessage *WiimoteMessage::setAccelCalibrated(WiimoteVector3 message)
{
    this->accel_calibrated = message;
    return this;
}
