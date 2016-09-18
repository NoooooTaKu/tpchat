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

    // 初始化数据库,当用户登录成功时,检查数据库内是否已经存在聊天记录,若不存在,则创建新表
    // init two tables ***log and ***partner, check if exists, if not, creat them
    void initTable(QString user);

    // 向登录记录中增加一个用户名
	// 该函数是不会自动检查是否已经存在用户名的，在登录时，一定要手动引用checkUserName
    void addUserName(QString user, int image);

    // 在登录记录中检查用户名是否已经存在，若已经存在，返回false，若不存在返回true
	// 使用场合是在登录界面中，可以根据当前输入的结果自动补全头像（扩展功能）
	// ****************注意返回值，不要用错****************
    bool checkUserName(QString user);

    // 向数据库的sendcache表中添加一条消息记录
    // 使用场合,在聊天窗口按下发送按钮之后
    void addMsgToSendcache(QString time, QString dstuser,
                           QString content, int msgid);
    // 从数据库的sendcache表中移除一条消息记录
    // 使用场合:收到dstuser返回的接受成功信号后执行
    // 作用:msgid没有存在聊天记录的必要,仅作为发送信息的标志,应该及时清理缓存,防止msgid太多,不好分配
    void removeMsgFromSendcache(int msgid);

    // 清除发送消息缓存
    // 使用场合:退出程序或登出时
    void clearSendcache();

    //拆分字符串函数
    //拆分字符串函数
    //输入参数:信息字符串QString msg
    //因为可能拆分为多个字符串,所以返回值是QStringList,其中的每项长度都会小于1024
    QStringList cutString(QString msg);
};

#endif // DATABASE_H
