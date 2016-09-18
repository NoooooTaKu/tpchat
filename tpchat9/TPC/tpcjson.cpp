#include "tpcjson.h"

TpcJson::TpcJson(QWidget *parent) :
    QMainWindow(parent)
{
}

QString TpcJson::TpcToJson(MessageType type, ErrCode error, QString srcuser, QString dstuser,
              int msglen, int msgid, QString msgbody, bool jsonerr, PortraitID portraitID)
{
    QJsonObject json;

    QJsonObject json_chat;
    json_chat.insert("srcuser", srcuser);
    json_chat.insert("dstuser", dstuser);
    json_chat.insert("msglen", QString::number(msglen));
    json_chat.insert("msgid", QString::number(msgid));
    json_chat.insert("msgbody", msgbody);

    switch (type)
    {
    case SENT_LOGIN:
        json.insert("type", QString("hellotpc"));
        json.insert("name", srcuser);
        json.insert("portrait", QString::number((int)portraitID));
        break;

    case SENT_CHAT:
        json.insert("type", QString("unichat"));
        json.insert("entry", json_chat);
        break;

    case SENT_LOGOUT:
        json.insert("type", QString("bye"));
        json.insert("name", srcuser);
        break;

    case RESP_LOGIN:
        switch (error)
        {
        case ERR_JSON:
            if (jsonerr == true)
            {
                json.insert("type", QString("unknown"));
                json.insert("errcode", QString::number(error));
            }
            else
            {
                json.insert("type", QString("hellotpc"));
                json.insert("errcode", QString::number(error));
            }
            break;

        default:
            json.insert("type", QString("login"));
            json.insert("errcode", QString::number(error));
            json.insert("name", srcuser);
            json.insert("portrait", QString::number((int)portraitID));
            break;
        }
        break;

    case RESP_CHAT:
        json.insert("type", QString("unichat"));
        json.insert("errcode", QString::number(error));
        json.insert("msgid", QString::number(msgid));
        json.insert("name", srcuser);
        break;

    default:
        break;
    }

    QJsonDocument document;
    document.setObject(json);
    QByteArray byte_array = document.toJson(QJsonDocument::Indented);
    QString sendMsg(byte_array);
    return sendMsg;
}

void TpcJson::JsonToTpc(QString receiveMsg)
{
    this->comdec = NO_COMMAND;
    this->portraitID = PORTRAIT_DEFAULT;

    bool isRespond = false;
    bool jsonerr = false;
    QByteArray byte_array;
    byte_array = receiveMsg.toUtf8();
    QJsonParseError json_error;
    QJsonDocument parse_doucment = QJsonDocument::fromJson(byte_array, &json_error);
    QJsonObject obj;
    QJsonValue type_value;
    QJsonValue name_value;
    QJsonValue error_value;
    QJsonValue msgid_value;
    QJsonValue entry_value;
    QJsonValue portrait_value;
    QString typeStr;
    int errcode;
    int msgidRes;

    QJsonObject eobj;
    QJsonValue esrcuser_value;
    QJsonValue edstuser_value;
    QJsonValue emsglen_value;
    QJsonValue emsgid_value;
    QJsonValue emsgbody_value;

    if (json_error.error == QJsonParseError::NoError)
    {
        if (parse_doucment.isObject())
        {
            obj = parse_doucment.object();
            if (obj.contains("type"))
            {
                type_value = obj.take("type");
                if (type_value.isString())
                {
                    typeStr = type_value.toString();
                }
                if (typeStr == QString("hellotpc") && (!obj.contains("name") && !obj.contains("errcode")))
                {
                    this->comdec = NEW_LOGIN_JSON_SEND;
                    return ;
                }
                if (typeStr == QString("unichat") && !obj.contains("entry"))
                {
                    this->comdec = NEW_CHAT_JSON_SEND;
                    return ;
                }
            }
            else
            {
                this->comdec = NEW_TPC_ERR;
                return ;
            }

            if (obj.contains("name"))
            {
                name_value = obj.take("name");
                if (name_value.isString())
                {
                    this->name = name_value.toString();
                }
            }

            if (obj.contains("portrait"))
            {
                portrait_value = obj.take("portrait");
                if (portrait_value.isString())
                {
                    this->portraitID = PortraitID(portrait_value.toString().toInt());
                }
            }

            if (obj.contains("errcode"))
            {
                isRespond = true;
                error_value = obj.take("errcode");
                if (error_value.isString())
                {
                    errcode = error_value.toString().toInt();
                }
            }

            if (obj.contains("msgid"))
            {
                msgid_value = obj.take("msgid");
                if (msgid_value.isString())
                {
                    msgidRes = msgid_value.toString().toInt();
                }
            }

            if (obj.contains("entry"))
            {
                this->type = SENT_CHAT;
                entry_value = obj.take("entry");
                if (entry_value.isObject())
                {
                    eobj = entry_value.toObject();
                    if (eobj.contains("srcuser") && eobj.contains("dstuser") && eobj.contains("msglen") && eobj.contains("msgid") && eobj.contains("msgbody"))
                    {
                        esrcuser_value = obj.take("srcuser");
                        if (esrcuser_value.isString())
                        {
                            this->srcuser = esrcuser_value.toString();
                        }
                        edstuser_value = obj.take("dstuser");
                        if (edstuser_value.isString())
                        {
                            this->dstuser = edstuser_value.toString();
                        }
                        emsglen_value = obj.take("msglen");
                        if (emsglen_value.isString())
                        {
                            this->msglen = emsglen_value.toString().toInt();
                        }
                        emsgid_value = obj.take("msgid");
                        if (emsgid_value.isString())
                        {
                            this->msgid = emsgid_value.toString().toInt();
                        }
                        emsgbody_value = obj.take("msgbody");
                        if (emsgbody_value.isString())
                        {
                            this->msgbody = emsgbody_value.toString();
                        }
                    }
                    else
                    {
                        this->comdec = NEW_CHAT_JSON_SEND;
                        return ;
                    }
                }
            }
        }
    }
    else
    {
        jsonerr = true;
    }

    this->error = (ErrCode)errcode;
    this->comdec = TpcDecode(typeStr, errcode, msgidRes, isRespond, jsonerr);
}

CommandDecode TpcJson::TpcDecode(QString typeStr, int errcode, int msgidRes, bool isRespond, bool jsonerr)
{
    CommandDecode comdec = NO_COMMAND;
    if (jsonerr == true)
    {
        comdec = NEW_JSON_ERR;
    }
    else
    {
        if (isRespond == true)
        {
            switch (errcode)
            {
            case ERR_JSON:
                comdec = NEW_LOGIN_JSON;
                if (typeStr == QString("unichat"))
                {
                    comdec = NEW_CHAT_JSON;
                    this->msgid = msgidRes;
                }
                break;
            case ERR_LOGIN:
                comdec = NEW_LOGIN_DUP;
                break;
            case ERR_RECEIVE:
                comdec = NEW_CHAT_DST;
                this->msgid = msgidRes;
                break;
            case ERR_MAXLENGTH:
                comdec = NEW_CHAT_LEN;
                this->msgid = msgidRes;
                break;
            case ERR_MSGLENGTH:
                comdec = NEW_CHAT_BODYLEN;
                this->msgid = msgidRes;
                break;
            case ERR_NO:
                if (typeStr == QString("login"))
                {
                    comdec = NEW_LOGIN_OK;
                }
                if (typeStr == QString("unichat"))
                {
                    comdec = NEW_CHAT_OK;
                    this->msgid = msgidRes;
                }
                break;
            default:
                comdec = NO_COMMAND;
                break;
            }
        }
        else
        {
            if (typeStr == QString("hellotpc"))
            {
                comdec = NEW_LOGIN_REQ;
            }
            if (typeStr == QString("bye"))
            {
                comdec = NEW_LOGOUT_REQ;
            }
            if (typeStr == QString("unichat"))
            {
                comdec = NEW_CHAT_REQ;
            }
        }
    }
    return comdec;
}

MessageType TpcJson::getMsgType()
{
    return this->type;
}

CommandDecode TpcJson::getCommand()
{
    return this->comdec;
}

ErrCode TpcJson::getErrCode()
{
    return this->error;
}

QString TpcJson::getName()
{
    return this->name;
}

QString TpcJson::getSrcuser()
{
    return this->srcuser;
}

QString TpcJson::getDstuser()
{
    return this->dstuser;
}

int TpcJson::getmsgid()
{
    return this->msgid;
}

int TpcJson::getmsglen()
{
    return this->msglen;
}

QString TpcJson::getMsg()
{
    return this->msgbody;
}

PortraitID TpcJson::getPortrait()
{
    return this->portraitID;
}
