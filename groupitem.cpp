#include "groupitem.h"
#include "ui_groupitem.h"

GroupItem::GroupItem(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GroupItem)
{
    ui->setupUi(this);
}

GroupItem::~GroupItem()
{
    delete ui;
}

void GroupItem::setGroupInfo(int id,int icon,QString name,QString feeling)
{
    ui->lb_name->setText(name);
    ui->lb_feeling->setText(feeling);
    m_groupId = id;
    QString path =QString(":/resource/素材/tx/%1.png").arg(icon);
    ui->pb_icon->setIcon(QIcon(path));
}

void GroupItem::on_pb_icon_clicked()
{
    Q_EMIT SIG_show_group_chat(m_groupId);
}
