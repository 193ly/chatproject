#include"TcpNetSever.h"
#include<process.h>
#include"../INETMEDIATOR/MidTcpNetSever.h"
TcpNetSever::TcpNetSever(MidINet* p) :sockp(false), m_sock(0), handle(nullptr)
{
	m_MidINet = p;
}

TcpNetSever::~TcpNetSever()
{
	UnInitNet();
}
//加载库 创建套接字 绑定IP 监听 线程接受连接
bool TcpNetSever::InitNet()
{
	//1.加载库
	WORD word = MAKEWORD(2, 2);
	WSADATA date;
	int err = WSAStartup(word, &date);
	if (0 != err)
	{

		WSACleanup();
		return false;

	}


	if (2 != HIBYTE(date.wHighVersion) || 2 != LOBYTE(date.wVersion))
	{
		WSACleanup();
		return false;
	}
	else
	{
		cout << "WSAStartup sucess" << endl;
	}
	//2.创建套接字
	m_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (INVALID_SOCKET == m_sock)
	{
		cout << WSAGetLastError() << endl;
		WSACleanup();
		return false;
	}
	else
	{
		cout << "sock sucess" << endl;
	}

	//3.绑定IP
	sockaddr_in addr;
	addr.sin_family = AF_INET;
    addr.sin_port = htons(DEFINE_TCP_PORT); //转换为大端
	addr.sin_addr.S_un.S_addr = INADDR_ANY;  //所有的IP
	err = bind(m_sock, (sockaddr*)&addr, sizeof(addr));
	if (SOCKET_ERROR == err)
	{
		cout << WSAGetLastError() << endl;
		WSACleanup();
		return false;
	}
	else
	{
		cout << "bind sucess" << endl;
	}
	// 4.监听
	err = listen(m_sock, 10);
	if (err == SOCKET_ERROR)
	{
		cout << "listen error" << WSAGetLastError() << endl;
		closesocket(m_sock);
		WSACleanup();
		return false;
	}
	else
	{
		cout << "listen sucess" << endl;

	}
	//5.连接

	HANDLE handle = (HANDLE)_beginthreadex
    (0, 0, &TcpNetSever::AcceptThread, (void*)this, 0, nullptr);
	m_lisHandle.push_back(handle);
	return true;
}

bool TcpNetSever::SendData(long ISend, char* sendBuf, int nLen)
{
	//1.参数是否合法
	if (!sendBuf || nLen <= 0)
	{
		cout << "TcpNetSever::SendData 参数 error" << endl;
		return false;
	}
	//先发包大小
	if (send(ISend, (char*)&nLen, sizeof(int), 0) <= 0)
	{
		cout << "TcpNetSever::SendData len error" << endl;
		return false;
	}
	//再发数据

	if (send(ISend, sendBuf, nLen, 0) <= 0)
	{
		cout << "TcpNetSever::SendData data error" << endl;
		return false;
	}
	cout << "TcpNetClient::RecvData" << endl;
	return true;
}

unsigned TcpNetSever::AcceptThread(void* lpvoid)
{
	TcpNetSever* p = (TcpNetSever*)lpvoid;
	sockaddr_in addrClient;
	HANDLE handle;
	int addrClientSize = sizeof(addrClient);
	int recvNum = 0;
	int sendNum = 0;
	unsigned int threadId = 0;
	SOCKET sock;
	while (!p->sockp)
	{
		sock = accept(p->m_sock, (sockaddr*)&addrClient, &addrClientSize);
		if (INVALID_SOCKET != sock)
		{//连接成功
			cout << "accept success " << inet_ntoa(addrClient.sin_addr) << endl;
			//创建接收数据的线程	
			handle = handle = (HANDLE)_beginthreadex
            (0, 0, &RecvThread, (void*)p, 0, &threadId);

			if (handle == NULL)
			{
				cout << "creat handle error" << endl;
				return false;
			}

			p->m_lisHandle.push_back(handle);
			//p->m_sock = sock;
			p->m_mapThreadIdSock[threadId] = sock;

		}
		else
		{
			//连接失败
			cout << "accept error" << WSAGetLastError() << endl;
			break;
		}

	}
	return 1;

}

unsigned TcpNetSever::RecvThread(void* lpvoid)
{
	TcpNetSever* p = (TcpNetSever*)lpvoid;
	p->RecvData();
	return 1;
}

void TcpNetSever::RecvData()
{
	//存入sock到map需要时间，等一等

	Sleep(50);
	//获取当前线程的sock
	SOCKET socket = m_mapThreadIdSock[GetCurrentThreadId()];
	cout << "接收成功" << endl;
	cout << "收到了数据,即将返回数据" << endl;
	if (socket == 0 || socket == INVALID_SOCKET)
	{
		cout << "Get Thread fail" << endl;
	}
	int  packSize = 0;
	int recvNum = 0;
	//累计的数据量
	int offSize = 0;
	while (!sockp)
	{
		//先接受包大小
		recvNum = recv(socket, (char*)&packSize, sizeof(int), 0);

		//接受数据
		if (recvNum > 0)
		{
			//接受包内容
			char* packBuf = new char[packSize];
			while (packSize > 0)
			{
				recvNum = recv(socket, packBuf + offSize, packSize, 0);
				if (recvNum > 0)
				{
					offSize += recvNum;
					packSize -= recvNum;
				}
				else
				{
					cout << "TcpNetSever::RecvData error:" << WSAGetLastError() << endl;
					break;
				}
			}
			//数据传给中介者类
			cout << "中介者收到数据" << endl;
			m_MidINet->DealData(socket, packBuf, offSize);
			offSize = 0;
			//cout << "服务端接收成功";
			/*cout << "TcpNetSever::SeverData" << endl;
			cout << packBuf << endl;
			SendData(socket, "TcpNetClient::RecvData", sizeof("TcpNetClient::RecvData"));*/

		}
		else
		{
			cout << "TcpNetSever::RecvData error:" << WSAGetLastError() << endl;
			break;
		}
	}

}

void TcpNetSever::UnInitNet()
{
	sockp = true;
	HANDLE handle;
	for (auto ite = m_lisHandle.begin(); ite != m_lisHandle.end();)
	{
		handle = *ite;
		if (handle)
		{
			if (WAIT_TIMEOUT == WaitForSingleObject(handle, 500))
			{
				TerminateThread(handle, -1);
			}
			CloseHandle(handle);
		}
		ite = m_lisHandle.erase(ite);
	}
	//关闭套接字
	if (m_sock && m_sock != INVALID_SOCKET)
	{
		closesocket(m_sock);
	}
	SOCKET socket;
	for (auto ite = m_mapThreadIdSock.begin(); ite != m_mapThreadIdSock.end();)
	{
		socket = ite->second;
		if (m_sock && m_sock != INVALID_SOCKET)
		{
			closesocket(m_sock);
		}
		m_mapThreadIdSock.erase(ite);

	}
	WSACleanup();
}
