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

#include "x11keysymbolscompleter.h"
#include <QDebug>

class X11KeySymbolsCompleterPrivate {
public:
    X11KeySymbolsCompleterPrivate() {
        wordList << "XF86ModeLock";
        wordList << "XF86MonBrightnessUp";
        wordList << "XF86MonBrightnessDown";
        wordList << "XF86KbdLightOnOff";
        wordList << "XF86KbdBrightnessUp";
        wordList << "XF86KbdBrightnessDown";
        wordList << "XF86Standby";
        wordList << "XF86AudioLowerVolume";
        wordList << "XF86AudioMute";
        wordList << "XF86AudioRaiseVolume";
        wordList << "XF86AudioPlay";
        wordList << "XF86AudioStop";
        wordList << "XF86AudioPrev";
        wordList << "XF86AudioNext";
        wordList << "XF86HomePage";
        wordList << "XF86Mail";
        wordList << "XF86Start";
        wordList << "XF86Search";
        wordList << "XF86AudioRecord";
        wordList << "XF86Calculator";
        wordList << "XF86Memo";
        wordList << "XF86ToDoList";
        wordList << "XF86Calendar";
        wordList << "XF86PowerDown";
        wordList << "XF86ContrastAdjust";
        wordList << "XF86RockerUp";
        wordList << "XF86RockerDown";
        wordList << "XF86RockerEnter";
        wordList << "XF86Back";
        wordList << "XF86Forward";
        wordList << "XF86Stop";
        wordList << "XF86Refresh";
        wordList << "XF86PowerOff";
        wordList << "XF86WakeUp";
        wordList << "XF86Eject";
        wordList << "XF86ScreenSaver";
        wordList << "XF86WWW";
        wordList << "XF86Sleep";
        wordList << "XF86Favorites";
        wordList << "XF86AudioPause";
        wordList << "XF86AudioMedia";
        wordList << "XF86MyComputer";
        wordList << "XF86VendorHome";
        wordList << "XF86LightBulb";
        wordList << "XF86Shop";
        wordList << "XF86History";
        wordList << "XF86OpenURL";
        wordList << "XF86AddFavorite";
        wordList << "XF86HotLinks";
        wordList << "XF86BrightnessAdjust";
        wordList << "XF86Finance";
        wordList << "XF86Community";
        wordList << "XF86AudioRewind";
        wordList << "XF86BackForward";
        wordList << "XF86Launch0";
        wordList << "XF86Launch1";
        wordList << "XF86Launch2";
        wordList << "XF86Launch3";
        wordList << "XF86Launch4";
        wordList << "XF86Launch5";
        wordList << "XF86Launch6";
        wordList << "XF86Launch7";
        wordList << "XF86Launch8";
        wordList << "XF86Launch9";
        wordList << "XF86LaunchA";
        wordList << "XF86LaunchB";
        wordList << "XF86LaunchC";
        wordList << "XF86LaunchD";
        wordList << "XF86LaunchE";
        wordList << "XF86LaunchF";
        wordList << "XF86ApplicationLeft";
        wordList << "XF86ApplicationRight";
        wordList << "XF86Book";
        wordList << "XF86CD";
        wordList << "XF86Calculater";
        wordList << "XF86Clear";
        wordList << "XF86Close";
        wordList << "XF86Copy";
        wordList << "XF86Cut";
        wordList << "XF86Display";
        wordList << "XF86DOS";
        wordList << "XF86Documents";
        wordList << "XF86Excel";
        wordList << "XF86Explorer";
        wordList << "XF86Game";
        wordList << "XF86Go";
        wordList << "XF86iTouch";
        wordList << "XF86LogOff";
        wordList << "XF86Market";
        wordList << "XF86Meeting";
        wordList << "XF86MenuKB";
        wordList << "XF86MenuPB";
        wordList << "XF86MySites";
        wordList << "XF86New";
        wordList << "XF86News";
        wordList << "XF86OfficeHome";
        wordList << "XF86Open";
        wordList << "XF86Option";
        wordList << "XF86Paste";
        wordList << "XF86Phone";
        wordList << "XF86Q";
        wordList << "XF86Reply";
        wordList << "XF86Reload";
        wordList << "XF86RotateWindows";
        wordList << "XF86RotationPB";
        wordList << "XF86RotationKB";
        wordList << "XF86Save";
        wordList << "XF86ScrollUp";
        wordList << "XF86ScrollDown";
        wordList << "XF86ScrollClick";
        wordList << "XF86Send";
        wordList << "XF86Spell";
        wordList << "XF86SplitScreen";
        wordList << "XF86Support";
        wordList << "XF86TaskPane";
        wordList << "XF86Terminal";
        wordList << "XF86Tools";
        wordList << "XF86Travel";
        wordList << "XF86UserPB";
        wordList << "XF86User1KB";
        wordList << "XF86User2KB";
        wordList << "XF86Video";
        wordList << "XF86WheelButton";
        wordList << "XF86Word";
        wordList << "XF86Xfer";
        wordList << "XF86ZoomIn";
        wordList << "XF86ZoomOut";
        wordList << "XF86Away";
        wordList << "XF86Messenger";
        wordList << "XF86WebCam";
        wordList << "XF86MailForward";
        wordList << "XF86Pictures";
        wordList << "XF86Music";
        wordList << "XF86Battery";
        wordList << "XF86Bluetooth";
        wordList << "XF86WLAN";
        wordList << "XF86UWB";
        wordList << "XF86AudioForward";
        wordList << "XF86AudioRepeat";
        wordList << "XF86AudioRandomPlay";
        wordList << "XF86Subtitle";
        wordList << "XF86AudioCycleTrack";
        wordList << "XF86CycleAngle";
        wordList << "XF86FrameBack";
        wordList << "XF86FrameForward";
        wordList << "XF86Time";
        wordList << "XF86Select";
        wordList << "XF86View";
        wordList << "XF86TopMenu";
        wordList << "XF86Red";
        wordList << "XF86Green";
        wordList << "XF86Yellow";
        wordList << "XF86Blue";
        wordList << "XF86Suspend";
        wordList << "XF86Hibernate";
        wordList << "XF86TouchpadToggle";
        wordList << "XF86TouchpadOn";
        wordList << "XF86TouchpadOff";
        wordList << "XF86Switch_VT_1";
        wordList << "XF86Switch_VT_2";
        wordList << "XF86Switch_VT_3";
        wordList << "XF86Switch_VT_4";
        wordList << "XF86Switch_VT_5";
        wordList << "XF86Switch_VT_6";
        wordList << "XF86Switch_VT_7";
        wordList << "XF86Switch_VT_8";
        wordList << "XF86Switch_VT_9";
        wordList << "XF86Switch_VT_10";
        wordList << "XF86Switch_VT_11";
        wordList << "XF86Switch_VT_12";
        wordList << "XF86Ungrab";
        wordList << "XF86ClearGrab";
        wordList << "XF86Next_VMode";
        wordList << "XF86Prev_VMode";
        wordList << "XF86LogWindowTree";
        wordList << "XF86LogGrabInfo";
    }
    QStringList wordList;
};


X11KeySymbolsCompleterPrivate staticKeySymbols;

X11KeySymbolsCompleter::X11KeySymbolsCompleter(QObject *parent) :
    QCompleter(staticKeySymbols.wordList, parent)
{
    setCaseSensitivity(Qt::CaseInsensitive);
}

X11KeySymbolsCompleter::~X11KeySymbolsCompleter()
{
}

QStringList X11KeySymbolsCompleter::keySymbols()
{
    return staticKeySymbols.wordList;
}
