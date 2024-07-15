#include "chatdialog.h"
#include "ui_chatdialog.h"
#include<QTime>
#include<QMessageBox>
#include<QFileDialog>
#include"./INET/def.h"
ChatDialog::ChatDialog(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChatDialog),m_id(-1),groupid(-1)
{
    ui->setupUi(this);
    //m_pSendFile = new SendFile;
    ui->ps_sendFile->setRange(0, 100);

    m_fileRecv = new FileRecvDialog;
    connect(m_fileRecv, SIGNAL(SIG_resend(QString)), this, SLOT(slot_resend(QString)));
}

ChatDialog::~ChatDialog()
{
    delete ui;
    if(m_fileRecv)
        delete m_fileRecv;
}
void ChatDialog::slot_resend(QString fileName)
{
    emit SIG_resend(fileName, this->m_id);
}

void ChatDialog::SetChatWindow(QString ip,int id)
{
    m_ip = ip;
    m_id = id;
    setWindowTitle(QString("[%1]").arg(m_ip));
    //m_pSendFile->SetSendWindow(m_id);

}

void ChatDialog::SetChatContent(QString content)
{
        ui->tb_chat->append(QString("[%1] %2").arg(m_ip).arg(QTime::currentTime().toString("hh:mm:ss")));
        ui->tb_chat->append(content);
}

void ChatDialog::SetGroupId(int id)
{
    groupid = id;
}

void ChatDialog::SetTcpGroupChatContent(QString content,int friendId)
{
    ui->tb_chat->append(QString("[%1] %2").arg(friendId).arg(QTime::currentTime().toString("hh:mm:ss")));
    ui->tb_chat->append(content);
}

void ChatDialog::SetSendFileBar(int value)
{
      ui->ps_sendFile->setValue(value);
//    if(value >= 100)
//    {
//        //QMessageBox::information(this, "提示", "发送文件成功");
//        ui->ps_sendFile->setValue(0);
//    }
}

bool ChatDialog::haveFileWidget(QString fileName)
{
    return m_fileRecv->haveFileWidget(fileName);
}

void ChatDialog::addRecvItem(QString fileName, long long size)
{
    m_fileRecv->addItem(fileName, size);
}

void ChatDialog::setRecvProcessBar(QString fileName, int value)
{
    m_fileRecv->setRecvProcessBar(fileName, value);
}

//SendFile *ChatDialog::ShowSendFile()
//{
//    return m_pSendFile;
//}



void ChatDialog::on_pb_send_clicked()
{
    //获取纯文本
    QString content = ui->te_chat->toPlainText();
    //判断输入是否全空格或空
    if(content.isEmpty() || content.remove(" ").isEmpty())
    {
        ui->te_chat->clear();
        return;
    }
    //获取带格式的文本
    content = ui->te_chat->toPlainText();
    //把编辑窗口清空
    ui->te_chat->clear();
    //显示到浏览窗口上
    ui->tb_chat->append(QString("[me] %1").arg(QTime::currentTime().toString("hh:mm:ss")));
    ui->tb_chat->append(content);
    //发送聊天内容到kernel，
    if(groupid >= 0)
    {
        Q_EMIT SIG_SendTcpGroupContent(content,groupid);
    }
    else if(m_id <= 0)
    {
        Q_EMIT SIG_SendContent(content,m_ip);
    }
    else
    {
        Q_EMIT SIG_SendTcpContent(content,m_id);
    }

}
#include<QFile>
void ChatDialog::on_pb_tool1_clicked()
{
    //QString path = QFileDialog::getOpenFileName(this, "打开", "./", "(*.flv *.mp4)");
    QString path = QFileDialog::getOpenFileName(this, "打开文件", "./");
    QFile file(path);
    file.open(QIODevice::ReadOnly);
    int size = file.size();
    if(size > _MAX_SEND_FILE_SIZE)
    {
        file.close();
        QMessageBox::about(this, "警告", "不许传递大小超过 2G 的文件");
        return;
    }
    file.close();
    if(!path.isEmpty())
        Q_EMIT SIG_SendFile(path, m_id);
}

void ChatDialog::slot_setSendFileProcess(int id,int value)
{
    if(id == m_id)
        SetSendFileBar(value);
}


void ChatDialog::on_pb_checkFile_clicked()
{
    //发送文件查看请求
    Q_EMIT SIG_fileCheck(m_id);

    m_fileRecv->show();
}
