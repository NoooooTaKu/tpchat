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
    //与数据库建立连接，在构造函数里引用了，实例化的时候自动连接所以不需要手动连接
    bool connectDatabase();

    //创建数据库，预备文件损坏或被删除的时候用，现在还没用到
    void createDb();

    //绑定数据，用不上，请忽略
    void bindData();

    //未完成，现在暂时不用
    //当有一个新的联系人记录时用到，输入参数是当前用户名,创建两张表，一张名为[user]log，一张名为[user]partner
    void createTable(QString user);

    //增加一条聊天记录，收到消息时或按下send按钮发送信息时引用
    //输入参数：用户名、时间、聊天对象用户名、文字内容、
    //NOTICE： 输入参数 int send 表示消息是谁发出的，1表示消息由自己发出，0表示消息由对方发出，在查看历史记录时用到
    void addLog(QString user, QString time,
                QString partner, QString content, int send);

    //查看历史记录，输入参数为用户名、聊天对象用户名
    //返回QStringList可以直接逐条输出，已经排好格式了，现在在./chatwindow.cpp写了一个简单的实现，按下聊天记录按钮可以将聊天记录输出在屏幕上
    QStringList viewLogByPartner(QString user, QString partner);

    //删除与某个联系人的聊天记录
    void deleteLogByPartner(QString user, QString partner);

    //删除某个用户的全部聊天记录
    void deleteLogByUser(QString user);

    //关闭数据库
	//可以放在MainWindow的析构函数中，在退出程序或logout时自动引用
    void closeDb();

    // 检查联系人是否已经存在，若已经存在，返回false（501），若不存在返回true（100）
	// ****************注意返回值，不要用错****************
    bool checkPartnerName(QString user, QString newuser);

    // 向联系人表中添加一条新联系人记录
	// 该函数是不会自动检查是否已经存在联系人的，一定要先手动引用checkPartnerName
    void addPartnerName(QString user, QString newuser, int image, QString ip);

    // 暂时不用
	//init two tables ***log and ***partner, check if exists, if not, creat them
    void initTable(QString user);

    // 向登录记录中增加一个用户名
	// 该函数是不会自动检查是否已经存在用户名的，在登录时，一定要手动引用checkUserName
    void addUserName(QString user, int image);

    // 在登录记录中检查用户名是否已经存在，若已经存在，返回false，若不存在返回true
	// 使用场合是在登录界面中，可以根据当前输入的结果自动补全头像（扩展功能）
	// ****************注意返回值，不要用错****************
    bool checkUserName(QString user);
};

#endif // DATABASE_H
