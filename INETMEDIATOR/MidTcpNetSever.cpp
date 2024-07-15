#include"MidTcpNetSever.h"
#include"../INET/TcpNetSever.h"
MidTcpNetSever::MidTcpNetSever()
{
	m_INet = new TcpNetSever(this);
}

MidTcpNetSever::~MidTcpNetSever()
{
	if (m_INet)
	{
		delete m_INet;
		m_INet = nullptr;
	}
}

bool MidTcpNetSever::InitNet()
{
	return true;
}

bool MidTcpNetSever::OpenNet()
{
	return m_INet->InitNet();
}

bool MidTcpNetSever::SendData(long ISend, char* sendBuf, int nLen)
{
	return m_INet->SendData(ISend, sendBuf, nLen);
}

void MidTcpNetSever::DealData(long ISend, char* sendBuf, int nLen)
{
	cout << "MidTcpNetSever::DealData " << sendBuf << endl;
	SendData(ISend, "MidTcpNetSever::DealData", sizeof("MidTcpNetSever::DealData"));
}

void MidTcpNetSever::UnInitNet()
{
	m_INet->UnInitNet();
}
