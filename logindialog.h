#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QWidget>
#include<QCloseEvent>
namespace Ui {
class LoginDialog;
}

class LoginDialog : public QWidget
{
    Q_OBJECT

public:
    explicit LoginDialog(QWidget *parent = 0);
    ~LoginDialog();

    //关闭应用程序
    void closeEvent(QCloseEvent *event);
private slots:
    void on_pb_submit_clicked();

    void on_pb_clear_clicked();

    void on_pb_submit_resigter_clicked();

    void on_pb_clear_resigter_clicked();
signals:
    void SIG_registerSubmit(QString name,QString tel,QString password);
    void SIG_logic(QString tel,QString password);
    void SIG_closeLog();
private:
    Ui::LoginDialog *ui;
};

#endif // LOGINDIALOG_H
