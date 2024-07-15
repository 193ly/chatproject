#include "sendfilethread.h"

SendFileThread::SendFileThread(Ckernel *ckerenl):m_quit(false)
{
    m_ckernel = ckerenl;
}
#include<QDebug>
SendFileThread::~SendFileThread()
{
    m_quit = true;

    wait();
    qDebug() << "成功退出发送文件的线程";
}

void SendFileThread::run()
{
    m_ckernel->sendFile(m_path, m_id, &m_quit);
}

void SendFileThread::setInfo(QString path, int id)
{
    m_path = path;
    m_id = id;
}
