#pragma once
#include "UdpNet.h"
#include<Windows.h>
#include"def.h"
#include<process.h>
UdpNet::UdpNet(MidINet* p):sockp(false),handle(nullptr),m_sock(0)
{
	m_MidINet = p;
}


UdpNet::~UdpNet()
{
	this->UnInitNet();
}
//初始化：加载库、套接字、绑定IP、线程接受数据
bool UdpNet::InitNet()
{
	//1.加载库
	WORD word = MAKEWORD(2, 2);
	WSADATA date;
	int err = WSAStartup(word, &date);
	if (0 != err)
	{

		WSACleanup();
		return 1;
	}

	if (2 != HIBYTE(date.wHighVersion) || 2 != LOBYTE(date.wVersion))
	{
		WSACleanup();
		return false;
	}
	//2.创建套接字
	m_sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (INVALID_SOCKET == m_sock)
	{
		cout << WSAGetLastError() << endl;
		WSACleanup();
		return false;
	}
	else
	{
		cout << "socket success " << endl;
	}
	//3.绑定IP地址

	//魔鬼数字:防止别人看不懂数字的含义，使用宏定义
	sockaddr_in addr;
	addr.sin_family = AF_INET;
    addr.sin_port = htons(DEFINE_UDP_PORT); //转换为大端
	addr.sin_addr.S_un.S_addr = INADDR_ANY;  //所有的IP
	err = bind(m_sock, (sockaddr*)&addr, sizeof(addr));
	if (SOCKET_ERROR == err)
	{
		cout<<"bind error" << WSAGetLastError() << endl;
		WSACleanup();
		closesocket(m_sock);
		return 1;
	
	}
	
	//线程
	//CreatThread 和 ExitThread 是一对， 退出线程时自动调用ExitThread，如果线程中
//使用了C++运行时库的函数（如：strcpy）
//，这个函数会自己申请空间而不回收，ExitThread也不会回收，造成了空间的浪费
	//_beginthreadex和_endthreadex是一对，_endthreadex会先回收空间，再调用ExitThread。
    handle = (HANDLE)_beginthreadex(0, 0,&UdpNet::recvThread, (void*)this, 0, nullptr);
	return true;

}
//线程函数
unsigned UdpNet::recvThread(void* lpvoid)
{
	UdpNet* p = (UdpNet*)lpvoid;
	p->RecvData();
	return 1;
}

bool UdpNet::SendData(long ISend, char* sendBuf, int nLen)
{
	sockaddr_in addr;
	addr.sin_family = AF_INET;
    addr.sin_port = htons(DEFINE_UDP_PORT); //转换为大端
	addr.sin_addr.S_un.S_addr = INADDR_ANY;  //所有的IP
	int addSize = sizeof(addr);
	addr.sin_addr.S_un.S_addr = ISend;  //指定的IP
//	if (inet_addr("255.255.255.255") == ISend)
//	{
//		bool val = true;
//		setsockopt(m_sock, SOL_SOCKET, SO_BROADCAST, (char*)val, sizeof(val));
//	}
	if (sendto(m_sock,sendBuf,nLen, 0, (sockaddr*)&addr, addSize)<= 0)
	{
		cout << "UDPNET::sendto error:" << WSAGetLastError() << endl;
		return false;

	}
	return true;
}

void UdpNet::RecvData()
{
	int nRecvNum = 0;
	char recvBuf[4096] = "";
	sockaddr_in addrClient;
	int addrClientSize = sizeof(addrClient);
	while (!sockp)
	{
		nRecvNum = recvfrom(m_sock, recvBuf, sizeof(recvBuf),
			0, (sockaddr*)&addrClient, &addrClientSize);
		if (nRecvNum > 0)
		{
			//接收数据成功
			m_MidINet->DealData(addrClient.sin_addr.S_un.S_addr, recvBuf, nRecvNum);
			cout << "ip" << inet_ntoa(addrClient.sin_addr) << ":" << recvBuf << endl;
		}
		else
		{
			cout << "UDPNET::recvfrom error:" << WSAGetLastError() << endl;
			break;
		}
		 
	}
}
//创建线程时，会给每个线程分配一个内核对象，引用计数器是2。回收内核对象，需要把计算器变为0，结束线程工作，关闭句柄。
//关闭网络:回收线程、关闭套接字、卸载库
void UdpNet::UnInitNet()
{
	sockp = true;
	if (handle)
	{
		if (WAIT_TIMEOUT == WaitForSingleObject(handle, 500))
		{
			handle = nullptr;
			WSACleanup();
			closesocket(m_sock);
		}
	}

}
