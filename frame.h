#ifndef FRAME_H
#define FRAME_H

namespace Q
{

class Frame : public QWidget
{
    Q_OBJECT
public:
    Frame(QWidget *parent = 0);
    void setCentralWidget(QWidget *widget);
protected:
    virtual void resizeEvent(QResizeEvent *) override;
private:
    QWidget *widget;
};

};

#endif
