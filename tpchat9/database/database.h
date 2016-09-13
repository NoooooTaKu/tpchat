#ifndef DATABASE_H
#define DATABASE_H

#include <QtSql/QSqlDatabase>

//class QSqlDatabase;

class Database
{
public:
    explicit Database();
    ~Database();

private:
    QSqlDatabase db;

signals:

public slots:
    bool connectDatabase();
    void createDb();
    void bindData();
    void createTable(QString user);
    void addLog(QString user, QString time,
                QString partner, QString content, int send);
    QStringList viewLogByPartner(QString user, QString partner);
    void deleteLogByPartner(QString user, QString partner);
    void deleteLogByUser(QString user);
    void closeDb();

};

#endif // DATABASE_H
