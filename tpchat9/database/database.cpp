#include <QtSql/QSqlDatabase>
#include <QDebug>
#include <QSqlQuery>
#include <QDir>
#include <QTableView>
#include <QSqlTableModel>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include "database.h"

#define SENDFROMME  1
#define SENDTOME    0

Database::Database()
{
    // check if the db file exists
    struct passwd *pwd;
    uid_t userid;
    userid = getuid();
    pwd = getpwuid(userid); //username: pwd->pw_name

    bool dbFile = QFile::exists("/home/" + QString(pwd->pw_name) + "/chatdb");
    qDebug() << "/home/" + QString(pwd->pw_name) + "/chatdb";
    //if the db file does not exit, create one
    if (!dbFile)
    {
        qDebug() << "cannot open file";
        //TODO 打开文件失败时自动创建文件

    }
    if (!connectDatabase())
    {
        qDebug() << "cannot connect database";
        //db.close();
    }
    else
    {
        qDebug() << "open database seccess";
    }
}

Database::~Database()
{
    ;
}

/*
 * Function: connection
 * Description: try to connect the database
 * Input: void
 * Output: bool
 * Return: if connection seccess, return true; else return false
 */
bool Database::connectDatabase()
{
    struct passwd *pwd;
    uid_t userid;
    userid = getuid();
    pwd = getpwuid(userid); //username: pwd->pw_name
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("/home/" + QString(pwd->pw_name) + "/chatdb");
    db.setUserName("tpchat");
    db.setPassword("123456");
    if (!db.open())
    {
        return false;
    }
    return true;
}

/*
 * Function: createDb
 * Description: try to connect the database
 * Input: void
 * Output: bool
 * Return: if connection seccess, return true; else return false
 */
void Database::createDb()
{
    QSqlQuery query;
    //TODO complete the function
}

void Database::initTable(QString user)
{
    QSqlQuery query;
    QString strQuery;

    strQuery = "select * from " + user + "log";
    qDebug() << strQuery;
    query.exec(strQuery);

    if (!query.next())
    {
        strQuery = "CREATE TABLE " + user + "log "
                    +"("
                    +"send int, "
                    +"time varchar(30), "
                    +"person varchar(40), "
                    +"content varchar(1024) "
                    +")";
        query.exec(strQuery);
        qDebug() << strQuery;
        strQuery = "CREATE TABLE " + user + "partner "
                    +"("
                    +"ip varchar(20), "
                    +"person varchar(40), "
                    +"image int "
                    +")";
        //strQuery = "CREATE TABLE " + user + "partner LIKE templatepartner";
        query.exec(strQuery);
        qDebug() << strQuery;
    }
}

/*
 * Function: addLog
 * Description: add chat history to database
 * Input:   QString user, current user
 *          int send, indicate the sender is me (1) or partner (0)
 *          QString time, time of the chat log
 *          QString partner, the chat partner
 *          QString content, chat content
 * Output: void
 * Return: void
 */
void Database::addLog(QString user, QString time,
                      QString partner, QString content, int send)
{
    QSqlQuery query;
    QString strQuery;
    strQuery = "INSERT INTO " + user
                + "log (send, time, person, content) VALUES ("
                + QString::number(send) + ", '"
                + time + "', '"
                + partner + "', '"
                + content + "')";
    qDebug() << strQuery;
    query.exec(strQuery);
}

/*
 * Function: deleteLogByPartner
 * Description: delete chat history of user with a partner
 * Input:   QString user, current user
 *          QString partner, the chat partner
 * Output: void
 * Return: void
 */
void Database::deleteLogByPartner(QString user, QString partner)
{
    QSqlQuery query;
    QString strQuery;
    //Delete from 数据表名称 where 列名称=值
    strQuery = "DELETE FROM " + user + "log WHERE person = " + partner + ")";
    qDebug() << strQuery;
    strQuery = "DELETE FROM " + user + "partner WHERE person = "
                + partner + ")";
    qDebug() << strQuery;
    query.exec(strQuery);
}

/*
 * Function: deleteLogByUser
 * Description: delete chat history and contant list of user
 * Input:   QString user, current user
 * Output: void
 * Return: void
 */
void Database::deleteLogByUser(QString user)
{
    QSqlQuery query;
    QString strQuery;
    // drop table 数据表1名称，数据表2名称
    strQuery = "DROP TABLE" + user + "log, " + user + "partner";
    qDebug() << strQuery;
    query.exec(strQuery);
}

/*
 * Function: viewLogByPartner
 * Description: search log history of current user with a partner
 * Input:   QString user, current user
 * Output:  void
 * Return:  QStringList, includes log, every log has sender, time and content,
 *          they are all QString
 */
QStringList Database::viewLogByPartner(QString user, QString partner)
{
    QSqlQuery query;
    QString strQuery;

    strQuery = "SELECT * FROM " + user + "log WHERE person = '"
                + partner + "'";
    qDebug() << strQuery;
    query.exec(strQuery);

    //TODO 使用QStringList会不会占用内存过大,可否将数据库search的结果直接输出
    QStringList result;
    while (query.next())
    {
        if (query.value("send") == SENDFROMME)
        {
            result << "Me " + query.value("time").toString()
                   << query.value("content").toString();
        }
        else if (query.value("send") == SENDTOME)
        {
            result << query.value("person").toString() + " "
                      + query.value("time").toString()
                   << query.value("content").toString();
        }
    }
    return result;
}

/*
 * Function: closeDb
 * Description: close the connection with database when logout or quit derictly
 * Input: void
 * Output: void
 * Return: void
 */
void Database::closeDb()
{
    if (db.isOpen())
    {
        db.close();
        qDebug() << "database closed";
    }
}

bool Database::checkPartnerName(QString user, QString newuser)
{
    //return true;
    QSqlQuery query;
    QString strQuery;

    strQuery = "SELECT * FROM " + user + "partner WHERE person = '"
                + newuser + "'";
    qDebug() << strQuery;
    query.exec(strQuery);
    if(query.next())
    {
        qDebug() << "exist name";
        return true;
    }
    else
    {
        qDebug() << "not exist name";
        return false;
    }
}

//add a new user to the table [user]partner
void Database::addPartnerName(QString user, QString newuser, int image, QString ip)
{
    QSqlQuery query;
    QString strQuery;
    strQuery = "INSERT INTO " + user + "partner (person, image, ip) VALUES ('"
                + newuser + "', '"
                + QString::number(image) + "', '"
                + ip +"')";
    qDebug() << strQuery;
    query.exec(strQuery);
}

void Database::addUserName(QString user, int image)
{
    QSqlQuery query;
    QString strQuery;
    strQuery = "INSERT INTO loginhistory (user, image) VALUES ('"
                + user + "', '"
                + QString::number(image) + "')";
    qDebug() << strQuery;
    query.exec(strQuery);
}

bool Database::checkUserName(QString user)
{
    QSqlQuery query;
    QString strQuery;

    strQuery = "SELECT * FROM loginhistory WHERE user = '" + user + "'";
    qDebug() << strQuery;
    query.exec(strQuery);
    if(query.next())
    {
        qDebug() << "exist name";
        return true;
    }
    else
    {
        qDebug() << "not exist name";
        return false;
    }
}

void Database::addMsgToSendcache(QString time, QString dstuser,
                             QString content, int msgid)
{
    QSqlQuery query;
    QString strQuery;

    strQuery = "INSERT INTO sendcache (time, dstuser, content, msgid) VALUES ('"
                + time + "', '"
                + dstuser +"', '"
                + content +"', '"
                + QString::number(msgid) + "')";
    qDebug() << strQuery;
    query.exec(strQuery);
}

void Database::removeMsgFromSendcache(int msgid)
{
    QSqlQuery query;
    QString strQuery;

    strQuery = "DELETE FROM sendcache WHERE msgid = "
                + QString::number(msgid) + "')";
    qDebug() << strQuery;
    query.exec(strQuery);
}

void Database::clearSendcache()
{
    QSqlQuery query;
    QString strQuery;

    strQuery = "DELETE FROM sendcache";
    qDebug() << strQuery;
    query.exec(strQuery);
}

//拆分字符串函数
//输入参数:信息字符串QString msg
//因为可能拆分为多个字符串,所以返回值是QStringList,其中的每项长度都会小于1024
//TODO 消除幻数
QStringList Database::cutString(QString msg)
{
    QStringList DiscreteMsg;
    QByteArray MsgArray = msg.toLocal8Bit();
    int RemainLength = MsgArray.length();
    while (RemainLength > 1024)
    {
        int i = 0;
        for (; i < 3 ; i++)
        {
            if ((MsgArray.at(1021 + i) & 0x80)
                && (MsgArray.at(1021 + i) & 0x40)
                && (MsgArray.at(1021 + i) & 0x20))
            {
                // cut the fist 0 ~ 1021+i-1 byte 赋值给QStringList
                DiscreteMsg << MsgArray.mid(0, 1021 + i);
                // 在QByteArray中把前1021+i字节切掉
                MsgArray = MsgArray.mid(1021+i, MsgArray.length() - (1021 + i));
                RemainLength -= (1021 + i);
                break;
            }
        }
        //如果在三个字节内都没找到汉字的标志,则保留前1024个字节
        if (i == 3)
        {
            DiscreteMsg << MsgArray.mid(0, 1024);
            MsgArray = MsgArray.mid(1024, MsgArray.length() - 1024);
            RemainLength -= 1024;
        }
    }
    DiscreteMsg << MsgArray;
    return DiscreteMsg;
}
