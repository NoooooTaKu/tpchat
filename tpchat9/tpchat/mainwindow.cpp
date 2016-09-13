#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
	// 窗口
    this->setGeometry(0, 0, 256, 512);
    this->setMaximumSize(256, 512);
    this->setWindowTitle("TP-Chat");
    move(300, 180);

	// 头像
    // NOTE: head portrait can be changed
    labelImage = new QLabel(this);
    labelImage->setGeometry(20, 10, 61, 61);
    QImage *image = new QImage("png61/13.png");
    labelImage->setPixmap(QPixmap::fromImage(*image));
    labelImage->show();

	// 登录用户的名字
    labelName = new QLabel(this);
    labelName->setGeometry(100, 10, 100, 27); // width 100 display 13 char
    // labelName->move(100, 10);
    /* not work
    QPalette palette;
    palette.setColor(QPalette::Text,Qt::red);
    labelName->setPalette(palette); */
    labelName->setFont(QFont( "Timers" , 12 ,  QFont::Bold) );

    tabWidget = new QTabWidget(this);
    tabWidget->setGeometry(15, 90, 226, 400);
// -----------------OnChat Tab------------------------- //
    tabOnChat = new QWidget(this);
    QGridLayout *tabOnChatLay = new QGridLayout();
    tabOnChat->setLayout(tabOnChatLay);
    tabWidget->addTab(tabOnChat, " OnChat ");

// ----------------Contacts Tab------------------------ //
    //tabContacts = new QWidget(this);
    //QGridLayout *tabContactsLay = new QGridLayout();
    //tabContacts->setLayout(tabContactsLay);
    //QListWidget *listWidget = new QListWidget(this);
    tabContacts = new personList(NULL);//listWidget);
    tabWidget->addTab(tabContacts, "Contacts");
    //tabWidget->addTab(tabContacts, "Contacts");

// ----------------Settings Tab------------------------ //
    tabSettings = new QWidget(this);
    btnAbout = new QPushButton(this);
    btnAbout->setText("About");
    btnQuit = new QPushButton(this);
    btnQuit->setText("Quit");
    btnAbout->setGeometry(20, 200, 171, 27);
    btnQuit->setGeometry(20, 260, 171, 27);
    btnHeadPortrait = new QPushButton(this);
    btnNoticeMsg = new QPushButton(this);
    btnClearLog = new QPushButton(this);
    btnHeadPortrait->setText("Head portrait");
    btnNoticeMsg->setText("Notice message");
    btnClearLog->setText("Clear log");

    QFormLayout *tabSettingsLay = new QFormLayout();
    tabSettingsLay->addWidget(btnHeadPortrait);
    tabSettingsLay->addWidget(btnNoticeMsg);
    tabSettingsLay->addWidget(btnClearLog);
    tabSettingsLay->addWidget(btnAbout);
    tabSettingsLay->addWidget(btnQuit);

    tabSettings->setLayout(tabSettingsLay);
    tabWidget->addTab(tabSettings, " Settings");

    connect(btnAbout, SIGNAL(clicked()), this, SLOT(btnAboutClicked()));
    connect(btnQuit, SIGNAL(clicked()), this, SLOT(btnQuitClicked()));

// ----------------show login window------------------------ //
    loginWindow = new Login(this);
	// 测试用点击登录按钮直接进入主窗口
    //connect(loginWindow, SIGNAL(showMainWindow(QString)), this, SLOT(showMainWindow(QString)));
    loginWindow->show();
    loginTimer = new QTimer(this);
    connect(loginTimer,SIGNAL(timeout()),this,SLOT(loginTimerOut()));
    loginTimerOutFlag = 0;
	// 现在是点击登录按钮，调用MainWindow的处理函数
    connect(loginWindow, SIGNAL(handleLoginApply(QString)), this, SLOT(handleLoginApply(QString)));
}

MainWindow::~MainWindow()
{
    qDebug("~MainWindow");
}

// 登录申请等待500ms可以直接进入主窗口
void MainWindow::loginTimerOut()
{
    loginTimerOutFlag = 1;
    loginTimer->stop();
    showMainWindow();
}
// 处理登录申请，两条路径进入显示主界面函数
// 路径一：计时器等待500ms
// 路径二：局域网回复错误码可登陆
void MainWindow::handleLoginApply(QString str)
{
	// 启动路径一
    qDebug("loginTimer start");
    loginTimer->start(500);
	
	// 提取登录用户名（待增加头像编号）
    myName = str;
    labelName->setText(myName);
	
	// 启动路径二
    // send online message
    udpSocket.bind(8888);
    connect(&udpSocket, SIGNAL(readyRead()), this, SLOT(udpReceive()));
    udpSendOnlineMsg();
}
void MainWindow::showMainWindow()
{
    qDebug("loginTimer stop");
    if(!loginTimer->isActive())
    {
		// 可以登录
        // online check
        if(1)
        {
            delete loginWindow;
            this->show();
        }
		// 局域网有重名，不可以登录
        else
        {
            emit reLogin();
            //loginWindow = new Login(NULL);
            //connect(loginWindow, SIGNAL(handleLoginApply(QString)), this, SLOT(handleLoginApply(QString)));
            //loginWindow->show();
        }
    }
}

void MainWindow::btnAboutClicked()
{
    QMessageBox::about(this, "About TP-Chat", "About this application");
}

void MainWindow::btnQuitClicked()
{
    QMessageBox::StandardButton rb = QMessageBox::question(this, "Quit TP-Chat?", "Are you sure to quit?", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
    if(rb == QMessageBox::Yes)
    {
        close();
    }
}
void MainWindow::udpSend(QString str)
{
    QByteArray datagram;
    QDataStream out(&datagram,QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_3);
    out<<str;
    udpSocket.writeDatagram(datagram,QHostAddress::LocalHost,8888);
}
void MainWindow::udpReceive()
{
    QByteArray datagram;
    do {
        datagram.resize(udpSocket.pendingDatagramSize());
        udpSocket.readDatagram(datagram.data(),datagram.size());
    }while(udpSocket.hasPendingDatagrams());
    QString str;
    QDataStream in(&datagram, QIODevice::ReadOnly);
    in.setVersion(QDataStream::Qt_4_3);
    in>>str;
    qDebug("udpReceive");
    qDebug(QByteArray(str.toLatin1()).data());
}
void MainWindow::udpSendOnlineMsg()
{
    QString a("{type:hello tpc; name:");
    QString str(a+myName+"}");
    udpSend(str);
}
