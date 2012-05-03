#include "tokeybinding.h"
#include <QDebug>

#include <X11/extensions/XTest.h>
#include <X11/Xlib.h>
#include <QX11Info>

class ToKeyBindingPrivate {
public:
    ToKeyBindingPrivate(const QString &keysymname, bool iskeypress) : keySymName(keysymname), iskeypress(iskeypress) {}
    QString const keySymName;
    bool iskeypress;
};

ToKeyBinding::ToKeyBinding(const QString &keySymName, bool iskeypress, QObject *parent) :
    QObject(parent), d_ptr(new ToKeyBindingPrivate(keySymName, iskeypress))
{
}

ToKeyBinding::~ToKeyBinding()
{
    qDebug() << "ToKeyBinding deleted";
    delete d_ptr;
}

void ToKeyBinding::execute()
{
    Q_D(ToKeyBinding);
    KeySym keysym = XStringToKeysym(d->keySymName.toLatin1());
    KeyCode keycode = XKeysymToKeycode(QX11Info::display(), keysym);
    qDebug() << "executing \"ToKeyBinding\": sending key" << (d->iskeypress ? "press" : "release") << " with keysym:" << keysym << "; keycode: " << keycode;
    XTestFakeKeyEvent(QX11Info::display(), keycode, d->iskeypress, 0 );
}
