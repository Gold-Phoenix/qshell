# Q::Shell

![Screenshot](/screenshots/4.png)
![Screenshot](/screenshots/5.png)


a simple desktop shell based on KF5 and QT5, inspired by plasmashell and BE::Shell.

## Build dependencies

You will need your distro's equivalent to:

- extra-cmake-modules
- Qt5 (Core, DBus, Widgets, X11Extras components)
- KF5 (Config, KIO, WindowSystem components)
- KF5Solid
- Xlib
- Pulseaudio
- compton-tryone
- kde plasma
- i3wm
- dex
- kvantum


To make sure the left top panel works you will need the following dependencies:

- jq
- xdotool
- perl-anyevent-i3
- wmctrl



It is recommended to install KDE Plasma along side this as Q::Shell utilizes many of Plasma's features.

For Ubuntu/Debian-based systems, this command should be sufficient:

```
sudo apt install extra-cmake-modules qtbase5-dev libx11-dev libkf5crash-dev libkf5kio-dev libkf5solid-dev libkf5jobwidgets-dev libkf5textwidgets-dev libkf5bookmarks-dev libkf5xmlgui-dev libkf5itemviews-dev libkf5attica-dev libkf5sonnet-dev libkf5globalaccel-dev libkf5guiaddons-dev libkf5codecs-dev libkf5auth-dev libkf5dbusaddons-dev libkf5coreaddons-dev libkf5iconthemes-dev libkf5configwidgets-dev libkf5widgetsaddons-dev libkf5service-dev libkf5config-dev libkf5windowsystem-dev libqt5concurrent5 libpulse-dev libqt5x11extras5 libqt5x11extras5-dev
```

For Arch, use this:

```
sudo pacman -S extra-cmake-modules plasma-meta qt5 kf5 solid libx11 jq xdotool perl-anytime-i3 wmctrl kvantum-qt5
```

## Build and install

```
mkdir build
cd build
cmake ..
make
sudo make install
```
A Q::Shell session will appear on your display manager's session selection.


## Run

After qshell has been installed the first thing you will want to do is move the qshellrc and the qshell.css, from the themes folder from inside the qshell folder (for qshell use sora), into your .config directory. When done edit this qshellrc and change every instance of '/home/user/' to your current user name in order for it to work.

Copy over the folder containing the top bar images and infopanel.sh to .config and rename the folder to qshell.
Also remember to sudo chmod +x the infopanel.sh file in the .config/qshell directory. Also copy over the compton.conf straight into your .config folder.

Once that is done copy and paste check-windows and change-layout from the bin folder into your /bin/ folder, also remember to sudo chmod +x both these files.

Next copy and paste the 2 script files from scripts and paste them anywhere, from within kdeplasma open 'Autostart', click add script, and select kde-i3.sh. Then set it to startup before session startup. After that do the same for qshelllaunch.sh but set it to startup on startup.

After all this is done, qshell should be ready to launch, keep in mind this will take over your kdeplasma DE and replace it with Qshell.


## Sidenotes

To get the most out of this DE, make sure you install kvantum and use the Akava-Kv theme, also make sure to install compton-tryone and the compton.conf for full blur and transparency effect. 

From within kdeplasma you can go into system settings and navigate to application style, from there choose kvantum, then back and into colours and select kvantum or kvantumAlt.

For more info on the ideal setup for Qshell check out this page for reference on all aspects of customizability for this DE, from the maker of this DE:

https://www.reddit.com/r/unixporn/comments/aotk1r/i3gapsqshell_tile_in_style/


## Known Bugs

As of the latest versions of KDE Plasma, kstartupconfig5 no longer exsits which means that the offical version of Qshell and its session on the display manager no longer work.

Opening or using the volume in the top bar, then switching to something will completely freeze Qshell and require a restart of the display manager in order to restart Qshell. So try to avoid using the volume bar or just straight up remove it if that bothers you.

## Credits

 * Qt and KF5
 * [BE::Shell](https://sourceforge.net/projects/be-shell/)
