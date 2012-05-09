#ifndef TRAYMANAGER_H
#define TRAYMANAGER_H
#include <QString>
#include <QObject>
class TrayManager {
public:
    virtual QAction *createAction(const QString &text, QObject *parent=0) =0;
    virtual void showMessage(const QString &title, const QString &text, const QString &iconTheme = QString()) =0;
    virtual void updateTooltip(const QString &tooltip) = 0;
};

#endif // TRAYMANAGER_H
