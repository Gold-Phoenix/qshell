#include <QWidget>
#include <QHBoxLayout>
#include <QLabel>
#include <QX11Info>
#include <QIcon>
#include <QPushButton>
#include <QMenu>
#include <QAction>
#include <QKeySequence>
#include <QApplication>
#include <QScreen>
#include <QTimer>
#include <QMouseEvent>
#include <QDebug>

#include <KF5/KWindowSystem/KWindowSystem>
#include <KF5/KWindowSystem/NETWM>
#include <KF5/KWindowSystem/KWindowInfo>

#include "winctrl.h"
#include "model.h"
#include "shell.h"

void toggleMaximize()
{
    NETWinInfo info(QX11Info::connection(), KWindowSystem::activeWindow(), QX11Info::appRootWindow(), NET::WMState, 0);
    if(info.state() & NET::Max)
        KWindowSystem::clearState(KWindowSystem::activeWindow(), NET::Max);
    else
        KWindowSystem::setState(KWindowSystem::activeWindow(), NET::Max);
};

void closeWindow()
{
    NETRootInfo(QX11Info::connection(), NET::CloseWindow).closeWindowRequest(KWindowSystem::activeWindow());
};

void minimize()
{
    KWindowSystem::minimizeWindow(KWindowSystem::activeWindow());
};

// ----------

Q::WinTitle::WinTitle(QWidget *parent) : QLabel(parent), timer(0)
{
    populateContextMenu();
};

// Clicks
void Q::WinTitle::doubleClick()
{
    toggleMaximize();
};

void Q::WinTitle::click()
{
    KWindowInfo info(KWindowSystem::activeWindow(), NET::WMState);
    if(info.state() & NET::SkipTaskbar)
        return;
    populateContextMenu();
    QRect geometry = QGuiApplication::primaryScreen()->geometry();
    Shell *shell = static_cast<WinCtrl*>(parentWidget())->shell();
    myContextMenu.popup(QPoint(
        std::min(shell->getStrutLeft() + parentWidget()->x(), geometry.width() - width()),
        parentWidget()->y() + parentWidget()->parentWidget()->height()
    ));
};

// Mouse Events
void Q::WinTitle::mousePressEvent(QMouseEvent *event)
{
    isDoubleClick = false;
};

void Q::WinTitle::mouseDoubleClickEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
        isDoubleClick = true;
};

void Q::WinTitle::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button() == Qt::RightButton)
        click();
    else if(!isDoubleClick)
    {
        timer = new QTimer();
        timer->setInterval(QApplication::doubleClickInterval() * 0.5);
        timer->setSingleShot(true);
        connect(timer, &QTimer::timeout, [this]() {
            click();
        });
        timer->start();
        return;
    }
    else
    {
        doubleClick();
    }
    
    if(timer)
        timer->stop();
    
};

// Context Menu
void Q::WinTitle::populateContextMenu()
{
    KWindowInfo info(KWindowSystem::activeWindow(), NET::WMName|NET::WMState);

    myContextMenu.clear();

    QAction *act;

    act = new QAction(QIcon::fromTheme("up"), "Keep above others");
    act->setCheckable(true);
    act->setChecked(info.state() & NET::StaysOnTop);
    connect(act, &QAction::triggered, [info](){
        if(info.state() & NET::StaysOnTop)
            KWindowSystem::clearState(KWindowSystem::activeWindow(), NET::StaysOnTop);
        else
            KWindowSystem::setState(KWindowSystem::activeWindow(), info.state() | NET::StaysOnTop);
    });
    myContextMenu.addAction(act);

    act = new QAction(QIcon::fromTheme("down"), "Keep below others");
    act->setCheckable(true);
    act->setChecked(info.state() & NET::KeepBelow);
    connect(act, &QAction::triggered, [info](){
        if(info.state() & NET::KeepBelow)
            KWindowSystem::clearState(KWindowSystem::activeWindow(), NET::KeepBelow);
        else
            KWindowSystem::setState(KWindowSystem::activeWindow(), info.state() | NET::KeepBelow);
    });
    myContextMenu.addAction(act);

    myContextMenu.addSeparator();

    act = new QAction(QIcon::fromTheme("window-minimize"), "Maximize");
    connect(act, &QAction::triggered, [](){ minimize(); });
    myContextMenu.addAction(act);

    if(info.state() & NET::Max)
    {
        act = new QAction(QIcon::fromTheme("window-restore"), "Unmaximize");
        connect(act, &QAction::triggered, [](){ toggleMaximize(); });
        myContextMenu.addAction(act);
    }
    else
    {
        act = new QAction(QIcon::fromTheme("window-maximize"), "Maximize");
        connect(act, &QAction::triggered, [](){ toggleMaximize(); });
        myContextMenu.addAction(act);
    }

    act = new QAction(QIcon::fromTheme("window-close"), "Close");
    act->setShortcut(QKeySequence("Alt+F4"));
    connect(act, &QAction::triggered, [](){ closeWindow(); });
    myContextMenu.addAction(act);
};

// ----------

Q::WinCtrl::WinCtrl(const QString& name, Shell* shell) : QWidget(), Model(name, shell)
{
    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setSpacing(0);
    layout->setMargin(0);
    layout->setContentsMargins(0,0,0,0);
    setLayout(layout);

    closeBtn = new QPushButton();
    closeBtn->setIcon(QIcon::fromTheme("window-close"));
    boxLayout()->addWidget(closeBtn);

    minimizeBtn = new QPushButton();
    minimizeBtn->setIcon(QIcon::fromTheme("window-minimize"));
    boxLayout()->addWidget(minimizeBtn);
    
    maximizeBtn = new QPushButton();
    maximizeBtn->setIcon(QIcon::fromTheme("window-restore"));
    boxLayout()->addWidget(maximizeBtn);

    label = new WinTitle(this);
    boxLayout()->addWidget(label);
    update();

    connect(KWindowSystem::self(), SIGNAL(activeWindowChanged(WId)), this, SLOT(update(WId)));
    connect(KWindowSystem::self(), SIGNAL(windowChanged(WId, const unsigned long*)), this, SLOT(update()));
    connect(closeBtn, &QPushButton::clicked, [this]() {
        closeWindow();
    });
    connect(minimizeBtn, &QPushButton::clicked, [this]() {
        minimize();
        update();
    });
    connect(maximizeBtn, &QPushButton::clicked, []() {
        toggleMaximize();
    });
};

void Q::WinCtrl::update(WId wid)
{
    KWindowInfo info(wid, NET::WMName|NET::WMState);
    label->setText(info.name());
    if(info.state() & NET::Max)
    {
        closeBtn->show();
        minimizeBtn->show();
        maximizeBtn->show();
    }
    else
    {
        closeBtn->hide();
        minimizeBtn->hide();
        maximizeBtn->hide();
    }
};
