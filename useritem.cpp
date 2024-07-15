#include "useritem.h"
#include "ui_useritem.h"
#include<QBitmap>
#include"def.h"
UserItem::UserItem(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UserItem)
{
    ui->setupUi(this);
}

UserItem::~UserItem()
{
    delete ui;
}

void UserItem::setFriendInfo(int id, int iconId, int statue, QString name, QString feeling,bool bTcp)
{
    //保存好友信息
    m_bTcp = bTcp;
    m_friendId = id;
    m_iconId = iconId;
    m_statue = statue;
    m_feeling = feeling;
    m_name = name;
    //把信息设置到控件上
    ui->lb_name->setText(m_name);
    ui->lb_feeling->setText(m_feeling);
    QString path =QString(":/resource/素材/tx/%1.png").arg(m_iconId);
    if(m_statue == status_online)
    {
        ui->pb_icon->setIcon(QIcon(path));
    }
    else
    {
        QBitmap bmp;
        bmp.load(path);
        ui->pb_icon->setIcon(bmp);
    }
    repaint();
}

void UserItem::setIconBlack()
{
    QString path =QString(":/resource/素材/tx/%1.png").arg(m_iconId);

    QBitmap bmp;
    bmp.load(path);
    ui->pb_icon->setIcon(bmp);
    repaint();
}

int UserItem::showId()
{
    return m_friendId;
}

QString UserItem::name()
{
    return m_name;
}

void UserItem::on_pb_icon_clicked()
{
    if(!m_bTcp)
    {
        Q_EMIT SIG_ShowChatIp(m_feeling);
    }
    else
    {
        Q_EMIT SIG_ShowTcpChatIp(m_friendId);
    }
}
