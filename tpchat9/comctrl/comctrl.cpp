#include "comctrl.h"
#include <iostream>
using namespace std;

ComCtrl::ComCtrl(QWidget *parent) :
    QMainWindow(parent)
{
    connect(&udpcom, SIGNAL(Udpcom::receiveBroad()), this, SLOT(receiveTPC()));
}

void ComCtrl::receiveTPC(void)
{
    cout<<"receiveTPC"<<endl;
    cout<< QByteArray((udpcom.getReceiveMsg()).toLatin1()).data() <<endl;

    if (this->myName.isEmpty())
    {
        return ;
    }

    tpcJson.JsonToTpc(udpcom.getReceiveMsg());

    CommandDecode comdec = tpcJson.getCommand();
    this->senderName = tpcJson.getName();
    this->senderportrait = tpcJson.getPortrait();
    int errmsgid = tpcJson.getmsgid();
    QHostAddress senderip = this->getSenderIP();

    QString jsonString;

    cout<<comdec<<endl;

    switch (comdec)
    {
    // json解析失败，回复401
    case NEW_JSON_ERR:
        cout<<"NEW_JSON_ERR"<<endl;
        jsonString = tpcJson.TpcToJson(RESP_LOGIN, ERR_JSON, "", "", 0, 0, "", true, this->myportrait);
        udpcom.sendSingleMsg(jsonString, senderip);
        break;
    // 协议解析失败，回复401
    case NEW_TPC_ERR:
        cout<<"NEW_TPC_ERR"<<endl;
        jsonString = tpcJson.TpcToJson(RESP_LOGIN, ERR_JSON, "", "", 0, 0, "", true, this->myportrait);
        udpcom.sendSingleMsg(jsonString, senderip);
        break;
    // 收到新用户上线广播请求
    case NEW_LOGIN_REQ:
        cout<<"NEW_LOGIN_REQ"<<endl;
        emit receivelogin(this->senderName, senderip, this->senderportrait);
        break;
    // 收到登录响应json错误，如果是自己的请求则重试
    case NEW_LOGIN_JSON:
        cout<<"NEW_LOGIN_JSON"<<endl;
        if (this->senderName == this->myName)
        {
            sendLogin(this->myName, this->myportrait);
        }
        break;
    // 新用户登录请求解析错误，回复401
    case NEW_LOGIN_JSON_SEND:
        cout<<"NEW_LOGIN_JSON_SEND"<<endl;
        jsonString = tpcJson.TpcToJson(RESP_LOGIN, ERR_JSON, "", "", 0, 0, "", false, this->myportrait);
        udpcom.sendSingleMsg(jsonString, senderip);
        break;
    // 收到登录用户名重复错误，发信号给main进行修改
    case NEW_LOGIN_DUP:
        cout<<"NEW_LOGIN_DUP"<<endl;
        emit receiveloginduplicate();
        break;
    // 登录成功，接收在线用户名及其ip
    case NEW_LOGIN_OK:
        cout<<"NEW_LOGIN_OK"<<endl;
        emit receiveloginok(this->senderName, senderip, this->senderportrait);
        break;
    // 收到用户离线广播，发信号给main
    case NEW_LOGOUT_REQ:
        cout<<"NEW_LOGOUT_REQ"<<endl;
        this->senderName = tpcJson.getName();
        emit receivelogout(this->senderName);
        break;
    // 收到聊天信息
    case NEW_CHAT_REQ:
        cout<<"NEW_CHAT_REQ"<<endl;
        receiveChat(senderip);
        break;
    // 收到聊天响应json错误，发信号通知main发送失败
    case NEW_CHAT_JSON:
        cout<<"NEW_CHAT_JSON"<<endl;
        if (!((this->dstuser).isEmpty()) && (this->dstuser == this->senderName))
        {
            emit receivechaterror(this->senderName, errmsgid, senderip);
        }
        break;
    // 聊天请求解析错误，回复401
    case NEW_CHAT_JSON_SEND:
        cout<<"NEW_CHAT_JSON_SEND"<<endl;
        jsonString = tpcJson.TpcToJson(RESP_CHAT, ERR_JSON, "", "", 0, this->msgid, "", false, this->myportrait);
        udpcom.sendSingleMsg(jsonString, senderip);
        break;
    // 收到聊天响应目标用户错误，发信号通知main发送失败
    case NEW_CHAT_DST:
        cout<<"NEW_CHAT_DST"<<endl;
        if (!((this->dstuser).isEmpty()) && (this->dstuser == this->senderName))
        {
            emit receivechaterror(this->senderName, errmsgid, senderip);
        }
        break;
    // 收到聊天响应消息长度超过限制错误，发信号通知main发送失败
    case NEW_CHAT_LEN:
        cout<<"NEW_CHAT_LEN"<<endl;
        if (!((this->dstuser).isEmpty()) && (this->dstuser == this->senderName))
        {
            emit receivechaterror(this->senderName, errmsgid, senderip);
        }
        break;
    // 收到聊天响应消息内容长度与msglen不匹配错误，发信号通知main发送失败
    case NEW_CHAT_BODYLEN:
        cout<<"NEW_CHAT_BODYLEN"<<endl;
        if (!((this->dstuser).isEmpty()) && (this->dstuser == this->senderName))
        {
            emit receivechaterror(this->senderName, errmsgid, senderip);
        }
    // 收到正常的聊天响应
    case NEW_CHAT_OK:
        cout<<"NEW_CHAT_OK"<<endl;
        if (!((this->dstuser).isEmpty()) && (this->dstuser == this->senderName))
        {
            emit receivechatok(this->senderName, errmsgid, senderip);
        }
        break;

    default:
        break;
    }
}

void ComCtrl::sendLogin(QString myname, PortraitID myportrait)
{
    this->myName = myname;
    this->myportrait = myportrait;
    QString jsonString = tpcJson.TpcToJson(SENT_LOGIN, ERR_NO, myname, "", 0, 0, "", false, myportrait);
    udpcom.sendBroadMsg(jsonString);
}

void ComCtrl::sendLoginRes(bool isdup, QHostAddress dstip)
{
    ErrCode errorCode;
    if (isdup == true)
    {
        errorCode = ERR_LOGIN;
    }
    else
    {
        errorCode = ERR_NO;
    }
    QString jsonString = tpcJson.TpcToJson(RESP_LOGIN, errorCode, this->myName, "", 0, 0, "", false, this->myportrait);
    udpcom.sendSingleMsg(jsonString, dstip);
    //udpcom.sendBroadMsg(jsonString);
}

void ComCtrl::sendLogout(QString myname)
{
    QString jsonString = tpcJson.TpcToJson(SENT_LOGOUT, ERR_NO, myname, "", 0, 0, "", false, this->myportrait);
    udpcom.sendBroadMsg(jsonString);
}

void ComCtrl::sendChat(QString dstuser, QString msg, int msgid, QHostAddress dstip)
{
    this->dstuser = dstuser;
    this->MsgToSend = msg;
    this->msgid = msgid;
    int msglen = msg.length();

    QString jsonString = tpcJson.TpcToJson(SENT_CHAT, ERR_NO, this->myName, dstuser, msglen, msgid, msg, false, this->myportrait);
    udpcom.sendSingleMsg(jsonString, dstip);
    //udpcom.sendBroadMsg(jsonString);
}

void ComCtrl::receiveChat(QHostAddress dstip)
{
    QString srcuser = tpcJson.getSrcuser();
    QString dstuser = tpcJson.getDstuser();
    int msglen = tpcJson.getmsglen();
    int msgid = tpcJson.getmsgid();
    QString msgbody = tpcJson.getMsg();

    this->senderName = srcuser;
    int len = msgbody.length();
    ErrCode errorCode = ERR_NO;
    QString jsonString;

    if (dstuser != this->myName)
    {
        errorCode = ERR_RECEIVE;
    }
    if (msglen >= MAXLEN)
    {
        errorCode = ERR_MAXLENGTH;
    }
    if (msglen != len)
    {
        errorCode = ERR_MSGLENGTH;
    }

    if (errorCode == ERR_NO)
    {
        emit receivechat(srcuser, msgbody, msgid, dstip);
    }
    jsonString = tpcJson.TpcToJson(RESP_CHAT, errorCode, this->myName, "", 0, msgid, "", false, this->myportrait);
    udpcom.sendSingleMsg(jsonString, dstip);
    //udpcom.sendBroadMsg(jsonString);
}

QString ComCtrl::getMyName(void)
{
    return this->myName;
}

QHostAddress ComCtrl::getSenderIP(void)
{
    this->senderIP = QHostAddress(udpcom.getIP());
    return this->senderIP;
}

QString ComCtrl::getSenderName(void)
{
    return this->senderName;
}

QString ComCtrl::getReceiveMsg(void)
{
    return this->receiveMsg;
}

int ComCtrl::getMsgID(void)
{
    return this->msgid;
}

PortraitID ComCtrl::getSenderportrait(void)
{
    return this->senderportrait;
}
