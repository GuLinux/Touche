#ifndef DUMPKEYS_H
#define DUMPKEYS_H

#include <QObject>

class InputEvent;
class DumpKeys : public QObject
{
    Q_OBJECT
public:
    explicit DumpKeys(QObject *parent = 0);
    ~DumpKeys();
signals:
    
public slots:
    void event(InputEvent *keyEvent);
private:
};

#endif // DUMPKEYS_H
