#include"MidTcpNetClient.h"
#include"../INET/TcpNetClient.h"

MidTcpNetClient::MidTcpNetClient()
{
	m_INet = new TcpNetClient(this);
}

MidTcpNetClient::~MidTcpNetClient()
{
	if (m_INet)
	{
		delete m_INet;
		m_INet = nullptr;
	}
}

bool MidTcpNetClient::InitNet()
{
	return 0;
}

bool MidTcpNetClient::OpenNet()
{
	return m_INet->InitNet();

}

bool MidTcpNetClient::SendData(long ISend, char* sendBuf, int nLen)
{
    if(!aes)
    {
        aes = new MyAES();

        //string key = aes->key;
        cout << "aes = " << aes->key<<endl << "size = " << strlen(aes->key);

        MyRSA rsa ;
        string en = rsa.rsaPubEncrypt(aes->key);
        cout << "en = " <<en << endl;
        cout << "en size = " << en.size();
        string de = rsa.rsaPriDecrypt(en);
        cout <<"de = " <<de << endl;
        if(!m_INet->SendData(ISend, (char*)en.c_str(), en.size()))
            return false;
    }
    int len = nLen % AES_GROUP_BLOCK == 0 ? nLen : (nLen / AES_GROUP_BLOCK + 1) * AES_GROUP_BLOCK;
    char* en = (char*)malloc(len);
    aes->aes_encrypt(sendBuf, en, len, aes->key);
    bool is = m_INet->SendData(ISend, en, len);
    delete[] en;
    return is;
}
//处理数据
void MidTcpNetClient::DealData(long ISend, char* sendBuf, int nLen)
{
    char* de = new char[nLen]{0};
    memset(de, 0, nLen);
    MyAES::aes_decrypt(sendBuf, de, nLen, aes->key);
    delete[] sendBuf;
    //cout << "MidTcpNetClient::DealData 开始处理数据：" << sendBuf << endl;
    Q_EMIT SIG_ReadyDate(ISend,de,nLen);
}

void MidTcpNetClient::UnInitNet()
{
	m_INet->UnInitNet(); 
}
