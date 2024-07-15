#include "ckernel.h"
#include<QMessageBox>
#include<QDebug>
#include<QInputDialog>
#include<QCloseEvent>
#include<QDateTime>
#include<QFileInfo>

Ckernel::   Ckernel(QObject *parent) : QObject(parent),canChangName(true)
{
    //信号和槽机制：QT特有，实际用来发送数据的
    //信号时发送数据的类
    //槽函数是接受数据的类

    //继承QObject
    //有Q_OBJECT
    //在发送数据的类声明有signals，在要发送数据的位置有Q_EMIT 信号
    //接受数据的类里声明并实现槽函数slots
    //接受数据的类里，发送信号对象new出来后绑定信号和槽函数
    //connect函数的第一个参数是发送数据的对象

    m_p = new Dialog;
    connect(m_p,SIGNAL(SIG_closeApp()),this,SLOT(slot_closeApp()));
    connect(m_p,SIGNAL(SIG_addFriend()),this,SLOT(slot_addFriend()));
    //new一个登录
    m_pLogDia = new LoginDialog;
    connect(m_pLogDia,SIGNAL(SIG_registerSubmit(QString,QString,QString)),this,SLOT(slots_registerSubmit(QString , QString , QString )));
    connect(m_pLogDia,SIGNAL(SIG_logic(QString,QString)),this,SLOT(slots_logic(QString,QString)));
    connect(m_pLogDia,SIGNAL(SIG_closeLog()),this,SLOT(slot_closeLog()));
    m_pLogDia->show();

    //创建文件传输线程
    m_sendFileThread = new SendFileThread(this);

    //m_p->show();
    m_pTcpMid = new MidTcpNetClient;
    m_pUdpMid = new MidUdpNet;
    setMap();
    if(m_pTcpMid->OpenNet())
    {
        //成功
        //QMessageBox::about(m_p,"提示","TCP打开网络成功");

    }
    else
    {
        //失败
        QMessageBox::about(m_p,"提示","TCP打开网络错误");
        exit(1);

    }

//    if(m_pUdpMid->OpenNet())
//    {
//        //成功
//        //QMessageBox::about(m_p,"提示","UDP打开网络成功");

//    }
//    else
//    {
//        //失败
//        QMessageBox::about(m_p,"提示","UDP打开网络失败");
//        exit(1);
//    }

//    MyRSA rsa;

//    //生成AES秘钥
//    m_pTcpMid->aes = new MyAES();

//    string en = rsa.rsaPubEncrypt(string(m_pTcpMid->aes));
//    m_pTcpMid->SendData(0, en.c_str(), en.size());


    connect(m_pTcpMid,SIGNAL(SIG_ReadyDate(long,char*,int)),this,SLOT(slot_ReadyDate(long,char*,int)));
    connect(m_pUdpMid,SIGNAL(SIG_ReadyDate(long,char*,int)),this,SLOT(slot_ReadyDate(long,char*,int)));

    //m_pTcpMid->SendData(000,"hellow",sizeof("hellow"));

    //上线先发送广播，告诉大家我来了
    _STRU_UDP_ONLINE_RQ rq;
    gethostname(rq.name,sizeof(rq));
    strcpy(rq.name,"我是朋友");
    m_pUdpMid->SendData(/*INADDR_BROADCAST*/inet_addr(SEVER_IP),(char*)&rq,sizeof(rq));
}
Ckernel::~Ckernel()
{
    qDebug() << __func__;
    slot_closeLog();
}

void Ckernel::Utf8ToGb2312(QString src, char *dest, int nLen)
{
    QTextCodec* pCodec = QTextCodec::codecForName("gb2312");
    QByteArray ba = pCodec->fromUnicode(src);
    if(ba.length() <= nLen)
    {
        memcpy(dest,ba.data(),ba.length());
    }
}

QString Ckernel::Gb2312ToUtf8(char *src)
{
    QTextCodec* pCodec = QTextCodec::codecForName("gb2312");
    return pCodec->toUnicode(src);
}

void Ckernel::setMap()
{
    memset(m_pFun,0,sizeof(m_pFun));
    m_pFun[_DEF_UDP_ONLINE_RQ-_DEF_UDP_BASE - 1] = &Ckernel::dealOnlineRq;
    m_pFun[_DEF_UDP_ONLINE_RS-_DEF_UDP_BASE - 1] = &Ckernel::dealOnlineRs;
    m_pFun[_DEF_UDP_CHAT_RQ-_DEF_UDP_BASE - 1] = &Ckernel::dealCharRq;
    m_pFun[_DEF_UDP_OFFLINE-_DEF_UDP_BASE - 1] = &Ckernel::dealOfflineRq;
    m_pFun[_DEF_TCP_REGISTER_RS-_DEF_UDP_BASE - 1] = &Ckernel::dealRegisterRs;
    m_pFun[_DEF_TCP_LOGIC_RS - _DEF_UDP_BASE - 1] = Ckernel::dealLoginRs;
    m_pFun[_DEF_TCP_FRIEND_INFO_RQ - _DEF_UDP_BASE - 1] = &Ckernel::dealFirendInfo;
    m_pFun[_DEF_TCP_CHAT_RQ - _DEF_UDP_BASE - 1] = &Ckernel::dealTcpChatRq;
    m_pFun[_DEF_TCP_CHAT_RS - _DEF_UDP_BASE - 1] = &Ckernel::dealTcpChatRs;
    m_pFun[_DEF_TCP_OFFLINE_RQ - _DEF_UDP_BASE - 1] = &Ckernel::dealTcpOfflineRq;
    m_pFun[_DEF_TCP_ADD_FRIEND_RQ- _DEF_UDP_BASE - 1]= &Ckernel::dealTcpAddFriendRq;
    m_pFun[_DEF_TCP_ADD_FRIEND_RS- _DEF_UDP_BASE - 1]= &Ckernel::dealTcpAddFriendRs;
    //m_pFun[_DEF_TCP_SENDFILE_RQ - _DEF_UDP_BASE - 1] = &Ckernel::dealTCPSendFileRq;
    //m_pFun[_DEF_TCP_SENDFILE_RS - _DEF_UDP_BASE - 1] = &Ckernel::dealTcpSendFileRs;
    m_pFun[_DEF_TCP_GROUP_INFO_RQ - _DEF_UDP_BASE - 1] = &Ckernel::dealTcpGroupInfoRq;
    m_pFun[_DEF_TCP_GROUP_CHAT_RQ - _DEF_UDP_BASE - 1] = &Ckernel::dealTcpGroupContentRq;
    m_pFun[_DEF_TCP_GROUP_CHAT_RS - _DEF_UDP_BASE - 1] = &Ckernel::dealTcpGroupContentRs;
    //m_pFun[_DEF_TCP_SEND_ONOFF - _DEF_UDP_BASE - 1] = &Ckernel::dealTcpSendOnOff;
    m_pFun[_DEF_TCP_FILE_INFO - _DEF_UDP_BASE - 1] = &Ckernel::dealTcpFileInfo;
    m_pFun[_DEF_TCP_FILE_BLOCK_RQ - _DEF_UDP_BASE - 1] = &Ckernel::dealTcpFileBlockRq;
    m_pFun[_DEF_TCP_FILE_CHECK_RS - _DEF_UDP_BASE - 1] = &Ckernel::dealTcpFileCheckRs;
}

void Ckernel::addFriend(UserItem)
{

}

void Ckernel::sendFile(QString path, int id, bool* quit)
{
    qDebug() <<__func__ ;
//    pFileFrom = file;
//    if(pFileFrom)
//    {
//        int len =fread(buf,sizeof(char),1024,pFileFrom);
//        if(len > 0)
//        {
//            _STRU_TCP_SENDFILE_RQ rq;
//            memcpy(rq.buff,buf,sizeof(rq.buff));
//            rq.friendId = friendId;
//            rq.userId = m_id;
//            rq.len = len;
//            strcpy(rq.path,(const char*)c.toStdString().c_str());
//            len =fread(buf,sizeof(char),1024,pFileFrom);
//            m_pTcpMid->SendData(00,(char*)&rq,sizeof(rq));

//        }
//    }
    STRU_FILE_INFO info; //发送文件信息
    QFileInfo f(path.toStdString().c_str());
    info.szFileSize = f.size();  //文件大小
    QDateTime time = QDateTime::currentDateTime();
    QString timestr = time.toString("yyddhhmmsszzz");
    memcpy(info.szFileId, timestr.toStdString().c_str(),_MAX_FILE_PATH); //文件id
    memcpy(info.szFileName, f.fileName().toStdString().c_str(), _MAX_FILE_PATH); //文件名
    info.userId = m_id;
    info.friendId = id;

    QString str(info.szFileName);
    auto ite = m_mapFileIdToFileInfoSend.find(str);
    if(ite != m_mapFileIdToFileInfoSend.end())
    {
        qDebug() << "错误：当前文件正在发送";
        return;
    }

    m_mapFileIdToFileInfoSend[str] = info; //设置发送的文件映射

    m_pTcpMid->SendData(0, (char*)&info, sizeof(info));
    FILE* file = nullptr;
    file = fopen(path.toStdString().c_str(), "rb");
    //设置文件指针
//    FILE* file = fopen(path.toStdString().c_str(), "r");
//    fseek(file, 0, SEEK_END);
//    info.szFileSize = ftell(file);
//    fseek(file, 0, SEEK_SET);

    int64_t pos = 0;
    int size = _MAX_FILE_CONTENT_SIZE;
    while(pos < info.szFileSize && size == _MAX_FILE_CONTENT_SIZE && !(*quit))
    {
        STRU_FILE_BLOCK_RQ rq;
        rq.userId = m_id;

        size = fread(rq.szFileContent,1, _MAX_FILE_CONTENT_SIZE, file);
        //size = fread(rq.szFileContent, 1, _MAX_FILE_CONTENT_SIZE, file);

        rq.szFileSize = size;
        pos += size;

        rq.friendId = id;         //向id的用户发送文件
        memcpy(rq.szFileId, info.szFileId, _MAX_FILE_PATH);

        m_pTcpMid->SendData(0, (char*)&rq, sizeof(rq)); // 发送文件块

        //设置发送的滑动条
        int value = (int)((double)pos / info.szFileSize * 100);
        Q_EMIT SIG_setSendFileProcess(id, value);
        if(value == 100)
        {
            Q_EMIT SIG_setSendFileProcess(id, 0);
        }
        if(pos >= info.szFileSize || size < _MAX_FILE_CONTENT_SIZE)
        {
            m_mapFileIdToFileInfoSend.erase(ite);
        }
    }
    fclose(file);
}

//void Ckernel::continueSend(_STRU_TCP_SENDFILE_RS rs)
//{
//    qDebug()<<__func__;
//    if(pFileFrom)
//    {
//        int len =fread(buf,sizeof(char),1024,pFileFrom);
//        if(len > 0)
//        {
//            _STRU_TCP_SENDFILE_RQ rq;
//            memcpy(rq.buff,buf,sizeof(rq.buff));
//            rq.friendId = rs.friendId;
//            rq.userId = m_id;
//            rq.len = len;
//            fread(buf,sizeof(char),1024,pFileFrom);
//            m_pTcpMid->SendData(00,(char*)&rq,sizeof(rq));

//        }
//    }
//}

void Ckernel::dealOnlineRq(long ISend, char *sendBuf, int nLen)
{
    qDebug()<<__func__;
    //拆包
    _STRU_UDP_ONLINE_RQ* rq = (_STRU_UDP_ONLINE_RQ*)  sendBuf;
    //把好友添加到列表上
    UserItem* item = new UserItem; // new一个好友
    struct in_addr addr;
    addr.S_un.S_addr = ISend;
    item->setFriendInfo(0,7,status_online,Gb2312ToUtf8(rq->name),inet_ntoa(addr));
    m_p->addFriend(ISend,item);//好友加到列表上
    //绑定显示聊天窗口的信号和槽函数
    connect(item,SIGNAL(SIG_ShowChatIp(QString)),this,SLOT(slot_ShowChatId(QString)));
    //new一个聊天窗口
    ChatDialog* chat = new ChatDialog;

    //保存到聊天map中
    m_pIpChat[ISend] = chat;
    connect(chat,SIGNAL(SIG_SendContent(QString,QString)),this,SLOT(slot_SendContent(QString,QString)));
    //绑定发送文件的函数
    connect(chat, SIGNAL(SIG_SendFile(QString, int)), this, SLOT(slot_sendFile(QString,int)));
    ////绑定文件发送进度条
    //connect(this, SIGNAL(SIG_setSendFileProcess(int)), chat, SLOT(slot_setSendFileProcess(int)));

    //设置聊天窗口属性
    chat->SetChatWindow(inet_ntoa(addr));

    //判断是不是自己的上线请求,是自己的就不回复
    //获取本地ip
//    set<long> ipList = INet::GetIpList();
//    //判断发送数据的ip是否在本地列表中
//    if(ipList.count(ISend)>0)
//    {
//        return;
//    }
    //给好友一个回复
    _STRU_UDP_ONLINE_RS rs;
    gethostname(rs.reply,sizeof(rs.name));
    m_pUdpMid->SendData(ISend,(char*)&rs,sizeof(rs));
}

void Ckernel::dealOnlineRs(long ISend, char *sendBuf, int nLen)
{
    qDebug()<<__func__ ;
//    _STRU_UDP_ONLINE_RQ* rq = (_STRU_UDP_ONLINE_RQ*)  sendBuf;
//    //把好友添加到列表上
//    UserItem* item = new UserItem; // new一个好友
//    struct in_addr addr;
//    addr.S_un.S_addr = ISend;
//    item->setFriendInfo(0,7,status_online,rq->name,inet_ntoa(addr));
//    m_p->addFriend(item);//好友加到列表上
//    //绑定显示聊天窗口的信号和槽函数
//    connect(item,SIGNAL(SIG_ShowChatIp(QString)),this,SLOT(slot_ShowChatId(QString)));
//    //new一个聊天窗口
//    ChatDialog* chat = new ChatDialog;
//    //保存到聊天map中
//    m_pIpChat[ISend] = chat;
//    connect(chat,SIGNAL(SIG_SendContent(QString,QString)),this,SLOT(slot_SendContent(QString,QString)));
//    //设置聊天窗口属性
//    chat->SetChatWindow(inet_ntoa(addr));
}

void Ckernel::dealCharRq(long ISend, char *sendBuf, int nLen)
{
    qDebug()<<__func__ ;
    _STRU_UDP_CHAT_RQ* rq = (_STRU_UDP_CHAT_RQ*)sendBuf;
    if(m_pIpChat.find(ISend) != m_pIpChat.end())
    {
        ChatDialog* pChat = m_pIpChat[ISend];
        pChat->SetChatContent(rq->content);
        pChat->showNormal();

    }

}

void Ckernel::dealOfflineRq(long ISend, char *sendBuf, int nLen)
{
    qDebug()<<__func__ ;
    auto ite = m_pIpChat.find(ISend);
    if(ite != m_pIpChat.end())
    {
        ChatDialog* chat = *ite;
        if(chat)
        {
            chat->hide();
            delete chat;
            chat = nullptr;
        }
        m_pIpChat.erase(ite);
    }
}

void Ckernel::dealRegisterRs(long ISend, char* sendBuf, int nLen)
{
    qDebug()<<__func__ ;
    _STRU_TCP_REGISTER_RS* rs = (_STRU_TCP_REGISTER_RS*)sendBuf;
    switch (rs->reply) {
    case register_success:
        QMessageBox::about(m_p,"message","register_success");
        break;
    case register_name_used:
        QMessageBox::about(m_p,"message","register_name_used");
        break;
    case register_tel_used:
        QMessageBox::about(m_p,"message","register_name_used");
        break;
    default:
        break;
    }
}

void Ckernel::dealLoginRs(long ISend, char *sendBuf, int nLen)
{
    qDebug()<<__func__ ;
    _STRU_TCP_LOGIC_RS* rs = (_STRU_TCP_LOGIC_RS*)sendBuf;
    switch(rs->result)
    {
    case login_success:
    {
        m_pLogDia->hide();
        m_id = rs->id;
        m_p->show();
    }
        break;
    case login_password_error:
        QMessageBox::about(m_p,"message","login_password_error");
        break;
    case login_no_this_user:
        QMessageBox::about(m_p,"message","login_no_this_user");
        break;
    }
}

void Ckernel::dealFirendInfo(long ISend, char *sendBuf, int nLen)
{
   _STRU_TCP_FRIEND_INFO_RQ* rq = (_STRU_TCP_FRIEND_INFO_RQ*) sendBuf;

    if(rq->userId == m_id)
    {
        //是自己的id
        m_name = rq->name;
        QString name = Gb2312ToUtf8(rq->name);
        QString feeling = Gb2312ToUtf8(rq->feeling);
        m_p->setUserInfo(name,feeling,rq->picId);
        return;
    }
    else
    {
        if(m_mapIdUserItem.count(rq->userId) > 0)
        {
            UserItem* item = m_mapIdUserItem[rq->userId];
            QString name = Gb2312ToUtf8(rq->name);
            QString feeling = Gb2312ToUtf8(rq->feeling);
            item->setFriendInfo(rq->userId,rq->picId, rq->state,name, feeling,true);
        }
        else
        {
            UserItem* item = new UserItem;
            QString name = Gb2312ToUtf8(rq->name);
            QString feeling = Gb2312ToUtf8(rq->feeling);
            item->setFriendInfo(rq->userId,rq->picId, rq->state,name, feeling,true);


            m_p->addTcpFriend(item);
            m_mapIdUserItem[rq->userId] = item;
            connect(item,SIGNAL(SIG_ShowTcpChatIp(int)),this,SLOT(slot_ShowTcpChatIp(int)));
            ChatDialog* ca = new ChatDialog;
            ca->SetChatWindow(name,rq->userId);
            m_mapIdChat[rq->userId] = ca;
            //绑定聊天的发送文件函数
            connect(ca,SIGNAL(SIG_SendFile(QString,int)),this,SLOT(slot_sendFile(QString,int)));
//            m_mapIpFILE[item->showId()] = new FILE;
//            m_mapIpFILE[item->showId()] = fopen("d:\\bmd1.txt", "wb");
            connect(ca,SIGNAL(SIG_SendTcpContent(QString,int)),this,SLOT(slot_SendTcpContent(QString,int)));
            //绑定文件传输的进度
            connect(this, SIGNAL(SIG_setSendFileProcess(int,int)), ca, SLOT(slot_setSendFileProcess(int, int)));
            //绑定文件重传
            connect(ca,SIGNAL(SIG_resend(QString,int)), this, SLOT(slot_ResendFile(QString, int)));
            //绑定文件查看
            connect(ca, SIGNAL(SIG_fileCheck(int)), this, SLOT(slot_checkFile(int)));
        }

    }

}

void Ckernel::dealTcpChatRq(long ISend, char *sendBuf, int nLen)
{
    qDebug() << __func__;
    _STRU_TCP_CHAT_RQ* rq = (_STRU_TCP_CHAT_RQ*)sendBuf;
    if(m_mapIdChat.count(rq->userId) > 0)
    {
        ChatDialog* pChat = m_mapIdChat[rq->userId];
        pChat->SetChatContent(rq->content);
        pChat->showNormal();
    }
}

void Ckernel::dealTcpChatRs(long ISend, char *sendBuf, int nLen)
{
    qDebug() << __func__;
    _STRU_TCP_CHAT_RS* rs = (_STRU_TCP_CHAT_RS*)sendBuf;
    if(m_mapIdChat.count(rs->friendId) > 0)
    {
        ChatDialog* pChat = m_mapIdChat[rs->friendId];
        pChat->SetChatContent(QString("好友不在线"));
        pChat->showNormal();

    }
    qDebug() << "处理结束" ;
}

void Ckernel::dealTcpAddFriendRq(long ISend, char *sendBuf, int nLen)
{
    qDebug() <<__func__;
    _STRU_TCP_ADD_FRIEND_RQ* rq = (_STRU_TCP_ADD_FRIEND_RQ*)sendBuf;
    _STRU_TCP_ADD_FRIEND_RS rs;
    rs.result = add_frined_success;
    QString name = Gb2312ToUtf8(rq->userName);
    if(QMessageBox::Yes == QMessageBox::question(m_p,"Yes",QString("[%1]添加好友").arg(name)))
    {
        rs.result = add_frined_success;
    }
    else
    {
        rs.result = add_frined_refuse;
    }
    rs.id = rq->userId;
     strcpy(rs.userName,rq->friendName);
    //回复
    m_pTcpMid->SendData(00,(char*)&rs,sizeof(rs));
}

void Ckernel::dealTcpAddFriendRs(long ISend, char *sendBuf, int nLen)
{
    qDebug() <<__func__;
    _STRU_TCP_ADD_FRIEND_RS* rs = (_STRU_TCP_ADD_FRIEND_RS*)sendBuf;
    QString name = Gb2312ToUtf8(rs->userName);
    switch (rs->result) {
    case add_frined_success:
        QMessageBox::about(m_p,"message","添加成功");
        break;
    case add_frined_refuse:
        QMessageBox::about(m_p,"message","添加失败");
        break;
    case add_frined_exists:
        QMessageBox::about(m_p,"message",QString("[%1]用户不存在").arg(name));
        break;
    case add_frined_offline:
        QMessageBox::about(m_p,"message",QString("[%1]用户不在线").arg(name));
        break;
    default:
        break;
    }
}

void Ckernel::dealTcpOfflineRq(long ISend, char *sendBuf, int nLen)
{
    qDebug() << __func__;
    _STRU_TCP_OFFLINE_RQ* rq = (_STRU_TCP_OFFLINE_RQ*)sendBuf ;
    if(m_mapIdUserItem.count(rq->userId) > 0 && rq->userId != m_id)
    {
        UserItem* item = m_mapIdUserItem[rq->userId];
        item->setIconBlack();
    }
}

//void Ckernel::dealTCPSendFileRq(long ISend, char *sendBuf, int nLen)
//{
//    qDebug() <<__func__;
//    _STRU_TCP_SENDFILE_RQ* rq = (_STRU_TCP_SENDFILE_RQ*)sendBuf;
//    QString q = QString("E:/").append(rq->path);
//    if(canChangName == true)
//    {
//        m_mapIpFILE[rq->userId] = fopen(q.toStdString().c_str(), "wb");
//    }
//    canChangName = false;
//    char buff[1024];
//    strcpy(buff,rq->buff);
//    fwrite(buff, sizeof(char), rq->len, m_mapIpFILE[rq->userId]);

//    _STRU_TCP_SENDFILE_RS rs;
//    rs.userId = rq->userId;
//    rs.friendId = rq->friendId;
//    if(m_mapIpFILE[rq->userId])
//    {

//        rs.result = recve_file_success;
//        if(rq->len <1024)
//        {
//            rs.result = recve_file_over;
//        }
//    }
//    else
//    {
//        rs.result = recve_file_false;
//    }
//    m_pTcpMid->SendData(00,(char*)&rs,sizeof(rs));

//}

//void Ckernel::dealTcpSendFileRs(long ISend, char *sendBuf, int nLen)
//{
//    qDebug() <<__func__;

//    _STRU_TCP_SENDFILE_RS* rs = (_STRU_TCP_SENDFILE_RS*)sendBuf;
//    switch (rs->result) {
//    case send_file_success:
//            continueSend(*rs);
//            qDebug() << "travel running";

//        //QMessageBox::about(m_mapIdChat[rs->userId],"message","travel running");

//        break;
//    case send_file_false:
//        //QMessageBox::about(m_mapIdChat[rs->userId],"message","传输失败");
//        qDebug() << "传输失败";
//    case send_file_offline:
//        QMessageBox::about(m_mapIdChat[rs->userId],"message","用户不在线");
//        break;
//    case recve_file_over:
//        fclose(m_mapIpFILE[rs->userId]);
//        m_mapIpFILE[rs->userId] = nullptr;
//        canChangName = true;
//        QMessageBox::about(m_mapIdChat[rs->userId],"message","接受结束");
//        qDebug() << "接受完毕";
//        break;
//    case send_file_over:
//        fclose(pFileFrom);
//        QMessageBox::about(m_mapIdChat[rs->friendId],"message","发送结束");
//        pFileFrom = nullptr;
//        qDebug()<<"发送完毕";
//        break;
//    }
//}

void Ckernel::dealTcpGroupInfoRq(long ISend, char *sendBuf, int nLen)
{
    qDebug() << __func__;
    _STRU_TCP_GROUP_INDO_RQ* rq = (_STRU_TCP_GROUP_INDO_RQ*)sendBuf;
    m_mapidGroupChat[rq->groupId] = new ChatDialog;
    m_mapIdGroupItem[rq->groupId] = new GroupItem;
    connect(m_mapIdGroupItem[rq->groupId],SIGNAL(SIG_show_group_chat(int)),this,SLOT(slot_show_group_chat(int)));
    connect(m_mapidGroupChat[rq->groupId],SIGNAL(SIG_SendTcpGroupContent(QString,int)),this,SLOT(slot_SendTcpGroupContent(QString,int)));
    QString name = Gb2312ToUtf8(rq->name);
    QString feeling = Gb2312ToUtf8(rq->feeling);
    //设置群信息
    m_mapIdGroupItem[rq->groupId]->setGroupInfo(rq->groupId,rq->icon,name,feeling);
    //设置聊天窗口信息
    m_mapidGroupChat[rq->groupId]->SetGroupId(rq->groupId);
    m_p->addGroup(m_mapIdGroupItem[rq->groupId]);
}

void Ckernel::dealTcpGroupContentRq(long ISend, char *sendBuf, int nLen)
{
    qDebug() << __func__;
    _STRU_TCP_GROUP_CHAT_RQ* rq = (_STRU_TCP_GROUP_CHAT_RQ*)sendBuf;
    QString content = Gb2312ToUtf8(rq->content);
    m_mapidGroupChat[rq->groupId]->SetTcpGroupChatContent(rq->content,rq->userIp);
    m_mapidGroupChat[rq->groupId]->showNormal();

}

void Ckernel::dealTcpGroupContentRs(long ISend, char *sendBuf, int nLen)
{
    _STRU_TCP_GROUP_CHAT_RS* rs = (_STRU_TCP_GROUP_CHAT_RS*) sendBuf;
    switch (rs->result) {
    case send_success:
        break;
    case send_fail:
        m_mapidGroupChat[rs->groupId]->SetTcpGroupChatContent(QString("无人在线"),0);
        break;
    default:
        break;
    }
}

void Ckernel::dealTcpFileInfo(long ISend, char *sendBuf, int nLen)
{
    STRU_FILE_INFO rq = *(STRU_FILE_INFO*)sendBuf;
    QString id(rq.szFileId);
    if(m_mapFileIdToFileInfoRecv.find(id) == m_mapFileIdToFileInfoRecv.end())
    {
        Filelfo fileInfo;
        fileInfo.nFileSize = rq.szFileSize;
        memcpy(fileInfo.szFileNAME, rq.szFileName, _MAX_FILE_PATH);
        memcpy(fileInfo.szFileId, rq.szFileId, _MAX_FILE_PATH);
        sprintf(fileInfo.szFilePATH, "%s/%s", _DOWNLOAD_PATH, rq.szFileName);

        fileInfo.pFile = fopen(fileInfo.szFilePATH, "wb");

        m_mapFileIdToFileInfoRecv[id] = fileInfo;

        //添加文件的接收窗口
        ChatDialog* chat = this->m_mapIdChat[rq.userId];
        QString fileName(rq.szFileName);
        if(!chat->haveFileWidget(fileName))
        {
            chat->addRecvItem(fileName, rq.szFileSize);
        }
        else
            cout << "当前接收文件的窗口已经存在" << endl;
    }
    else
    {
        qDebug() << "当前文件已经在接受";
    }
}

void Ckernel::dealTcpFileBlockRq(long ISend, char *sendBuf, int nLen)
{
    STRU_FILE_BLOCK_RQ rq = *(STRU_FILE_BLOCK_RQ*)sendBuf;
    QString id(rq.szFileId);
    auto ite = m_mapFileIdToFileInfoRecv.find(id);
    if(ite == m_mapFileIdToFileInfoRecv.end())
    {
        qDebug() << "文件块接受失败：没有对应需要接受的文件";
        return;
    }
    FILE* f = (*ite).pFile;
    int size = fwrite(rq.szFileContent, 1, rq.szFileSize, f);
    if(size <= 0)
    {
        qDebug() << "写入失败";
        return;
    }
    Filelfo* info = &(ite.value());
    info->nPos += size;

    //设置接收的滑动条
    ChatDialog* chat = m_mapIdChat[rq.userId];
    chat->setRecvProcessBar(QString(info->szFileNAME), (double)info->nPos / info->nFileSize * 100);

    if(info->nPos >= info->nFileSize || rq.szFileSize < _MAX_FILE_CONTENT_SIZE)
    {
        qDebug() << "文件接受完成";
        fclose(f);

        m_mapFileIdToFileInfoRecv.erase(ite);
//        delete[] info->szFileId;
//        delete[] info->szFileNAME;
//        delete[] info->szFilePATH;
    }
}

void Ckernel::dealTcpFileBlockRs(long ISend, char *sendBuf, int nLen)
{
    STRU_FILE_BLOCK_RS rs = *(STRU_FILE_BLOCK_RS*)sendBuf;
    QString id(rs.szFileId);
    auto ite = m_mapFileIdToFileInfoSend.find(id);
    if(ite == m_mapFileIdToFileInfoSend.end())
    {
        qDebug() << "未找到文件";
        return;
    }
    int value = (int)(rs.szPos / (double)ite->szFileSize * 100);
    //设置滑动条
    auto ite1 = m_mapIdChat.find(ite->friendId);
    if(ite1 == m_mapIdChat.end())
    {
        qDebug() << "没有对应的用户窗口";
        return;
    }
    ite1.value()->SetSendFileBar(value);
}

void Ckernel::dealTcpFileCheckRs(long ISend, char *sendBuf, int nLen)
{
    qDebug() <<__func__;
    STRU_FILE_CHECK_RS rs = *(STRU_FILE_CHECK_RS*)sendBuf;
    //添加文件的接收窗口
    ChatDialog* chat = this->m_mapIdChat[rs.userId];
    QString fileName(rs.fileName);
    if(!chat->haveFileWidget(fileName))
    {
        chat->addRecvItem(fileName, rs.fileSize);
    }
    else
        cout << "当前接收文件的窗口已经存在" << endl;
}

//void Ckernel::dealTcpSendOnOff(long ISend, char *sendBuf, int nLen)
//{
//    _STRU_TCP_SEND_ONOFF* onOff = (_STRU_TCP_SEND_ONOFF*) sendBuf;
//}
//处理接受到的数据
void Ckernel::slot_ReadyDate(long ISend, char* sendBuf, int nLen)
{
    qDebug()<< __func__ ;
    PackType type = *(PackType*)sendBuf;

    int index = type - _DEF_UDP_BASE -1 ;
    if(index>_FUN_NUMBER_MAX|| index <0)
    {
        qDebug()<< "数组越界" ;
    }
    else
    {
        qDebug()<<"数组正确"    ;
        PFun pf = m_pFun[index];
        if(pf)
        {
            (this->*pf)(ISend,sendBuf,nLen);
        }
        else
        {
            qDebug()<<index<<"  " << type << endl;
        }
    }
    delete[] sendBuf;
}

void Ckernel::slots_registerSubmit(QString name, QString tel, QString password)
{
    qDebug()<<__func__;
    //封包
    _STRU_TCP_REGISTER_RQ rq;
    memcpy(rq.name,name.toStdString().c_str(),name.length());
    memcpy(rq.tel,tel.toStdString().c_str(),tel.length());
    memcpy(rq.password,password.toStdString().c_str(),password.length());
    //发送到服务端
    m_pTcpMid->SendData(120,(char*)&rq,sizeof(rq));

}

void Ckernel::slots_logic(QString tel, QString password)
{
    qDebug()<<__func__;
    _STRU_TCP_LOGIC_RQ rq;

    memcpy(rq.password,password.toStdString().c_str(),password.length());
    memcpy(rq.tel,tel.toStdString().c_str(),tel.length());
    m_pTcpMid->SendData(00,(char*)&rq,sizeof(rq));
}

void Ckernel::slot_ShowChatId(QString ip)
{
    long LIp= inet_addr(ip.toStdString().c_str());
    if(m_pIpChat.find(LIp) != m_pIpChat.end())
    {
        ChatDialog* pChat = m_pIpChat[LIp];
        pChat->showNormal();

    }
}



void Ckernel::slot_ShowTcpChatIp(int id)
{
    qDebug() << __func__;
    if(m_mapIdChat.find(id) != m_mapIdChat.end())
    {
        ChatDialog* pChat = m_mapIdChat[id];
        pChat->showNormal();
//        //发送文件重传请求
//        STRU_FILE_RESEND send;
//        send.friendId = id;
//        send.userId = m_id;
//        m_pTcpMid->SendData(0, (char*)&send, sizeof(send));
    }
}
//处理聊天内容
void Ckernel::slot_SendContent(QString content, QString ip)
{
    qDebug() << __func__;
    _STRU_UDP_CHAT_RQ rq;
    memcpy(rq.content,content.toStdString().c_str(),content.length());
    //发给对端
    long LIp= inet_addr(ip.toStdString().c_str());
    m_pUdpMid->SendData(LIp,(char*)&rq,sizeof(rq));


}
void Ckernel::slot_SendTcpContent(QString content, int id)
{
    qDebug() << __func__;
    _STRU_TCP_CHAT_RQ rq;
    memcpy(rq.content,content.toStdString().c_str(),strlen(content.toStdString().c_str()));
    rq.userId = m_id;
    rq.friendId = id;
    //发给对端
    m_pTcpMid->SendData(00,(char*)&rq,sizeof(rq));
}
void Ckernel::slot_closeApp()
{
    //停止当前的工作进程
    if(m_sendFileThread)
    {
        delete m_sendFileThread;
        m_sendFileThread = nullptr;
    }

    _STRU_TCP_OFFLINE_RQ tRq;
    tRq.userId = m_id;
    m_pTcpMid->SendData(00,(char*)&tRq,sizeof(tRq));
    qDebug() << __func__;
    //发送下线通知
    _STRU_UDP_OFFONLINE_RQ rq;
    m_pUdpMid->SendData(INADDR_BROADCAST,(char*)&rq,sizeof(rq));
    //回收资源
    slot_closeLog();

    delete this;
}

void Ckernel::slot_closeLog()
{
    qDebug() << __func__;

    if(m_pLogDia)
    {
        m_pLogDia->hide();
        delete m_pLogDia;
        m_pLogDia = nullptr;
    }
    if(m_pUdpMid)
    {
        m_pUdpMid->UnInitNet();
        delete m_pUdpMid;
        m_pUdpMid = nullptr;
    }
    if(m_pTcpMid)
    {
        m_pTcpMid->UnInitNet();
        delete m_pTcpMid;
        m_pTcpMid = nullptr;
    }
    for(auto ite = m_pIpChat.begin();ite != m_pIpChat.end();)
    {
        ChatDialog* chat = *ite;
        if(chat)
        {
            chat->hide();
            delete chat;
            chat = nullptr;
        }
        ite = m_pIpChat.erase(ite);
    }
    if(m_p)
    {
        m_p->hide();
        delete m_p;
        m_p = nullptr;

    }
    exit(0);
}

void Ckernel::slot_addFriend()
{
    qDebug() <<__func__;
    QString name = QInputDialog::getText(m_p,"add friend","please input friend name");
    QString nameTemp = name;
    if(name.isEmpty() || nameTemp.remove(" ").isEmpty())
    {
        QMessageBox::about(m_p,"Message","名字是空");
        return;
    }
    for(auto ite = m_mapIdUserItem.begin();ite!= m_mapIdUserItem.end();ite++)
    {
        UserItem* item = *ite;
        if(item->name() == name)
        {
            QMessageBox::about(m_p,"Message",QString("[%1] 已经是好友了").arg(name));
            return;
        }
    }
    //添加好友请求
    _STRU_TCP_ADD_FRIEND_RQ rq;
    rq.userId = m_id;
    strcpy(rq.userName,m_name.toStdString().c_str()/*,m_name.length()*/);
    Utf8ToGb2312(name,rq.friendName,sizeof(name));
    //memcpy(rq.friendName,name.toStdString().c_str(),name.length());
    m_pTcpMid->SendData(000,(char*)&rq,sizeof(rq));
}
#include<QFile>
void Ckernel::slot_sendFile(QString path, int id)
{
    qDebug() << __func__;
    m_sendFileThread->setInfo(path, id);
    m_sendFileThread->start();
}

void Ckernel::slot_show_group_chat(int id)
{
    m_mapidGroupChat[id]->showNormal();
}

void Ckernel::slot_SendTcpGroupContent(QString content, int id)
{
    qDebug() << __func__;
    _STRU_TCP_GROUP_CHAT_RQ rq;
    memcpy(rq.content,content.toStdString().c_str(),content.length());
    rq.userId = m_id;
    rq.groupId = id;
    //发给对端
    m_pTcpMid->SendData(00,(char*)&rq,sizeof(rq));
}

void Ckernel::slot_ResendFile(QString fileName, int id)
{
    STRU_FILE_RESEND resend;
    resend.userId = id;
    resend.friendId = m_id;
    memcpy(resend.szFileNAME, fileName.toStdString().c_str(), _MAX_FILE_PATH);
    m_pTcpMid->SendData(0, (char*)&resend, sizeof(resend));
}

void Ckernel::slot_checkFile(int id)
{
    STRU_FILE_CHECK_RQ rq;
    rq.userId = id;
    rq.friendId = m_id;
    m_pTcpMid->SendData(0, (char*)&rq, sizeof(rq));
}
