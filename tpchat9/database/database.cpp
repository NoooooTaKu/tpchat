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

    strQuery = "select * from " + user;
    query.exec(strQuery);

    if (!query.next())
    {
        strQuery = "create table " + user + "log";// like templatelog";
        query.exec(strQuery);
        qDebug() << strQuery;
        strQuery = "create table " + user + "partner";// like templatepartner";
        query.exec(strQuery);
        qDebug() << strQuery;
    }
}

/*
 * Function:
 */
void Database::createTable(QString user)
{

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
    strQuery = "insert into " + user
                + "log (send, time, person, content) values ("
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
    strQuery = "delete from " + user + "log where person = " + partner + ")";
    qDebug() << strQuery;
    strQuery = "delete from " + user + "partner where person = "
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
    strQuery = "drop table" + user + "log, " + user + "partner";
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

    strQuery = "select * from " + user + "log where person = '"
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
        qDebug() << "database closed";
        db.close();
    }
}

bool Database::checkPartnerName(QString user, QString newuser)
{
    //return true;
    QSqlQuery query;
    QString strQuery;

    strQuery = "select * from " + user + "partner where person = '"
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
    strQuery = "insert into " + user + "partner (person, image, ip) values ('"
                + newuser + "', '" + QString::number(image) + "', '" + ip +"')";
    qDebug() << strQuery;
    query.exec(strQuery);
}

void Database::addUserName(QString user, int image)
{
    QSqlQuery query;
    QString strQuery;
    strQuery = "insert into loginhistory (user, image) values ('"
                + user + "', '" + QString::number(image) + "')";
    qDebug() << strQuery;
    query.exec(strQuery);
}

bool Database::checkUserName(QString user)
{
    QSqlQuery query;
    QString strQuery;

    strQuery = "select * from loginhistory where user = '" + user + "'";
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
