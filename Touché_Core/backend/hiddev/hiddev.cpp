/***********************************************************************
Copyright (c) 2012 "Marco Gulino <marco.gulino@gmail.com>"

This file is part of Touché: https://github.com/rockman81/Touche

Touché is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details (included the COPYING file).

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
***********************************************************************/

#include "hiddev.h"
#include <linux/hiddev.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/select.h>

#include <QTextStream>
#include <QTimer>
#include "domain/inputevent.h"
#include <QCoreApplication>
#include <QThread>
#include <QMutex>
#include <QMutexLocker>
#include <QElapsedTimer>

#include <QDebug>
#include "backend/config/databaseentry.h"
#include "domain/deviceinfo.h"
#include "backend/config/keyboarddatabase.h"

#define OPEN_TIMEOUT 5000


class HidDevPrivate {
public:
    HidDevPrivate(const QString &device, KeyboardDatabase* keyboardDatabase, HidDev *parent) : fd(-1), had_events(false), keyboardDatabase(keyboardDatabase), q_ptr(parent), aboutToQuit(false) {
        deviceInfo.path(device);
    }
    int fd;
    QTimer readTimer;
    fd_set fdset;
    DeviceInfo deviceInfo;
    bool had_events;
    KeyboardDatabase* keyboardDatabase;
    HidDev* const q_ptr;
    QElapsedTimer errors_timer;
    ulong read_timeout_milliseconds;
    bool aboutToQuit;
    QMutex mutex;
    Q_DECLARE_PUBLIC(HidDev)

    void stop()
    {
        if(fd<0) return;
        aboutToQuit=true;
        if(fd>=0) {
            close(fd);
            fd=-1;
        }
    }

    void read_events()
    {
        if(fd<0 || aboutToQuit) return;
        QMutexLocker locker(&mutex);
        Q_UNUSED(locker);
        Q_Q(HidDev);

        timeval tv;
        tv.tv_sec=0;
        tv.tv_usec=read_timeout_milliseconds*1000;
        FD_SET(fd, &(fdset));
        int selectRD = select(fd+1, &(fdset), NULL, NULL, &tv);
        if(selectRD==-1) {
            qDebug() << "Error on select: " << strerror(errno);
            stop();
            return;
        }

        if(!selectRD>0) {
            if(had_events) {
                had_events=false;
                emit q->noMoreEvents(&deviceInfo);
            }
            return;
        }

        struct hiddev_event ev[256];
        int rd = read(fd, ev, sizeof(ev));
        if(rd==-1) {
            stop();
            qDebug() << "Error on read: " << strerror(errno);
            return;
        }
        InputEvent *keyEvent = new InputEvent(q);
        for(int i=0; i<rd; i++) {
            keyEvent->addRegister(ev[i].hid, ev[i].value, i);
        }
        q->emit event(keyEvent, &deviceInfo);
        had_events=true;
        QTimer::singleShot(30000, keyEvent, SLOT(deleteLater()));
    }
};





HidDev::HidDev(const QString &path, KeyboardDatabase *keyboardDatabase, QObject *parent):
    QObject(parent), d_ptr(new HidDevPrivate(path, keyboardDatabase, this))
{
}



HidDev::~HidDev()
{
    delete d_ptr;
}


void HidDev::start()
{
    Q_D(HidDev);
    struct hiddev_devinfo dinfo;
    char name[256] = "Unknown";

    d->errors_timer.start();
    while(d->fd==-1) {
        d->fd = open(d->deviceInfo.path().toLatin1(), O_RDONLY);
        if(d->fd==-1 && d->errors_timer.elapsed() > OPEN_TIMEOUT) {
            qDebug() << "Error on open: " << strerror(errno) << " for more than " << OPEN_TIMEOUT << " milliseconds; giving up.";
            // TODO: emit signal for gui notification
            return;
        }
        qApp->processEvents();
    }
    qDebug() << "Hid dev opened: " << d->fd;


    int version;

    QTextStream err(stderr);
    ioctl(d->fd, HIDIOCGVERSION, &version);
    err << QString("hiddev driver version is %1.%2.%3\n").arg(version >> 16).arg((version >> 8) & 0xff).arg(version & 0xff);

    ioctl(d->fd, HIDIOCGDEVINFO, &dinfo);
    qDebug() << "Product: " << QString("%1").arg((quint16) dinfo.product, 4, 16, QChar('0'));
    d->deviceInfo.vendor(dinfo.vendor)->productID(dinfo.product)->version(dinfo.version)
            ->bus(dinfo.busnum)->deviceNumber(dinfo.devnum)->interfaceNumber(dinfo.ifnum);
    err << QString("HID: vendor 0x%1 product 0x%2 version 0x%3\n")
           .arg(d->deviceInfo.vendor(), 0, 16).arg(d->deviceInfo.productID(), 0, 16).arg(d->deviceInfo.version(), 0, 16);

    err << QString("applications [%1]: %2").arg(dinfo.num_applications).arg(ioctl(d->fd, HIDIOCAPPLICATION, 0), 4, 16);
    for (uint i = 1; i < dinfo.num_applications; i++)
        err << QString(", %1").arg(ioctl(d->fd, HIDIOCAPPLICATION, i), 4, 16);
    err << endl;
    err << QString("HID: bus: %1 devnum: %2 ifnum: %3\n").arg(d->deviceInfo.bus()).arg(d->deviceInfo.deviceNumber()).arg(d->deviceInfo.interfaceNumber());
    ioctl(d->fd, HIDIOCGNAME(sizeof(name)), name);
    d->deviceInfo.deviceReportedName(name);
    err << QString("HID device name: \"%1\"\n").arg(d->deviceInfo.name());
    FD_ZERO(&(d->fdset));

    QMap<QString, QVariant> keyboardDatabaseEntry = d->keyboardDatabase->deviceConfiguration(&d->deviceInfo);
    d->read_timeout_milliseconds = keyboardDatabaseEntry.value("read_timeout_milliseconds", 300).toULongLong();
    d->deviceInfo.name(keyboardDatabaseEntry.value("name").toString());
    d->deviceInfo.keyboardDatabaseEntry(keyboardDatabaseEntry);

    qDebug() << "Read timeout set to " << d->read_timeout_milliseconds << " milliseconds";
    emit connected(&d->deviceInfo);
    while(d->fd!=-1 && !d->aboutToQuit) {
        qApp->processEvents();
        d->read_events();
    }
    qDebug() << "Device loop stopped";
    emit removed(&d->deviceInfo);
}

void HidDev::stop()
{
    Q_D(HidDev);
    qDebug() << "qapp said it's time to go...";
    QMutexLocker locker(&d->mutex);
    Q_UNUSED(locker);
    d->stop();
}






