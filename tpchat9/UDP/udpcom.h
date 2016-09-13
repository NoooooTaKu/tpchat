#ifndef UDPCOM_H
#define UDPCOM_H

#include <QMainWindow>
#include <QTextCharFormat>
#include <QHostAddress>
#include <QUdpSocket>
#include <QNetworkInterface>
#include <QDateTime>

class Udpcom : public QMainWindow
{
    Q_OBJECT
public:
    explicit Udpcom(QWidget *parent = 0);

signals:
    void receiveBroad();

private:
    QUdpSocket *udpSocket;
    quint16 port;
    QString receiveMsg;
    QString receiveTime;

    QHostAddress senderIP;
    quint16 senderPort;

public slots:
    void processPendingDatagrams();

    void sendBroadMsg(QString sendMsg);

    void initSingleMsg(QHostAddress receive_ip);
    void sendSingleMsg(QString sendMsg, QHostAddress sendtoip);

    QString getIP(void);
    QString getReceiveMsg(void);
    QString getReceiveTime(void);
    QHostAddress getSenderIP(void);
    quint16 getSenderPort(void);
};

#endif // UDPCOM_H
