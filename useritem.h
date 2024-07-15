#ifndef USERITEM_H
#define USERITEM_H

#include <QWidget>

namespace Ui {
class UserItem;
}

class UserItem : public QWidget
{
    Q_OBJECT

public:
    explicit UserItem(QWidget *parent = 0);
    ~UserItem();
    void setFriendInfo(int id,int iconId,int statue,QString name,QString feeling, bool bTcp = false);
    void setIconBlack();
    int showId();
    QString name();
private:
    Ui::UserItem *ui;
    int m_friendId;
    QString m_name;
    int m_iconId;
    QString m_feeling;
    int m_statue;
    bool m_bTcp;

signals:
    void SIG_ShowChatIp(QString);
    void SIG_ShowTcpChatIp(int);
private slots:
    void on_pb_icon_clicked();
};

#endif // USERITEM_H
