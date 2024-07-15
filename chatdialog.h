#ifndef CHATDIALOG_H
#define CHATDIALOG_H

#include <QWidget>
//#include"sendfile.h"
#include"filerecvdialog.h"
namespace Ui {
class ChatDialog;
}

class ChatDialog : public QWidget
{
    Q_OBJECT

public:
    explicit ChatDialog(QWidget *parent = 0);
    ~ChatDialog();
    //设置窗口属性
    void SetChatWindow(QString ip,int id = 0);

    //设置到聊天窗口上
    void SetChatContent(QString content);
    //设置群聊id
    void SetGroupId(int id);
    //设置群聊的内容的函数
    void SetTcpGroupChatContent(QString content,int friendIp);
    //设置发送文件的滑动条
    void SetSendFileBar(int value);
    //发送文件
    //SendFile* ShowSendFile();
    //检查是否已经存在文件的对应窗口
    bool haveFileWidget(QString fileName);
    //添加接收文件的窗口
    void addRecvItem(QString fileName, long long size);
    void setRecvProcessBar(QString fileName, int value);
private slots:
    void on_pb_send_clicked();
    void on_pb_tool1_clicked();

    void slot_setSendFileProcess(int id, int value);

    void on_pb_checkFile_clicked();

    void slot_resend(QString fileName);
signals:
    void SIG_SendContent(QString content,QString ip);
    void SIG_SendTcpContent(QString content,int id);
    void SIG_SendTcpGroupContent(QString content,int id);
    void SIG_SendFile(QString path, int id);
    void SIG_resend(QString fileName, int id);
    void SIG_fileCheck(int id);
private:
    Ui::ChatDialog *ui;
    //SendFile* m_pSendFile;
    FileRecvDialog* m_fileRecv;
    QString m_ip;
    int m_id;
    int groupid;
};

#endif // CHATDIALOG_H
