#pragma once
#include"MidINet.h"
#include"../INET/INet.h"
class MidTcpNetClient :public MidINet
{
    Q_OBJECT
public:
	MidTcpNetClient();
	~MidTcpNetClient();
	//初始化网络 
	virtual bool InitNet();

	virtual bool OpenNet();
	//发送数据
	virtual bool SendData(long ISend, char* sendBuf, int nLen);

	//接受数据
	virtual void DealData(long ISend, char* sendBuf, int nLen);

	//关闭网络
	virtual void UnInitNet();
signals:
    //数据发给ckerenl
    void SIG_ReadyDate(long ISend, char* sendBuf, int nLen);

public:
    MyAES* aes = nullptr;

};
