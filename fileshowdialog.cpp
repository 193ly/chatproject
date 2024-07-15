#include "fileshowdialog.h"
#include "ui_fileshowdialog.h"

FileShowDialog::FileShowDialog(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FileShowDialog)
{
    ui->setupUi(this);
}

FileShowDialog::~FileShowDialog()
{
    delete ui;
}

//void FileShowDialog::eventFilter(QObject *watched, QEvent *event)
//{
//    if(event->type() == QEvent::MouseButtonPress)
//    {
//        emit SIG_resendFile(ui->lb_fileName->text());
//    }
//}

void FileShowDialog::setNameAndSize(QString name, long long size)
{
    ui->lb_fileName->setText(name);
    //转换形式
    char buff[100] = "";
    if(size < 1024)
    {
        itoa(size, buff, 10);
        ui->lb_fileSize->setText(QString(buff) + QString("B"));
    }
    else if(size < (long long)1024 * 1024)
    {
        itoa(size / (long long)1024, buff, 10);
        ui->lb_fileSize->setText(QString(buff) + QString("KB"));
    }
    else if(size < (long long)1024 * 1024 * 1024)
    {
        itoa(size / 1024 / 1024, buff, 10);
        ui->lb_fileSize->setText(QString(buff) + QString("MB"));
    }
    else if(size < (long long)1024 * 1024 * 1024 * 1024)
    {
        int front = size / 1024 / 1024 / 1024;
        int back = (size % ( (long long)1024 * 1024 * 1024)) / (double)size * ( (long long)1024 * 1024 * 1024 );
        itoa(front, buff, 10);
        char buff1[100] = "";
        itoa(back, buff1, 10);
        buff1[2] = 0;
        ui->lb_fileSize->setText(QString(buff) + QString(".") + QString(buff1) + QString("GB"));
    }
}

void FileShowDialog::setProcessBar(int value)
{
    ui->progressBar->setValue(value);
}

void FileShowDialog::on_pushButton_clicked()
{
    emit SIG_resendFile(ui->lb_fileName->text());
}
