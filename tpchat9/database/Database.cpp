#include <QtSql/QSqlDatabase>
#include <QDebug>
#include <QSqlQuery>
#include <QDir>
#include <QTableView>
#include <QSqlTableModel>
#include "database.h"

#define SENDFROMME  1
#define SENDTOME    0

Database::Database()
{
    // check if the db file exists
    bool dbFile = QFile::exists("/home/whf/db/chatdb");

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
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("/home/whf/db/chatdb");
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
    //query.exec("create table " + user);
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
