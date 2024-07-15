#pragma once
#include<QFile>
#include<string.h>
#include<stdio.h>
#define DEFINE_TCP_PORT (75845)
#define DEFINE_UDP_PORT (84545)
#define SEVER_IP ("192.168.43.125")

typedef int PackType;
#define _FUN_NUMBER_MAX 30
#define _DEF_UDP_BASE (1000)
#define _NAME_MAX 30
#define _REPLY_MAX 2048
#define _MAX_FILE_PATH (100)
#define _MAX_FILE_CONTENT_SIZE (1024 * 10)
#define _DOWNLOAD_PATH "D:/qtobject/wL/download"
#define _MAX_SEND_FILE_SIZE ((long long)2 * 1024 * 1024 * 1024)
//UDP局域网通信设备：上线广播请求，上线回复、聊天请求、回复请求
#define _DEF_UDP_ONLINE_RQ	(_DEF_UDP_BASE + 1)
#define _DEF_UDP_ONLINE_RS	(_DEF_UDP_BASE + 2)
#define _DEF_UDP_CHAT_RQ	(_DEF_UDP_BASE + 3)
#define _DEF_UDP_OFFLINE	(_DEF_UDP_BASE + 4)
//TCP广域网通信软件：注册请求、注册回复、登录请求、登录回复、
//获取好友信息、聊天请求、聊天回复、添加好友请求、添加好友回复、下线请求
#define _DEF_TCP_REGISTER_RQ	 (_DEF_UDP_BASE + 5)
#define _DEF_TCP_REGISTER_RS	 (_DEF_UDP_BASE + 6)
#define _DEF_TCP_LOGIC_RQ		 (_DEF_UDP_BASE + 7)
#define _DEF_TCP_LOGIC_RS		 (_DEF_UDP_BASE + 8)
#define _DEF_TCP_FRIEND_INFO_RQ  (_DEF_UDP_BASE + 9)
#define _DEF_TCP_CHAT_RQ		 (_DEF_UDP_BASE + 10)
#define _DEF_TCP_CHAT_RS		 (_DEF_UDP_BASE + 11)
#define _DEF_TCP_ADD_FRIEND_RS	 (_DEF_UDP_BASE + 12)
#define _DEF_TCP_ADD_FRIEND_RQ   (_DEF_UDP_BASE + 13)
#define _DEF_TCP_OFFLINE_RQ      (_DEF_UDP_BASE + 14)
#define _DEF_TCP_SENDFILE_RQ     (_DEF_UDP_BASE + 15)
#define _DEF_TCP_SENDFILE_RS     (_DEF_UDP_BASE + 16)
#define _DEF_TCP_GROUP_CHAT_RQ   (_DEF_UDP_BASE + 17)
#define _DEF_TCP_GROUP_INFO_RQ   (_DEF_UDP_BASE + 18)
#define _DEF_TCP_GROUP_CHAT_RS   (_DEF_UDP_BASE + 19)
#define _DEF_TCP_SEND_ONOFF      (_DEF_UDP_BASE + 20)
#define _DEF_TCP_FILE_INFO       (_DEF_UDP_BASE + 21)
#define _DEF_TCP_FILE_BLOCK_RQ   (_DEF_UDP_BASE + 22)
#define _DEF_TCP_FILE_BLOCK_RS   (_DEF_UDP_BASE + 23)
#define _DEF_TCP_FILE_RESEND     (_DEF_UDP_BASE + 24)
#define _DEF_TCP_FILE_CHECK_RQ      (_DEF_UDP_BASE + 25)
#define _DEF_TCP_FILE_CHECK_RS      (_DEF_UDP_BASE + 26)

//注册结果
#define register_success	(0)
#define register_name_used	(1)
#define register_tel_used	(2)
//登录结果
#define login_success		(0)
#define login_no_this_user	(1)
#define login_password_error (2)
// 聊天结果
#define send_success		(0)
#define send_fail			(1)
// 添加好友结果
#define add_frined_success	(0)
#define add_frined_offline	(1)
#define add_frined_exists	(2)
#define add_frined_refuse	(3)
// 用户状态
#define status_online		(0)
#define status_offline		(1)
//传输文件结果
#define send_file_success   (0)
#define send_file_false     (1)
#define send_file_offline   (2)
#define recve_file_success  (3)
#define recve_file_false    (4)
#define recve_file_over     (5)
#define send_file_over      (6)
//协议结构体
//上线请求:协议头
typedef struct _STRU_UDP_ONLINE_RQ
{
    _STRU_UDP_ONLINE_RQ() :ip(0), type(_DEF_UDP_ONLINE_RQ)
    {
        memset(name, 0, _NAME_MAX);
    }
    PackType type;
    long ip;
    char name[_NAME_MAX];
}_STRU_UDP_ONLINE_RQ;
//上线回复:协议头
typedef struct _STRU_UDP_ONLINE_RS
{
    _STRU_UDP_ONLINE_RS() :ip(0), type(_DEF_UDP_ONLINE_RS)
    {
        memset(reply, 0, _NAME_MAX);
        memset(name, 0, _NAME_MAX);

    }
    PackType type;
    long ip;
    char name[_NAME_MAX];
    char reply[_NAME_MAX];
}_STRU_UDP_ONLINE_RS;
//聊天请求:协议头
typedef struct _STRU_UDP_CHAT_RQ
{
    _STRU_UDP_CHAT_RQ() :ip(0), type(_DEF_UDP_CHAT_RQ), ipChatWith(0)
    {
        memset(content, 0, _REPLY_MAX);
    }
    PackType type;
    long ip;
    long ipChatWith;
    char content[_REPLY_MAX];
}_STRU_UDP_CHAT_RQ;
//下线广播请求:协议头
typedef struct _STRU_UDP_OFFONLINE_RQ
{
    _STRU_UDP_OFFONLINE_RQ() :ip(0), type(_DEF_UDP_OFFLINE)
    {
        memset(name, 0, _NAME_MAX);
    }
    PackType type;
    long ip;
    char name[_NAME_MAX];
}_STRU_UDP_OFFONLINE_RQ;
//注册请求:协议头 昵称 密码 手机号
typedef struct _STRU_TCP_REGISTER_RQ
{
    _STRU_TCP_REGISTER_RQ() :type(_DEF_TCP_REGISTER_RQ)
    {
        memset(tel, 0, _NAME_MAX);
        memset(name, 0, _NAME_MAX);
        memset(password, 0, _NAME_MAX);
    }
    PackType type;
    char tel[_NAME_MAX];
    char name[_NAME_MAX];
    char password[_NAME_MAX];
}_STRU_TCP_REGISTER_RQ;
//注册回复:协议头 注册结果（注册成功 昵称重复 手机号已被注册）
typedef struct _STRU_TCP_REGISTER_RS
{
    _STRU_TCP_REGISTER_RS() :ip(0), type(_DEF_TCP_REGISTER_RS), reply(0)
    {

    }
    PackType type;
    long ip;
    int reply;
}_STRU_TCP_REGISTER_RS;
//登录请求:协议头 密码 手机号
typedef struct _STRU_TCP_LOGIC_RQ
{
    _STRU_TCP_LOGIC_RQ() : type(_DEF_TCP_LOGIC_RQ)
    {
        memset(password, 0, _NAME_MAX);
        memset(tel, 0, _NAME_MAX);

    }
    PackType type;
    char password[_NAME_MAX];
    char tel[_NAME_MAX];
}_STRU_TCP_LOGIC_RQ;
//登录回复:协议头 登录结果（登录成功 密码错误 手机号未注册）
typedef struct _STRU_TCP_LOGIC_RS
{
    _STRU_TCP_LOGIC_RS() :type(_DEF_TCP_LOGIC_RS), id(0), result(0)
    {

    }
    PackType type;
    int result;
    int id;
}_STRU_TCP_LOGIC_RS;
//获取好友信息:协议头 昵称 状态 签名 用户id 头像id
typedef struct _STRU_TCP_FRIEND_INFO_RQ
{
    _STRU_TCP_FRIEND_INFO_RQ() : type(_DEF_TCP_FRIEND_INFO_RQ), state(status_offline), picId(0)
    {
        memset(name, 0, _NAME_MAX);
        memset(feeling, 0, _NAME_MAX);
    }
    PackType type;
    char name[_NAME_MAX];
    int state;
    char feeling[_NAME_MAX];
    int picId;
    int userId;
}_STRU_TCP_FRIEND_INFO_RQ;
//聊天请求:协议头 聊天内容 好友id 自己id
typedef struct _STRU_TCP_CHAT_RQ
{
    _STRU_TCP_CHAT_RQ() : type(_DEF_TCP_CHAT_RQ), userId(0), friendId(0)
    {
        memset(content, 0, _REPLY_MAX);

    }
    PackType type;
    int userId;
    int friendId;
    char content[_REPLY_MAX];
}_STRU_TCP_CHAT_RQ;
//聊天回复:协议头 发送结果（发送成功 好友不在线）
typedef struct _STRU_TCP_CHAT_RS
{
    _STRU_TCP_CHAT_RS() :type(_DEF_TCP_CHAT_RS), result(0), friendId(0)
    {

    }
    PackType type;
    int result;
    int friendId;
}_STRU_TCP_CHAT_RS;
//添加好友请求:协议头 自己的id 自己的昵称 好友的昵称
typedef struct _STRU_TCP_ADD_FRIEND_RQ
{
    _STRU_TCP_ADD_FRIEND_RQ() :type(_DEF_TCP_ADD_FRIEND_RQ),userId(0)
    {
        memset(userName, 0, _NAME_MAX);
        memset(friendName, 0, _NAME_MAX);

    }
    PackType type;
    int userId;
    char userName[_NAME_MAX];
    char friendName[_NAME_MAX];
}_STRU_TCP_ADD_FRIEND_RQ;
//添加好友回复:协议头 添加结果（成功 好友不在线 好友不存在 拒绝参加）
typedef struct _STRU_TCP_ADD_FRIEND_RS
{
    _STRU_TCP_ADD_FRIEND_RS() :type(_DEF_TCP_ADD_FRIEND_RS), id(0), result(-1)
    {
        memset(userName, 0, _NAME_MAX);
    }
    PackType type;
    char userName[_NAME_MAX];
    int result;
    int id;
}_STRU_TCP_ADD_FRIEND_RS;
//下线请求:协议头 自己的id
typedef struct _STRU_TCP_OFFLINE_RQ
{
    _STRU_TCP_OFFLINE_RQ() :type(_DEF_TCP_OFFLINE_RQ)
    {

    }
    PackType type;
    int userId;
}_STRU_TCP_OFFLINE_RQ;
//发送文件请求
typedef struct _STRU_TCP_SENDFILE_RQ
{
    _STRU_TCP_SENDFILE_RQ() :type(_DEF_TCP_SENDFILE_RQ), userId(-1), friendId(-1), len(0)
    {
        memset(buff, 0, sizeof(buff));
        memset(path, 0, sizeof(path));

    }
    PackType type;
    char buff[1024];
    int len;
    int userId;
    int friendId;
    char path[100];
}_STRU_TCP_SENDFILE_RQ;
//发送文件回复
typedef struct _STRU_TCP_SENDFILE_RS
{
    _STRU_TCP_SENDFILE_RS():type(_DEF_TCP_SENDFILE_RS),userId(-1),friendId(-1),result(1)
    {
    }
    PackType type;
    int result;
    int userId;
    int friendId;
}_STRU_TCP_SENDFILE_RS;
//群聊请求
typedef struct _STRU_TCP_GROUP_CHAT_RQ
{
    _STRU_TCP_GROUP_CHAT_RQ():type(_DEF_TCP_GROUP_CHAT_RQ),groupId(-1),userId(-1),userIp(0)
    {
        memset(content,0,_REPLY_MAX);
    }
    PackType type;
    int groupId;
    int userId;
    int userIp;
    char content[_REPLY_MAX];
}_STRU_TCP_GROUP_CHAT_RQ;
//群聊的回复
typedef struct _STRU_TCP_GROUP_CHAT_RS
{
    _STRU_TCP_GROUP_CHAT_RS() :type(_DEF_TCP_GROUP_CHAT_RS), groupId(-1),result(-1)
    {
    }
    PackType type;
    int groupId;
    int result;
}_STRU_TCP_GROUP_CHAT_RS;
//群聊信息
typedef struct _STRU_TCP_GROUP_INDO_RQ
{
    _STRU_TCP_GROUP_INDO_RQ() :type(_DEF_TCP_GROUP_INFO_RQ), groupId(-1), icon(-1)
    {
        memset(feeling, 0, _NAME_MAX);
        memset(name, 0, _NAME_MAX);
    }
    PackType type;
    int groupId;
    char feeling[_NAME_MAX];
    char name[_NAME_MAX];
    int icon;
}_STRU_TCP_GROUP_INDO_RQ;

typedef struct _STRU_TCP_SEND_ONOFF
{
    _STRU_TCP_SEND_ONOFF() :type(_DEF_TCP_SEND_ONOFF),isOn(false)
    {

    }
    PackType type;
    bool isOn;
}_STRU_TCP_SEND_ONOFF;

//发送文件信息
struct STRU_FILE_INFO
{
    STRU_FILE_INFO() :nType(_DEF_TCP_FILE_INFO), szFileSize(0), friendId(-1),userId(-1)
    {
        memset(szFileId, 0, _MAX_FILE_PATH);
        memset(szFileName, 0, _MAX_FILE_PATH);
    }
    PackType nType;
    int userId;
    int friendId;
    char szFileId[_MAX_FILE_PATH];
    char szFileName[_MAX_FILE_PATH];
    long long szFileSize;
};

//文件块结构体：协议头、文件块、块大小、文件的唯一标识
struct STRU_FILE_BLOCK_RQ
{
    STRU_FILE_BLOCK_RQ() :nType(_DEF_TCP_FILE_BLOCK_RQ), szFileSize(0), friendId(-1), userId(-1)
    {
        memset(szFileId, 0, _MAX_FILE_PATH);
        memset(szFileContent, 0, _MAX_FILE_CONTENT_SIZE);
    }
    int nType;
    int userId;
    int friendId;
    char szFileId[_MAX_FILE_PATH];
    char szFileContent[_MAX_FILE_CONTENT_SIZE];
    int szFileSize;
};
//文件回复，用来设置上传进度条
struct STRU_FILE_BLOCK_RS
{
    STRU_FILE_BLOCK_RS() :nType(_DEF_TCP_FILE_BLOCK_RS), szPos(0), friendId(-1)
    {
        memset(szFileId, 0, _MAX_FILE_PATH);
    }
    PackType nType;
    char szFileId[_MAX_FILE_PATH];
    int friendId;
    int szPos;
};

//文件信息（本地需要的文件信息）:文件标志id，文件名，文件路径，当前位置，总大小，文件指针
struct Filelfo
{
    Filelfo():nPos(0),nFileSize(0),pFile(nullptr)
    {
        memset(szFileId, 0, _MAX_FILE_PATH);
        memset(szFileNAME, 0, _MAX_FILE_PATH);
        memset(szFilePATH, 0, _MAX_FILE_PATH);
    }

    char szFileId[_MAX_FILE_PATH];
    char szFileNAME[_MAX_FILE_PATH];
    char szFilePATH[_MAX_FILE_PATH];
    long long nPos; //当前在什么位置
    long long nFileSize; //总文件大小
    FILE* pFile; //文件指针
};
//文件重传请求
typedef struct STRU_FILE_RESEND
{
    STRU_FILE_RESEND() :nType(_DEF_TCP_FILE_RESEND), userId(-1), friendId(-1)
    {
        memset(szFileNAME, 0, _MAX_FILE_PATH);
    }
    PackType nType;
    int userId;
    int friendId;
    char szFileNAME[_MAX_FILE_PATH];
}STRU_FILE_RESEND;
//文件查看请求
typedef struct STRU_FILE_CHECK_RQ
{
    STRU_FILE_CHECK_RQ():nType(_DEF_TCP_FILE_CHECK_RQ), userId(-1), friendId(-1)
    {

    }
    PackType nType;
    int userId;    //需要接受文件信息的用户Id
    int friendId;   //已经完成文件上传的用户id
}STRU_FILE_CHECK_RQ;
//文件查看回复
typedef struct STRU_FILE_CHECK_RS
{
    STRU_FILE_CHECK_RS():nType(_DEF_TCP_FILE_CHECK_RS), fileSize(0), userId(-1)
    {
        memset(fileName, 0, _MAX_FILE_PATH);
    }
    PackType nType;
    int userId;
    char fileName[_MAX_FILE_PATH];
    long long fileSize;
}STRU_FILE_CHECK_RS;
