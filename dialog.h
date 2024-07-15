#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include<QVBoxLayout>
#include"useritem.h"
#include<QCloseEvent>
#include<QMap>
#include<QMenu>
#include<groupitem.h>
namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();
    //TCP添加好友
    void addTcpFriend(UserItem* item);
    //添加群聊
    void addGroup(GroupItem* item);
    //设置自己的信息
    void setUserInfo(QString name, QString feeling, int icon);
    //添加好友
    void addFriend(long ,UserItem*);
    //删除好友
    void deleteFriend(long ip);
    void closeEvent(QCloseEvent *event) override;
private:
    Ui::Dialog *ui;
    //定义垂直布局的
    QVBoxLayout* m_pLayOut;
    QVBoxLayout* m_pLayOut2;
    QVBoxLayout* m_pLayOut3;

    //定义表储存
    QMap<long,UserItem*> m_pIpUerItem;
    //定义菜单栏的指针
    QMenu* m_menu;
signals:
    void SIG_closeApp();
    //添加好有信号
    void SIG_addFriend();
private slots:
    void on_pb_menu_clicked();

    void slot_clickedMenu(QAction* action);
};

#endif // DIALOG_H
