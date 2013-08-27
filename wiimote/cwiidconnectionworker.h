#ifndef CWIIDCONNECTIONWORKER_H
#define CWIIDCONNECTIONWORKER_H
#include "cwiid.h"
#include "wiimote_messages.h"
#include <QObject>
class QMutex;

class CWiidConnectionWorker : public QObject
{
    Q_OBJECT
public:
    explicit CWiidConnectionWorker(QObject *parent = 0);
    ~CWiidConnectionWorker();
    void emitMessage(WiimoteMessage message);
    void emitButtons(quint16 buttonsMask);
signals:
    void connected(const QString address);
    void disconnected(const QString address);
    void wiimoteMessage(WiimoteMessage message);
    void deviceMessage(const QString &message, int timeout);
    void buttons(int buttonsMask);
public slots:
    void setLeds(int ledsMask);
    void wiimoteConnect();
    void wiimoteDisconnect();
private slots:
private:
    cwiid_wiimote_t *wiimote;
    acc_cal accelerometerCalibration;
    WiimoteVector3 accelCalibZero;
    WiimoteVector3 accelCalibOne;
    QMutex *mutex;
    QString bluetoothAddress;
};

#endif // CWIIDCONNECTIONWORKER_H
