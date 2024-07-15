#include "MidUdpNet.h"


MidUdpNet::MidUdpNet()
{
	m_INet = new UdpNet(this);
}

MidUdpNet::~MidUdpNet()
{
	if (m_INet)
	{
		delete m_INet;
		m_INet = nullptr;
	}
	this->UnInitNet();
}



bool MidUdpNet::InitNet()
{
	return 0;
}

bool MidUdpNet::OpenNet()
{
	return m_INet->InitNet();

}

bool MidUdpNet::SendData(long ISend, char* sendBuf, int nLen)
{
	return m_INet->SendData(ISend, sendBuf, nLen);
}

void MidUdpNet::DealData(long ISend, char* sendBuf, int nLen)
{
	cout << "MidUdpNet::DealData" << "  " << "中介者类接收到" << endl;
    //cout << sendBuf << endl;
    Q_EMIT SIG_ReadyDate(ISend,sendBuf,nLen);
}

void MidUdpNet::UnInitNet()
{

}
