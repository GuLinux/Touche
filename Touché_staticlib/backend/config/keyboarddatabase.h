#ifndef KEYBOARDDATABASE_H
#define KEYBOARDDATABASE_H

#include <QObject>
#include <QStringList>

class DeviceInfo;
class KeyboardDatabasePrivate;
class DatabaseEntry;
class KeyboardDatabase : public QObject
{
    Q_OBJECT
public:
    explicit KeyboardDatabase(QStringList databaseFiles, QObject *parent =0);
    ~KeyboardDatabase();
    DatabaseEntry *keyboard(DeviceInfo* deviceInfo);
signals:
    
public slots:
    void deviceAdded(DeviceInfo *device);
    void deviceRemoved(DeviceInfo *device);
private:
    KeyboardDatabasePrivate * const d_ptr;
    Q_DECLARE_PRIVATE(KeyboardDatabase)
};



#endif // KEYBOARDDATABASE_H
