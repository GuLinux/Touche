#ifndef DATABASEENTRY_H
#define DATABASEENTRY_H

#include <QObject>
#include <QVariant>
#include <QList>

class ConfigEvent;
class DatabaseEntryPrivate;
class InputEvent;
class DatabaseEntry : public QObject {
    Q_OBJECT
public:
    static DatabaseEntry *fromConfig(QMap<QString, QVariant> config, QObject *parent);
    ~DatabaseEntry();
    QList<ConfigEvent *> configuredEvents();
private:
    DatabaseEntry(QObject *parent);
    DatabaseEntryPrivate * const d_ptr;
    Q_DECLARE_PRIVATE(DatabaseEntry)
};

#endif // DATABASEENTRY_H
