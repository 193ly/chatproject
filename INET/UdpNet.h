#pragma once
#include"INet.h"
#include"Winsock2.h"
#include"../INETMEDIATOR/MidINet.h"
#include"../INETMEDIATOR/MidUdpNet.h"
#include<iostream>
using namespace std;
#pragma comment(lib,"Ws2_32.lib")
class UdpNet:public INet
{

public:


	UdpNet(MidINet*);
	~UdpNet();
	//初始化网络 
	virtual bool InitNet();

	//发送数据
	virtual bool SendData(long ISend, char* sendBuf, int nLen);



	//关闭网络
	virtual void UnInitNet();

	//线程函数
private:
	SOCKET m_sock;
	bool sockp;
	HANDLE handle;
    static unsigned __stdcall recvThread(void* lpvoid);
protected:
	//接受数据
	virtual void RecvData();
	MidINet* m_MidINet;
};
