#ifndef COMCTRL_H
#define COMCTRL_H

#include <QMainWindow>
#include "../TPC/constant.h"
#include "../udp/udpcom.h"
#include "../TPC/tpcjson.h"

class ComCtrl : public QMainWindow
{
    Q_OBJECT
public:
    explicit ComCtrl(QWidget *parent = 0);

private:
    QHostAddress senderIP;
    QString myName;
    QString dstuser;
    QString senderName;
    QString receiveMsg;
    QString MsgToSend;
    int msgid;
    PortraitID myportrait;
    PortraitID senderportrait;
    Udpcom udpcom;
    TpcJson tpcJson;

signals:
    void receivelogin(QString, QHostAddress, PortraitID);   // 局域网新用户上线，包含发送者，发送者ip，发送者头像ID
    void receiveloginok(QString, QHostAddress, PortraitID); // 本机可以登录，接收已上线用户名及IP，发送者头像ID
    void receiveloginduplicate();                           // 用户名在网络中存在
    void receivelogout(QString);                            // 有用户离线，包含发送者
    void receivechat(QString, QString, int, QHostAddress);  // 收到聊天信息，包含发送者，消息内容，消息id，发送者ip
    void receivechatok(QString, int, QHostAddress);         // 收到正常聊天响应，即消息发送成功，包含发送者，消息id，发送者ip
    void receivechaterror(QString, int, QHostAddress);      // 聊天信息发送失败，标记对应id的msg为失败

public slots:
    // 发送登录请求，输入自身用户名及头像ID
    void sendLogin(QString myname, PortraitID myportrait);
    // 发送登录响应，输入是否重复及对方IP
    void sendLoginRes(bool isdup, QHostAddress dstip);
    // 发送下线通知，输入自身用户名
    void sendLogout(QString myname);
    // 发送聊天请求，输入对方用户名，消息内容，消息ID，对方IP
    void sendChat(QString dstuser, QString msg, int msgid, QHostAddress dstip);

    QString getMyName(void);
    QHostAddress getSenderIP(void);
    QString getSenderName(void);
    QString getReceiveMsg(void);
    int getMsgID(void);
    PortraitID getSenderportrait(void);

private slots:
    void receiveTPC(void);
    void receiveChat(QHostAddress dstip);
};

#endif // COMCTRL_H
