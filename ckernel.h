#ifndef CKERNEL_H
#define CKERNEL_H
#include"chatdialog.h"
#include <QObject>
#include<QThread>
#include"dialog.h"
#include"MidUdpNet.h"
#include"MidTcpNetClient.h"
#include"useritem.h"
#include<QMap>
#include<QTextCodec>
#include"logindialog.h"
//#include<sendfile.h>
#include"groupitem.h"
#include"sendfilethread.h"
#include"./mycrypt/myaes.h"
#include"./mycrypt/myrsa.h"
class SendFileThread;
class Ckernel;
typedef void(Ckernel::*PFun)(long , char* , int);
class Ckernel : public QObject
{
    Q_OBJECT
public:
    explicit Ckernel(QObject *parent = 0);
    ~Ckernel();

    //编码转换
    void Utf8ToGb2312(QString src,char* dest, int nLen);
    QString Gb2312ToUtf8(char* src);

    void setMap();
    void addFriend(UserItem);
    //void continueSend(_STRU_TCP_SENDFILE_RS rs);
    void sendFile(QString path, int id, bool *quit);
    void setSildrBar(int precent);

    void dealOnlineRq(long ISend, char* sendBuf, int nLen);
    void dealOnlineRs(long ISend, char* sendBuf, int nLen);
    void dealCharRq(long ISend, char* sendBuf, int nLen);
    void dealOfflineRq(long ISend, char* sendBuf, int nLen);
    void dealRegisterRs(long ISend, char* sendBuf, int nLen);
    void dealLoginRs(long ISend, char* sendBuf, int nLen);
    void dealFirendInfo(long ISend, char* sendBuf, int nLen);
    void dealTcpChatRq(long ISend, char* sendBuf, int nLen);
    void dealTcpChatRs(long ISend, char* sendBuf, int nLen);
    void dealTcpAddFriendRq(long ISend, char* sendBuf, int nLen);
    void dealTcpAddFriendRs(long ISend, char* sendBuf, int nLen);
    void dealTcpOfflineRq(long ISend, char* sendBuf, int nLen);
    //void dealTCPSendFileRq(long ISend, char* sendBuf, int nLen);
    //void dealTcpSendFileRs(long ISend, char* sendBuf, int nLen);
    void dealTcpGroupInfoRq(long ISend, char* sendBuf, int nLen);
    void dealTcpGroupContentRq(long ISend, char* sendBuf, int nLen);
    void dealTcpGroupContentRs(long ISend, char* sendBuf, int nLen);
    //void dealTcpSendOnOff(long ISend, char* sendBuf, int nLen);

    void dealTcpFileInfo(long ISend, char* sendBuf, int nLen);
    void dealTcpFileBlockRq(long ISend, char* sendBuf, int nLen);
    void dealTcpFileBlockRs(long ISend, char* sendBuf, int nLen);
    void dealTcpFileCheckRs(long ISend, char* sendBuf, int nLen);
public slots:
    void slot_ReadyDate(long ISend, char* sendBuf, int nLen);
    //处理注册信息
    void slots_registerSubmit(QString name,QString tel,QString password);
    //处理登录
    void slots_logic(QString tel,QString password);
    //显示聊天窗口
    void slot_ShowChatId(QString);
    void slot_ShowTcpChatIp(int);
    //处理聊天内容
    void slot_SendContent(QString content,QString ip);
    void slot_SendTcpContent(QString content,int id);
    void slot_closeApp();
    void slot_closeLog();
    //添加好友
    void slot_addFriend();
    //发送文件
    void slot_sendFile(QString path, int id);
    //打开群聊窗口
    void slot_show_group_chat(int id);
    //发送群聊信息
    void slot_SendTcpGroupContent(QString content,int id);
    //发送问价重传请求
    void slot_ResendFile(QString fileName, int id);
    //查看文件
    void slot_checkFile(int id);
signals:
    void SIG_setSendFileProcess(int id,int value); //按照0到100设置发送文件的进度
private:

    int m_id;    
    QString m_name;
    SendFileThread* m_sendFileThread;
    Dialog* m_p;
    LoginDialog* m_pLogDia;
    MidINet* m_pTcpMid;
    MidINet* m_pUdpMid;
    PFun m_pFun[_FUN_NUMBER_MAX];
    QMap<long,ChatDialog*> m_pIpChat;
    QMap<int , UserItem*> m_mapIdUserItem;
    QMap<int,ChatDialog*> m_mapIdChat;
    QMap<int,ChatDialog*> m_mapidGroupChat;
    QMap<int,GroupItem*> m_mapIdGroupItem;
    QMap<QString, Filelfo> m_mapFileIdToFileInfoRecv;
    QMap<QString, STRU_FILE_INFO> m_mapFileIdToFileInfoSend;
    char buf[1024];
    bool canChangName;
};

#endif // CKERNEL_H
