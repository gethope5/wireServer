#include "expandpushbutton.h"

expandPushButton::expandPushButton(QWidget *parent) :
    QPushButton(parent)
{
    this->setToolTip(QTextCodec::codecForName("gbk")->toUnicode("单击扩展界面..."));
}
void expandPushButton::enterEvent(QEvent *)
{
    this->setCursor(Qt::OpenHandCursor );
}
void expandPushButton::leaveEvent(QEvent *)
{
    this->setCursor(Qt::ArrowCursor          );
}
