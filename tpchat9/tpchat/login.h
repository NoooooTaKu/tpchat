#ifndef LOGIN_H
#define LOGIN_H

#include <QMainWindow>
#include <QtWidgets>

class MainWindow;

class Login : public QMainWindow
{
    Q_OBJECT
public:
    explicit Login(QWidget *parent = 0);

signals:
    // 测试用，btnLogin按钮直接登录主界面
    // void showMainWindow(QString);
	
	// 现方案，MainWindow对登录申请进行处理
	// 传递给MainWindow用户名（无默认值）
	// 后续增加传递头像编号，可以有默认值
    void handleLoginApply(QString);

public slots:

private:
    QLabel *label;         // 头像
    QLineEdit *lineEdit;   // 用户名输入
    QPushButton *btnLogin; // 登录按钮
    QPushButton *btnQuit;  // 退出按钮

    QLabel *waiting;       // 登录等待动画框
    QMovie *movie;         // 登录等待动画

	// 数字和字符合法性检查
    int charDigitCheck(QString);

private slots:
    void btnLoginClicked();
    void btnQuitClicked();
	// MainWindow处理登录申请发现局域网该用户已登录
	// 回调Login槽函数激活登陆界面重新填写用户名
    void reLogin();
    // search database and load head image
    void loadHeadImage(QString);
};

#endif // LOGIN_H
