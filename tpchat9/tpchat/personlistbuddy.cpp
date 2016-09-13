#include "personlistbuddy.h"

personListBuddy::personListBuddy(QWidget *parent) :
    QWidget(parent)
{
    initUi();
    chatWindow = NULL;
}

void personListBuddy::initUi()
{
    // 头像
    head = new QLabel(this);
    head->setFixedSize(40,40);
	head->move(7,7);
    //head->setGeometry(50, 40, 156, 156);
    QImage *image = new QImage("png40/13.png");
    head->setPixmap(QPixmap::fromImage(*image));
    head->show();

	// 姓名
	name = new QLabel(this);
    name->setFixedSize(100,27);
    name->move(54,3);
	
    // 个性签名
	sign = new QLabel(this);
	sign->move(54,27);
    QPalette color;
    color.setColor(QPalette::Text,Qt::gray);
    sign->setPalette(color);
}
