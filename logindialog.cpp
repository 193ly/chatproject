#include "logindialog.h"
#include "ui_logindialog.h"
#include<QMessageBox>
LoginDialog::LoginDialog(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LoginDialog)
{
    ui->setupUi(this);
}

LoginDialog::~LoginDialog()
{
    delete ui;
}

void LoginDialog::closeEvent(QCloseEvent *event)
{
    //忽略关闭事件
    event->ignore();
    //弹出提示确认是否关闭
    if(QMessageBox::Yes == QMessageBox::question(this,"Yes","确认关闭？"))
    {
        // 通知kerenl，发送下线广播
        Q_EMIT SIG_closeLog();
    }
}

void LoginDialog::on_pb_submit_clicked()
{
    QString tel = ui->le_tel->text();
    QString password = ui->le_password->text();
    QString telTemp = tel;
    QString passwordTemp = password;
    //检验内容是否为空
    if(tel.isEmpty()||password.isEmpty()||
             telTemp.remove(" ").isEmpty()|| passwordTemp.remove(" ").isEmpty())
    {
        QMessageBox::about(this,"message","input error");
        return;
    }
    //检验长度
    if(tel.length()>11||password.length()>15)
    {
        QMessageBox::about(this,"message","length error");
        return;
    }
    //检验内容包含是否有误

    //
    Q_EMIT SIG_logic(tel,password);
}

void LoginDialog::on_pb_clear_clicked()
{
    ui->le_password->setText("");
    ui->le_tel->setText("");
}

void LoginDialog::on_pb_submit_resigter_clicked()
{
    QString name = ui->le_name_resigter->text();
    QString tel = ui->le_tel_resigter->text();
    QString password = ui->le_password_resigter->text();
    QString nameTemp = name;
    QString telTemp = tel;
    QString passwordTemp = password;
    //检验内容是否为空
    if(name.isEmpty()||tel.isEmpty()||password.isEmpty()||
            nameTemp.remove(" ").isEmpty()|| telTemp.remove(" ").isEmpty()|| passwordTemp.remove(" ").isEmpty())
    {
        QMessageBox::about(this,"message","input error");
        return;
    }
    //检验长度
    if(name.length()>10||tel.length()>11||password.length()>15)
    {
        QMessageBox::about(this,"message","length error");
        return;
    }
    //检验内容包含是否有误

    //发送到krenel
    Q_EMIT SIG_registerSubmit( name, tel, password);

}

void LoginDialog::on_pb_clear_resigter_clicked()
{
    ui->le_password_resigter->setText("");
    ui->le_tel_resigter->setText("");
    ui->le_name_resigter->setText("");
}
