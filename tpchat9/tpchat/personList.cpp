#include "personList.h"
#include <QAction>
#include <QIcon>

#include <iostream>
using namespace std;

personList::personList(QListWidget *parent) :
QListWidget(parent)
{
    setFocusPolicy(Qt::NoFocus);  // 去除item选中时的虚线边框
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);//水平滚动条关闭
    initMenu();
    this->onLineGroup = slotAddGroup("onLine");   // 增加在线联系人组
    this->offLineGroup = slotAddGroup("offLine"); // 增加离线联系人组
}
//初始化菜单
void personList::initMenu()
{
    //初始化：
    blankMenu = new QMenu();
    groupMenu = new QMenu();
    personMenu = new QMenu();
    groupNameEdit = new QLineEdit();
    QAction *addGroup = new QAction("添加分组", this);
    QAction *delGroup = new QAction("删除该组", this);
    QAction *rename   = new QAction("重命名组", this);
    QAction *addBuddy = new QAction("添加好友", this);
    QAction *delBuddy = new QAction("删除好友", this);
    //设置：
    groupNameEdit->setParent(this); //设置父类
    groupNameEdit->hide(); //设置初始时隐藏
    groupNameEdit->setPlaceholderText("未命名");//设置初始时的内容
    //布局：
    blankMenu->addAction(addGroup);
    groupMenu->addAction(delGroup);
    groupMenu->addAction(rename);
    groupMenu->addAction(addBuddy);
    personMenu->addAction(delBuddy);
    //信息槽：
    connect(groupNameEdit,SIGNAL(editingFinished()),this,SLOT(slotRenameEditFshed()));
    connect(addGroup,SIGNAL(triggered()),this,SLOT(slotAddGroup()));
    connect(delGroup,SIGNAL(triggered()),this,SLOT(slotDelGroup()));
    connect(rename,SIGNAL(triggered()),this,SLOT(slotRename()));
    connect(addBuddy,SIGNAL(triggered()),this,SLOT(slotAddBuddy()));
    connect(delBuddy,SIGNAL(triggered()),this,SLOT(slotDelBuddy()));

	// 双击某个联系人事件
    connect(this, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(itemDoubleClicked(QListWidgetItem*)));
    //connect(this, SIGNAL(doubleClicked(QModelIndex)),this,SLOT(itemDoubleClicked(QModelIndex)));
}
//鼠标点击事件
void personList::mousePressEvent(QMouseEvent *event)
{
    QListWidget::mousePressEvent(event); // 如果不调用基类mousePressEvent，item被select会半天不响应,调用父类，让QSS起效，因为QSS基于父类QListWidget，子类就是子窗口，就是最上层窗口，是覆盖在父窗口上的，所以先于父窗口捕获消息

    //防止一种特殊情况：给新item命名、点击其他item或空白处时，指向新item的currentItem被赋予其他item
    if(groupNameEdit->isVisible() && !(groupNameEdit->rect().contains(event->pos())))
    {
        if(groupNameEdit->text()!=NULL)
            currentItem->setText(groupNameEdit->text());
        groupNameEdit->setText("");
        groupNameEdit->hide();
    }
    currentItem = this->itemAt(mapFromGlobal(QCursor::pos()));//鼠标位置的Item，不管右键左键都获取

    if(event->button()==Qt::LeftButton && currentItem!=NULL && currentItem==groupMap.value(currentItem))//如果点击的左键并且是点击的是组
    {

        if(isHideMap.value(currentItem))  //如果先前是隐藏，则显示
        {
            foreach(QListWidgetItem* subItem, groupMap.keys(currentItem))//遍历组的对应的项（包括自身和好友）
                if(subItem!=currentItem)  //如果是组的话不进行处理
                {
                    subItem->setHidden(false);  //好友全部显示
                }
            isHideMap.insert(currentItem,false);  //设置该组为显示状态
            currentItem->setIcon(QIcon("png/arrowDown.png"));
        }
        else //否则，先前是显示，则隐藏
        {
            foreach(QListWidgetItem* subItem, groupMap.keys(currentItem))//遍历组的对应的项（包括自身和好友）
                if(subItem!=currentItem)  //如果是组的话不进行处理
                {
                    subItem->setHidden(true);  //好友全部隐藏
                }
            isHideMap.insert(currentItem,true);  //设置该组为隐藏状态
            currentItem->setIcon(QIcon("png/arrowRight.png"));
        }
    }
}
//菜单事件，为了显示菜单，点击鼠标右键响应，鼠标点击事件mousePressEvent优先于contextMenuEvent
void personList::contextMenuEvent(QContextMenuEvent *event)
{
    QListWidget::contextMenuEvent(event);  //调用基类事件
    if(currentItem==NULL)  //如果点击到的是空白处
    {
        blankMenu->exec(QCursor::pos());
        return;
    }
    if(currentItem==groupMap.value(currentItem))  // 如果点击到的是组
        groupMenu->exec(QCursor::pos());
    else //否则点击到的是好友
        personMenu->exec(QCursor::pos());
}
//添加组
QListWidgetItem * personList::slotAddGroup(QString groupName)
{
    QListWidgetItem *newItem=new QListWidgetItem(QIcon("png/arrowDown.png"), groupName);  //创建一个Item
    newItem->setSizeHint(QSize(this->width(),25));//设置宽度、高度
    this->addItem(newItem);  //加到QListWidget中
    groupMap.insert(newItem,newItem);//加到容器groupMap里，key和value都为组
    isHideMap.insert(newItem,false); //设置该组显示状态
    // groupNameEdit->raise();
    // groupNameEdit->setText(tr("未命名")); //设置默认内容
    // groupNameEdit->selectAll();  //设置全选
    // groupNameEdit->setGeometry(this->visualItemRect(newItem).left()+15,this->visualItemRect(newItem).top()+1,this->visualItemRect(newItem).width(),this->visualItemRect(newItem).height()-2);//出现的位置

    // groupNameEdit->show();  //显示
    // groupNameEdit->setFocus();  //获取焦点
    // currentItem = newItem;  // 因为要给group命名，所以当前的currentItem设为该group

    return newItem;
}
// void personList::slotAddGroup()
// {
    // slotAddGroup("untitled");
// }

//删除组
void personList::slotDelGroup()
{
    foreach(QListWidgetItem* item, groupMap.keys(currentItem)) //遍历该组的所有好友和自身的组
    {
        groupMap.remove(item);  //移除
        delete item;  //删除
    }
    isHideMap.remove(currentItem); //移除
}
//重命名
void personList::slotRename()
{
    groupNameEdit->raise();
    groupNameEdit->setGeometry(this->visualItemRect(currentItem).left()+15,this->visualItemRect(currentItem).top()+1,this->visualItemRect(currentItem).width(),this->visualItemRect(currentItem).height()-2);//出现的位置

    groupNameEdit->setText(currentItem->text());  //获取该组名内容
    groupNameEdit->show();  //显示
    groupNameEdit->selectAll();  //全选
    groupNameEdit->setFocus();  //获取焦点
}
//重命名完成
void personList::slotRenameEditFshed()
{
    if(groupNameEdit->text()!=NULL)  //如果重命名编辑框不为空
        currentItem->setText(groupNameEdit->text());  //更新组名
    groupNameEdit->setText("");
    groupNameEdit->hide(); //隐藏重命名编辑框
}

//添加好友，主要是为了测试功能，实际工程里可以改成动态读取数据库进行添加好友
void personList::slotAddBuddy()
{
    personListBuddy *buddy=new personListBuddy();  //创建一个自己定义的信息类
    //buddy->setFixedHeight(54);
    buddy->headPath="/head";  //设置头像路径
    buddy->name->setText("姓名");  //设置用户名
    buddy->sign->setText("最近一条聊天记录");  //设置个性签名
    QList<QListWidgetItem*> tem = groupMap.keys(currentItem);//当前组对应的项（包括组本身和好友）复制给tem
//关键代码
    QListWidgetItem *newItem = new QListWidgetItem();  //创建一个newItem
    newItem->setSizeHint(QSize(226,54));
    // this->insertItem(1,newItem);
    this->insertItem(row(currentItem)+tem.count(),newItem); //将该newItem插入到后面
    this->setItemWidget(newItem, buddy); //将buddy赋给该newItem
    groupMap.insert(newItem,currentItem);  //加进容器，key为好友，value为组
    if(isHideMap.value(currentItem))  //如果该组是隐藏，则加进去的好友设置为隐藏
        newItem->setHidden(true);
    else //否则，该好友设置为显示
        newItem->setHidden(false);

    personRowMap.insert(row(currentItem)+tem.count(), buddy);
    personItemMap.insert(newItem, buddy);
}
//删除好友
void personList::slotDelBuddy()
{
    groupMap.remove(currentItem); //移除该好友
	// 其他几个map也需要删除该联系人
    delete currentItem;  //删除
}

void personList::itemDoubleClicked(QListWidgetItem *item)
{
    qDebug("clicked");

    // double click on the group name
    currentItem = this->itemAt(mapFromGlobal(QCursor::pos()));//鼠标位置的Item，不管右键左键都获取
    if(currentItem==NULL || currentItem==groupMap.value(currentItem))//如果点击的左键并且是点击的是组
    {
        return;
    }

    personListBuddy *p = personItemMap.value(item);

    p->name->setText("ma xinbing");

    // open chat window
    if (p->chatWindow == NULL)
    {
        p->chatWindow = new ChatWindow(NULL);
        p->chatWindow->strName = p->name->text();
        p->chatWindow->name->setText(p->chatWindow->strName);
    }
    p->chatWindow->show();
    p->chatWindow->raise();
    p->chatWindow->activateWindow();
    p->chatWindow->inputTextEdit->setFocus();
}

// 不再使用这个
void personList::itemDoubleClicked(QModelIndex index)
{
    qDebug("clicked");

    cout<<index.row()<<endl;
    personListBuddy *p = personRowMap.value(index.row());
    p->name->setText("ma xinbing");

    // open chat window
    if (p->chatWindow == NULL)
    {
        p->chatWindow = new ChatWindow(NULL);
    }
    p->chatWindow->show();
}
