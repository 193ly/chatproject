#ifndef FILESHOWDIALOG_H
#define FILESHOWDIALOG_H

#include <QWidget>
#include<QString>

namespace Ui {
class FileShowDialog;
}

class FileShowDialog : public QWidget
{
    Q_OBJECT

public:
    explicit FileShowDialog(QWidget *parent = 0);
    ~FileShowDialog();
//    void eventFilter(QObject *watched, QEvent *event);
    void setNameAndSize(QString name, long long size);
    void setProcessBar(int value);
signals:
    void SIG_resendFile(QString);
private slots:
    void on_pushButton_clicked();

private:
    Ui::FileShowDialog *ui;

};

#endif // FILESHOWDIALOG_H
