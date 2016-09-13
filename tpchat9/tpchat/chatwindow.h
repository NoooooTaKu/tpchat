#ifndef CHATWINDOW_H
#define CHATWINDOW_H

#include <QMainWindow>
#include <QtWidgets>

class ChatWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit ChatWindow(QWidget *parent = 0);

    QLabel *headImage; // 对方头像
    QLabel *name;
    QTextEdit *textEdit;
    QTextEdit *inputTextEdit;
    QPushButton *btnSend;

    QString strName;
signals:

private slots:
    void sendMsg();
};

#endif // CHATWINDOW_H
