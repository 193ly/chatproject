#include"MidINet.h"
class MidTcpNetSever :public MidINet
{
public:
	MidTcpNetSever();
	~MidTcpNetSever();
	//��ʼ������ 
	virtual bool InitNet();

	virtual bool OpenNet();

	//��������
	virtual bool SendData(long ISend, char* sendBuf, int nLen);

	//��������
	virtual void DealData(long ISend, char* sendBuf, int nLen);

	//�ر�����
	virtual void UnInitNet();
};