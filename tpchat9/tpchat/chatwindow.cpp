#include "chatwindow.h"

ChatWindow::ChatWindow(QWidget *parent) :
    QMainWindow(parent)
{
    this->setGeometry(0, 0, 512, 512);
    this->setMaximumSize(512, 512);
    this->setWindowTitle("TP-Chat");
    move(556, 180);

    // NOTE: head portrait can be changed
    headImage = new QLabel(this);
    headImage->setGeometry(25, 5, 61, 61);
    QImage *image = new QImage("png61/13.png");
    headImage->setPixmap(QPixmap::fromImage(*image));
    headImage->show();

    name = new QLabel(this);
    name->setGeometry(100, 5, 100, 27);
    name->setText(strName);
    name->setFont(QFont( "Timers" , 12 ,  QFont::Bold) );

    textEdit = new QTextEdit(this);
    textEdit->setGeometry(25, 70, 462, 281);

    inputTextEdit = new QTextEdit(this);
    inputTextEdit->setGeometry(25, 400, 361, 81);
    inputTextEdit->setFocus();

    btnSend = new QPushButton(this);
    btnSend->setGeometry(400, 400, 81, 81);
    btnSend->setText("Send");

    connect(btnSend, SIGNAL(clicked()), this, SLOT(sendMsg()));
}

void ChatWindow::sendMsg()
{
    textEdit->append(inputTextEdit->toPlainText());
    inputTextEdit->setText("");
}
