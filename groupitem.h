#ifndef GROUPITEM_H
#define GROUPITEM_H

#include <QWidget>
#include<QString>
namespace Ui {
class GroupItem;
}

class GroupItem : public QWidget
{
    Q_OBJECT

public:
    explicit GroupItem(QWidget *parent = 0);
    ~GroupItem();
    void setGroupInfo(int id,int icon,QString name,QString feeling);
private slots:
    void on_pb_icon_clicked();

private:
    Ui::GroupItem *ui;
    int m_groupId;
    QString m_feeling;
    QString m_name;
    int icon;    
signals:
    void SIG_show_group_chat(int id);
};

#endif // GROUPITEM_H
