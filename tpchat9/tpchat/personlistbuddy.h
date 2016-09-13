#ifndef PERSONLISTBUDDY_H
#define PERSONLISTBUDDY_H

#include <QWidget>
#include <QtWidgets>
#include "chatwindow.h"

class personListBuddy : public QWidget
{
    Q_OBJECT
public:
    explicit personListBuddy(QWidget *parent = 0);
    void initUi();//初始化Ui
    //QWidget *head; //头像
    QLabel *head; //头像
    QLabel *name; //用户名
    QLabel *sign; //个性签名
    QString headPath;//头像路径

	// 与该联系人的聊天窗口
    ChatWindow *chatWindow;

signals:

public slots:

private:

};

#endif // PERSONLISTBUDDY_H
