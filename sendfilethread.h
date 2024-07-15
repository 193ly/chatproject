#ifndef SENDFILETHREAD_H
#define SENDFILETHREAD_H

#include <QObject>
#include<QThread>
#include<QString>
#include"ckernel.h"
class Ckernel;
class SendFileThread : public QThread
{
    Q_OBJECT
public:
    SendFileThread(Ckernel* ckerenl);
    ~SendFileThread();
    void run();

    void setInfo(QString path, int id);

private:
    QString m_path;
    int m_id;
    Ckernel* m_ckernel;
    bool m_quit;
};

#endif // SENDFILETHREAD_H
