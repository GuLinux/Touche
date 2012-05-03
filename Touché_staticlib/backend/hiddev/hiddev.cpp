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

#include <QDebug>
#include "domain/deviceinfo.h"


class HidDevPrivate {
public:
    HidDevPrivate(const QString &device, HidDev *parent) : q_ptr(parent) { deviceInfo.path(device); }
    int fd=-1;
    QTimer readTimer;
    fd_set fdset;
    DeviceInfo deviceInfo;
    HidDev* const q_ptr;
    Q_DECLARE_PUBLIC(HidDev)

    void stop()
    {
        Q_Q(HidDev);
        if(fd>=0) {
            close(fd);
            fd=-1;
        }
        q->thread()->quit();
    }

    void read_events()
    {
        Q_Q(HidDev);
        if(fd<0) return;

        timeval tv;
        tv.tv_sec=1;
        tv.tv_usec=0;
        FD_SET(fd, &(fdset));
        int selectRD = select(fd+1, &(fdset), NULL, NULL, &tv);

        if(selectRD==-1) {
            qDebug() << "Error on select: " << strerror(errno);
            stop();
            q->emit removed(& deviceInfo);
            return;
        }

        if(!selectRD>0) {
            return;
        }

        struct hiddev_event ev[256];
        int rd = read(fd, ev, sizeof(ev));
        if(rd==-1) {
            stop();
            q->emit removed(& deviceInfo);
            qDebug() << "Error on read: " << strerror(errno);
            return;
        }
        InputEvent *keyEvent = new InputEvent(q);
        for(int i=0; i<rd; i++) {
            keyEvent->addRegister(ev[i].hid, ev[i].value, i);
        }
        q->emit event(keyEvent, &deviceInfo);
        QTimer::singleShot(60000, keyEvent, SLOT(deleteLater()));
    }
};





HidDev::HidDev(const QString &path, QObject *parent):
    QObject(parent), d_ptr(new HidDevPrivate(path, this))
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

    while(d->fd==-1) {
        d->fd = open(d->deviceInfo.path().toLatin1(), O_RDONLY);
        if(d->fd==-1) {
            qDebug() << "Error on open: " << strerror(errno) << "; retrying in a while";
        }
        qApp->processEvents();
    }
    qDebug() << "Hid dev opened: " << d->fd;


    int version;

    QTextStream err(stderr);
    ioctl(d->fd, HIDIOCGVERSION, &version);
    err << QString("hiddev driver version is %1.%2.%3\n").arg(version >> 16).arg((version >> 8) & 0xff).arg(version & 0xff);

    ioctl(d->fd, HIDIOCGDEVINFO, &dinfo);
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
    d->deviceInfo.name(name);
    err << QString("HID device name: \"%1\"\n").arg(d->deviceInfo.name());
    FD_ZERO(&(d->fdset));

    emit connected(&d->deviceInfo);
    while(d->fd!=-1) {
        qApp->processEvents();
        d->read_events();
    }
    qDebug() << "Device loop stopped";
}






