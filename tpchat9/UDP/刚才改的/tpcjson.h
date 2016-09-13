#ifndef TPCJSON_H
#define TPCJSON_H

#include <QMainWindow>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>

#include "constant.h"

class TpcJson : public QMainWindow
{
    Q_OBJECT
public:
    explicit TpcJson(QWidget *parent = 0);

signals:

public slots:

private:
    CommandDecode comdec;
    MessageType type;
    ErrCode error;
    QString name;
    QString srcuser;
    QString dstuser;
    int msglen;
    int msgid;
    QString msgbody;

public slots:
    /** the TPC data to send */
    QString TpcToJson(MessageType type, ErrCode error, QString srcuser, QString dstuser,
                  int msglen, int msgid, QString msgbody, bool jsonerr);

    /** decode json from receive msg */
    void JsonToTpc(QString receiveMsg);

    MessageType getMsgType();
    CommandDecode getCommand();
    ErrCode getErrCode();
    QString getName();
    QString getSrcuser();
    QString getDstuser();
    int getmsgid();
    int getmsglen();
    QString getMsg();

private slots:
    CommandDecode TpcDecode(QString typeStr, int errcode, QString name, QString srcuser, QString dstuser,
                            int msglen, int msgid, QString msgbody, bool isRespond, bool jsonerr);

};

#endif // TPCJSON_H
