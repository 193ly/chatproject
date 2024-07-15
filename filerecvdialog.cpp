#include "filerecvdialog.h"
#include "ui_filerecvdialog.h"
#include<QDebug>
FileRecvDialog::FileRecvDialog(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FileRecvDialog)
{
    ui->setupUi(this);
    m_pLayOut = new QVBoxLayout;
    m_pLayOut->setContentsMargins(3,0,3,0);

    //设置spacing
    m_pLayOut->setSpacing(3);

    ui->widget->setLayout(m_pLayOut);
}

FileRecvDialog::~FileRecvDialog()
{
    delete ui;
    if(m_pLayOut)
    {
        delete m_pLayOut;
        m_pLayOut = nullptr;
    }
}

void FileRecvDialog::addItem(QString fileName, long long size)
{
    FileShowDialog* fileShow = new FileShowDialog;
    //绑定发送重传文件的信号
    connect(fileShow, SIGNAL(SIG_resendFile(QString)), this, SIGNAL(SIG_resend(QString)));

    fileShow->setNameAndSize(fileName, size);
    m_pLayOut->addWidget(fileShow);
    m_mapFileNameToWidget[fileName] = fileShow;
}

bool FileRecvDialog::haveFileWidget(QString fileName)
{
    if(m_mapFileNameToWidget.find(fileName) != m_mapFileNameToWidget.end())
        return true;
    else
        return false;
}

void FileRecvDialog::setRecvProcessBar(QString fileName, int value)
{
    if(m_mapFileNameToWidget.find(fileName) == m_mapFileNameToWidget.end())
    {
        qDebug() << "没有对应的窗口";
        return;
    }
    FileShowDialog* show = m_mapFileNameToWidget[fileName];
    show->setProcessBar(value);
}
