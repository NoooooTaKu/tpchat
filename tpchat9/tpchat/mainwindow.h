#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>
#include "personList.h"
#include "login.h"
#include <QUdpSocket>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

signals:
    // MainWindow处理登录申请发现局域网该用户已登录
	// 回调Login槽函数激活登陆界面重新填写用户名
    void reLogin();

private:
    // Login传递过来的登录用户名
    QString myName;

	// MainWindow显示界面
    QLabel *labelImage;      // 头像
    QLabel *labelName;       // 登录用户名
    QTabWidget * tabWidget;  // 三个标签
    QWidget *tabOnChat;      // 会话
    personList *tabContacts; // 联系人
    QWidget *tabSettings;    // 设置

    QPushButton *btnAbout;         // 关于
    QPushButton *btnQuit;          // 退出
    QPushButton *btnHeadPortrait;  // 头像设置
    QPushButton *btnNoticeMsg;     // 消息提醒
    QPushButton *btnClearLog;      // 清空记录

    Login *loginWindow;      // 登录窗口对象
    QTimer *loginTimer;      // 登录等待500ms计时器
    int loginTimerOutFlag;   // unused

	// 测试UDP
    QUdpSocket udpSocket;
    void udpSend(QString);
    void udpSendOnlineMsg();

private slots:
    void btnAboutClicked();
    void btnQuitClicked();

	// 关闭登录窗口，显示主界面
    void showMainWindow();
	// 登录等待500ms
    void loginTimerOut();
	// 处理登录申请
    void handleLoginApply(QString);

    void udpReceive();
};

#endif // MAINWINDOW_H
