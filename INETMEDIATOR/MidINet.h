#pragma once
#include"../INET/INet.h"
#include<QObject>
#include"./mycrypt/myaes.h"
#include"./mycrypt/myrsa.h"
class INet;
class MidINet:public  QObject
{
Q_OBJECT
public:
	MidINet();
	~MidINet();
	//初始化网络 
	virtual bool InitNet() ;

	virtual bool OpenNet() = 0;

	//发送数据
	virtual bool SendData(long ISend, char* sendBuf, int nLen) = 0;

	//接受数据
	virtual void DealData(long ISend, char* sendBuf, int nLen) = 0;

	//关闭网络
	virtual void UnInitNet() = 0;

    MyAES* aes = nullptr;
protected:
	INet* m_INet;
};
