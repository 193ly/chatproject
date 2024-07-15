#include "dialog.h"
#include "ui_dialog.h"
#include<QMessageBox>
#include<QDebug>
Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    //new一个菜单栏的对象
    m_menu = new QMenu;
    m_menu->addAction("set system");
    m_menu->addAction("add friend");

    connect(m_menu,SIGNAL(triggered(QAction*)),this,SLOT(slot_clickedMenu(QAction*)));


    ui->setupUi(this);
    m_pLayOut = new QVBoxLayout;
    m_pLayOut2 = new QVBoxLayout;
    m_pLayOut3 = new QVBoxLayout;

    //设置margin
    m_pLayOut->setContentsMargins(3,0,3,0);
    m_pLayOut2->setContentsMargins(3,0,3,0);
    m_pLayOut3->setContentsMargins(3,0,3,0);

    //设置spacing
    m_pLayOut->setSpacing(3);
    ui->widget->setLayout(m_pLayOut);
    m_pLayOut2->setSpacing(3);
    ui->widget_2->setLayout(m_pLayOut2);
    m_pLayOut3->setSpacing(3);
    ui->widget_3->setLayout(m_pLayOut3);
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::addTcpFriend(UserItem *item)
{
    m_pLayOut2->addWidget(item);
}

void Dialog::addGroup(GroupItem *item)
{
    m_pLayOut3->addWidget(item);
}

void Dialog::setUserInfo(QString name, QString feeling, int icon)
{
    ui->lb_name->setText(name);
    ui->le_feeling->setText(feeling);
    ui->pb_picture->setIcon(QIcon(QString(":/resource/素材/tx/%1.png").arg(icon)));
}

void Dialog::addFriend(long ip,UserItem *item)
{
    m_pLayOut->addWidget(item);
    m_pIpUerItem[ip] = item;
}

void Dialog::deleteFriend(long ip)
{
    auto ite = m_pIpUerItem.find(ip);
    if(ite != m_pIpUerItem.end())
    {
        UserItem* item = *ite;
        if(item)
        {
            item->hide();
            m_pLayOut->removeWidget(item);
            delete item;
            item = nullptr;
        }
        m_pIpUerItem.erase(ite);
    }

}

void Dialog::closeEvent(QCloseEvent *event)
{
    //忽略关闭事件
    event->ignore();
    //弹出提示确认是否关闭
    if(QMessageBox::Yes == QMessageBox::question(this,"Yes","确认关闭？"))
    {
        // 通知kerenl，发送下线广播
        Q_EMIT SIG_closeApp();
    }
}

void Dialog::on_pb_menu_clicked()
{
    QPoint pos = QCursor::pos();

    QSize size = m_menu->sizeHint();
    m_menu->exec(QPoint(pos.x(),  pos.y() - size.height()));

}

void Dialog::slot_clickedMenu(QAction *action)
{
    qDebug() << __func__;
    if(action->text() == "set system")
    {
        qDebug() << "点击系统设置" ;
    }
    else if(action->text() == "add friend")
    {
        Q_EMIT SIG_addFriend();
    }
}
