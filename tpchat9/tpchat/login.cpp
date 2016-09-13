#include "login.h"
#include <iostream>
using namespace std;

Login::Login(QWidget *parent) :
    QMainWindow(parent)
{
	// 窗口
    this->setGeometry(0, 0, 256, 512);
    this->setMaximumSize(256, 512);
    this->setWindowTitle("Login");
    move(300, 180);

	// 头像
    // NOTE: head portrait can be changed
    label = new QLabel(this);
    label->setGeometry(50, 40, 156, 156);
    QImage *image = new QImage("png156/13.png"); // to be changed
    label->setPixmap(QPixmap::fromImage(*image));
    label->show();

	// 用户名输入框
    lineEdit = new QLineEdit(this);
    lineEdit->setGeometry(40, 240, 176, 27);
    lineEdit->setPlaceholderText("Input your name");
    lineEdit->setFocus();

	// 登录按钮
    btnLogin = new QPushButton(this);
    btnLogin->setGeometry(40, 290, 176, 27);
    btnLogin->setText("Login");

	// 退出按钮
    btnQuit = new QPushButton(this);
    btnQuit->setGeometry(40, 320, 176, 27);
    btnQuit->setText("Quit");

    connect(btnLogin, SIGNAL(clicked()), this, SLOT(btnLoginClicked()));
    connect(btnQuit, SIGNAL(clicked()), this, SLOT(btnQuitClicked()));

    // not work
    // connect(lineEdit, SIGNAL(editingFinished()),this,SLOT(btnLoginClicked()));
    // connect(lineEdit,SIGNAL(returnPressed()),this,SLOT(btnLoginClicked()));

	// 等待登录的动画（暂时还比较丑）
    waiting = new QLabel(this);
    waiting->setGeometry(28, 350, 200, 160);
    movie = new QMovie("loading.gif");
    waiting->setMovie(movie);
    // movie->start();

	// MainWindow处理登录申请发现该用户已登录
	// 激活登录窗口重新输入用户名
    connect(parent, SIGNAL(reLogin()), this, SLOT(reLogin()));

    // search database and load head portrait
    connect(lineEdit, SIGNAL(textChanged(QString)), this, SLOT(loadHeadImage(QString)));
}

void Login::btnLoginClicked()
{
    // need many work here
    QString str = lineEdit->text();
    int checkResult = charDigitCheck(str);
    if (checkResult == 1)
    {
        QMessageBox::warning(this, "warning", "name can not be empty", QMessageBox::Yes, QMessageBox::Yes);
        lineEdit->setFocus();
    }
    else if (checkResult == 2)
    {
        QMessageBox::warning(this, "warning", "can not more than 31 char", QMessageBox::Yes, QMessageBox::Yes);
        lineEdit->setFocus();
    }
    else if (checkResult == 3)
    {
        QMessageBox::warning(this, "warning", "charDigitCheck fail", QMessageBox::Yes, QMessageBox::Yes);
        lineEdit->setFocus();
    }
    else
    {
		// 字符数字检查合法，交给MainWindow进行局域网登录广播
        lineEdit->setDisabled(true);
        btnLogin->setDisabled(true);
        btnQuit->setDisabled(true);
        movie->start();
        waiting->setVisible(true);
        emit handleLoginApply(str);
        //emit showMainWindow(str);
    }
}

void Login::btnQuitClicked()
{
    QMessageBox::StandardButton rb = QMessageBox::question(this, "Quit TP-Chat?", "Are you sure to quit?", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
    if(rb == QMessageBox::Yes)
    {
        close();
    }
}

// NOTE: need more code
int Login::charDigitCheck(QString str)
{
//    if(str.isEmpty())
//        return 0;
//    return 1;

    if(str.isEmpty())
    {
        return 1;
    }

    QByteArray ba = str.toLatin1();
    char *mm = ba.data();
    if (strlen(mm)>31)
    {
        return 2;
    }
    int i = 0;
    for (i = 0; i < strlen(mm); i++)
    {
        if( !( (mm[i]>='a' && mm[i]<='z') || (mm[i]>='A' && mm[i]<='Z') || mm[i]>='0' && mm[i]<='9' )  )
        {
            return 3;
        }
    }
    return 0;
}

void Login::reLogin()
{
    waiting->setVisible(false);
    movie->stop();
    QMessageBox::warning(this, "warning", "network has the name", QMessageBox::Yes, QMessageBox::Yes);
    lineEdit->setEnabled(true);
    btnLogin->setEnabled(true);
    btnQuit->setEnabled(true);
    lineEdit->setFocus();
}

// search database and load head image
void Login::loadHeadImage(QString str)
{
    qDebug("textChanged");
    qDebug(QByteArray(str.toLatin1()).data());

    QByteArray ba = str.toLatin1();
    char *mm = ba.data();
    //<<sizeof(mm);
    cout<<strlen(mm)<<endl;
}
