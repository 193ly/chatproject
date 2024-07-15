#pragma once
#include"MidINet.h"
#include "../INET/UdpNet.h"

class MidUdpNet :public MidINet
{
    Q_OBJECT
public:
	MidUdpNet();
	~MidUdpNet();
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
    void SIG_ReadyDate(long ISend, char* sendBuf, int nLen);
};
