#include "dumpkeys.h"
#include <QTextStream>
#include "domain/inputevent.h"


DumpKeys::DumpKeys(QObject *parent) :
    QObject(parent)
{
}

DumpKeys::~DumpKeys()
{
}

void DumpKeys::event(InputEvent *keyEvent)
{
    QTextStream out(stdout);
    out << keyEvent->asJSON();
    out << endl;
    out.flush();
}
// o << QString("index[%1]:hid=[%2]v=[%3]").arg(i, 2, 16, QChar('0')).arg(ev[i].hid, 8, 16, QChar('0')).arg((uint)ev[i].value, 8, 16, QChar('0')) << endl;
