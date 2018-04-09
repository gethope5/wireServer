#ifndef EXPANDPUSHBUTTON_H
#define EXPANDPUSHBUTTON_H

#include <QPushButton>
#include <qtextcodec.h>
class expandPushButton : public QPushButton
{
    Q_OBJECT
public:
    explicit expandPushButton(QWidget *parent = 0);

signals:

public slots:
protected:
    void enterEvent(QEvent *);
    void leaveEvent(QEvent *);
};

#endif // EXPANDPUSHBUTTON_H
