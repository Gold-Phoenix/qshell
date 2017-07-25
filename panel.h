#ifndef PANEL_H
#define PANEL_H

#include <QWidget>
#include <QBoxLayout>
#include <QPoint>

#include <X11/Xatom.h>
#include <QX11Info>
#include <X11/Xlib.h>
#include <fixx11h.h>

#include <KF5/KConfigCore/KConfigGroup>

#include "model.h"

namespace Q
{

enum PanelPosition { Left = 0, Top, Right, Bottom };

class Shell;
class PanelShadow : public QWidget
{
public:
    PanelShadow(QWidget *parent = 0);
};
class Panel : public QWidget, public Model
{
    Q_OBJECT
public:
    Panel(const QString& name, Shell *shell);
    inline QBoxLayout *boxLayout() const { return static_cast<QBoxLayout*>(layout()); };
    void addWidget(QWidget *widget);
    void addStretch(int stretch = 0);
    inline PanelPosition position() const { return myPosition; };
    inline const QPoint& point() const { return myPoint; };
    inline const bool struts() const { return setStruts; };
    inline const QString& name() const { return myName; };
    inline const bool displaysShadow() const { return displayShadow; };
    inline int iconSize() { return myIconSize; };
    void load(KConfigGroup *grp) override;
protected:
    virtual void showEvent(QShowEvent *);
    virtual void paintEvent(QPaintEvent *);
public slots:
    void geometryChanged();
private:
    Shell *myShell;
    QString myName;
    QString myWidth, myHeight;
    PanelPosition myPosition;
    QPoint myPoint;
    bool setStruts;
    int blurRadius;
    bool displayShadow;
    int myIconSize;
    float offsetTop, offsetLeft, offsetRight, offsetBottom;
};

}

#endif
