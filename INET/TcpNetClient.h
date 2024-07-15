#pragma once
#include"INet.h"
#include"Winsock2.h"
#include <process.h>
#include<iostream>
#include<winsock.h>
using namespace std;
#pragma comment(lib,"Ws2_32.lib")
class TcpNetClient :public INet
{
public:
	TcpNetClient(MidINet* p);
	~TcpNetClient();
	//初始化网络 
	virtual bool InitNet();

	//发送数据
	virtual bool SendData(long ISend, char* sendBuf, int nLen);
protected:

	virtual void RecvData();
	


	//关闭网络
	virtual void UnInitNet();
private:
	bool sockp;
	HANDLE handle;
	SOCKET m_sock;
	static unsigned __stdcall RecvThread(void* lpvoid);
	
};
