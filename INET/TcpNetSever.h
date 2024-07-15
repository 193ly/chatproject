#pragma once
#include"INet.h"
#include"Winsock2.h"
#include<iostream>
#include<list>
#include<map>
using namespace std;
#pragma comment(lib,"Ws2_32.lib")
class TcpNetSever :public INet
{

public:
	TcpNetSever(MidINet *);
	~TcpNetSever();
	//初始化网络 
	virtual bool InitNet();

	//发送数据(ISend在UDP表示IP，TCP中表示套接字)
	virtual bool SendData(long ISend, char* sendBuf, int nLen);

protected:

	virtual void RecvData();



	//关闭网络
	virtual void UnInitNet();
private:
	bool sockp;
	HANDLE handle;
	SOCKET m_sock;
    static unsigned __stdcall AcceptThread(void* lpvoid);
    static unsigned __stdcall RecvThread(void* lpvoid); //接收数据的线程
    list<HANDLE> m_lisHandle;
	map<unsigned int, SOCKET> m_mapThreadIdSock;
	//数据结构
	//数组：空间连续，数组下标连续，插入删除不方便，在已知下标的前提下查找方便，否则查找慢
	//链表：空间不连续，插入删除方便，查找慢
	//map：key与value一对一关系，查找快，
	//set：key与value相等，查找快
	//栈：先进先出
	//队列：先进后出	
};
