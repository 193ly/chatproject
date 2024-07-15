#pragma once
#include"def.h"
#include<set>
#include<iostream>
#include<winsock2.h>
using namespace std;
class MidINet;
class INet
{
public:
	INet();
	~INet();
	//初始化网络 
	virtual bool InitNet() = 0;

	//发送数据
	virtual bool SendData(long ISend, char* sendBuf, int nLen) = 0;



	//关闭网络
	virtual void UnInitNet() = 0;
    //获取本地IP地址列表
    static set<long> GetIpList()
    {
        set<long> ipList;
        char name[100] = "";
        //先获取主机名字
        gethostname(name,sizeof(name));
        //再根据主机名字获取ip
        struct  hostent* remoteHost = gethostbyname(name);
        int i = 0;
        long ip = 0;
        while(remoteHost->h_addr_list[i] !=0 )
        {
            ip = *(u_long*) remoteHost->h_addr_list[i++];
            ipList.insert(ip);
        }
        return ipList;

    }

protected:
	MidINet* m_MidINet;
	//接受数据
	virtual void RecvData() = 0;
};
