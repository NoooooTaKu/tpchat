#ifndef MYUDPCOM_H
#define MYUDPCOM_H

#include <QMainWindow>
#include <QUdpSocket>
#include <QDateTime>

class myUdpcom : public QMainWindow
{
    Q_OBJECT
public:
    explicit myUdpcom(QWidget *parent = 0);

signals:

public slots:

private:
    QUdpSocket *udpSocket;
    int port;
    QString receiveMsg;
    QString receiveTime;

public slots:
    void processPendingDatagrams();
    void sendMessage(QString sendMsg);
    QString getReceiveMsg(void);
    QString getReceiveTime(void);
};

#endif // MYUDPCOM_H
