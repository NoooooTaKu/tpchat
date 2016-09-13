#ifndef PERSONLIST_H
#define PERSONLIST_H

#include <QListWidget>
#include "personlistbuddy.h"
#include "chatwindow.h"

class personList : public QListWidget
{
    Q_OBJECT
public:
    explicit personList(QListWidget *parent = 0);
    void mousePressEvent(QMouseEvent *event);  // 鼠标点击事件
    void contextMenuEvent(QContextMenuEvent*); // 菜单事件，为了显示菜单
    void initMenu();  // 初始化菜单
	
    QMenu *blankMenu; // 点击空白上的菜单
    QMenu *groupMenu; // 点击组上的菜单
    QMenu *personMenu;// 点击人上的菜单
    QMap<QListWidgetItem*, QListWidgetItem*> groupMap;  // 组容器 key:项 value:组
    QMap<QListWidgetItem*, bool> isHideMap; //用来判断该组是否隐藏了
    QLineEdit *groupNameEdit;               //组的名字，重命名的时候需要用到
    QListWidgetItem *currentItem;           //当前的项

    QListWidgetItem *onLineGroup;  // 在线组联系人
    QListWidgetItem *offLineGroup; // 离线组联系人
signals:

public slots:
    QListWidgetItem *  slotAddGroup(QString groupName = "untitled");  //添加组
    // void slotAddGroup();  // 添加组
    void slotDelGroup();  // 删除组
    void slotAddBuddy();  // 添加好友
    void slotDelBuddy();  // 删除好友
    void slotRename();    // 重命名组
    void slotRenameEditFshed();//命名完成

	// 双击联系人
    void itemDoubleClicked(QListWidgetItem * item);
    void itemDoubleClicked(QModelIndex); // not used

private:
    // not used
    QMap<int, personListBuddy *> personRowMap;
	// 双击对应表
    QMap<QListWidgetItem *, personListBuddy *> personItemMap;

};
#endif // PERSONLIST_H
