#include"TcpNetClient.h"
#include"../INETMEDIATOR/MidTcpNetClient.h"

TcpNetClient::TcpNetClient(MidINet* p):sockp(false),m_sock(0), handle(nullptr)
{
	m_MidINet = p;
}

TcpNetClient::~TcpNetClient()
{
	UnInitNet();
}
//加载库、创建套接字、连接、创建接收数据的线程
bool TcpNetClient::InitNet()
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
		return 1;
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
		return 1;
	}
	else
	{
		cout << "sock sucess" << endl;
	}
	sockaddr_in addrSever;
	addrSever.sin_family = AF_INET;
    addrSever.sin_port = htons(DEFINE_TCP_PORT);
	addrSever.sin_addr.S_un.S_addr = inet_addr(SEVER_IP);

	//3.接收连接
	int error = connect(m_sock, (sockaddr*)&addrSever, sizeof(addrSever));
	if (SOCKET_ERROR != error)
	{//连接成功
		cout << "connect success " << inet_ntoa(addrSever.sin_addr) << endl;
	}
	else
	{
		//连接失败
		cout << "connect error" << WSAGetLastError() << endl;
	}
	//线程
    handle = (HANDLE)_beginthreadex(0, 0, &TcpNetClient::RecvThread, (void*)this, 0, nullptr);
}
unsigned TcpNetClient::RecvThread(void* lpvoid)
{
	TcpNetClient* p = (TcpNetClient*)lpvoid;
	p->RecvData();
	return 1;
}
bool TcpNetClient::SendData(long ISend, char* sendBuf, int nLen)
{
	//1.参数是否合法
	if (!sendBuf || nLen <= 0)
	{
		cout << "TcpNetClient::SendData 参数 error" << endl;
		return false;
	}
	//先发包大小
	if (send(m_sock, (char*)&nLen, sizeof(int), 0) <= 0)
	{
		cout << "TcpNetClient::SendData len error" << endl;
		return false;
	}
	//再发数据

	if (send(m_sock, sendBuf, nLen, 0) <= 0)
	{
		cout << "TcpNetClient::SendData data error" << endl;
		return false;
	}
	cout << "TcpNetClient::SendData succsee" << endl;
	return true;
}

void TcpNetClient::RecvData()
{
	//cout << "客户端接收到数据" << endl;
	int  packSize = 0;
	int recvNum = 0;
	//累计的数据量
	int offSize = 0;
	while (true)
	{
		//先接受包大小
		recvNum = recv(m_sock, (char*)&packSize, sizeof(int), 0);

		//接受数据
		if (recvNum > 0)
		{
			//接受包内容
			char* packBuf = new char[packSize];
			while (packSize > 0)
			{
				recvNum = recv(m_sock, packBuf + offSize, packSize, 0);
				if (recvNum > 0)
				{
					offSize += recvNum;
					packSize -= recvNum;
				}
				else
				{
					cout << "TcpNetClient::RecvData error:" << WSAGetLastError() << endl;
					break;
				}
			}
			//数据传给中介者
			m_MidINet->DealData(m_sock,packBuf,offSize);
			//cout << packBuf << endl;
			//累加数据清零
			offSize = 0;
		}
		else
		{
			cout << "TcpNetClient::RecvData error:" << WSAGetLastError() << endl;
			break;
		}
	}
}



void TcpNetClient::UnInitNet()
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
