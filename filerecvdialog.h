#ifndef FILERECVDIALOG_H
#define FILERECVDIALOG_H

#include <QWidget>
#include<QVBoxLayout>
#include"fileshowdialog.h"
#include<QMap>
namespace Ui {
class FileRecvDialog;
}

class FileRecvDialog : public QWidget
{
    Q_OBJECT

public:
    explicit FileRecvDialog(QWidget *parent = 0);
    ~FileRecvDialog();

    void addItem(QString fileName, long long size);
    bool haveFileWidget(QString fileName);
    void setRecvProcessBar(QString fileName, int value);
signals:
    void SIG_resend(QString fileName);
private:
    Ui::FileRecvDialog *ui;
    QVBoxLayout* m_pLayOut;
    QMap<QString, FileShowDialog*> m_mapFileNameToWidget;
};

#endif // FILERECVDIALOG_H
