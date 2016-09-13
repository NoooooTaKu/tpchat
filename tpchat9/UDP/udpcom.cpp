#include "udpcom.h"

Udpcom::Udpcom(QWidget *parent) :
    QMainWindow(parent)
{
    udpSocket = new QUdpSocket(this);
    port = 8888;
    udpSocket->bind(port, QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint);
    connect(udpSocket, SIGNAL(readyRead()), this, SLOT(processPendingDatagrams()));
}

void Udpcom::sendBroadMsg(QString sendMsg)
{
    QByteArray data;
    QDataStream out(&data, QIODevice::WriteOnly);
    out << sendMsg;
    udpSocket->writeDatagram(data, data.length(), QHostAddress::Broadcast, port);
}

void Udpcom::initSingleMsg(QHostAddress receive_ip)
{
    port = 8888;
    udpSocket->bind(receive_ip, port, QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint);
    connect(udpSocket, SIGNAL(readyRead()), this, SLOT(processPendingDatagrams()));
}

void Udpcom::sendSingleMsg(QString sendMsg, QHostAddress sendtoip)
{
    QByteArray data;
    QDataStream out(&data, QIODevice::WriteOnly);
    out << sendMsg;
    udpSocket->writeDatagram(data, data.length(), sendtoip, port);
}

void Udpcom::processPendingDatagrams()
{
    while(udpSocket->hasPendingDatagrams())
    {
        QByteArray datagram;
        datagram.resize(udpSocket->pendingDatagramSize());

        udpSocket->readDatagram(datagram.data(), datagram.size(), &this->senderIP, &this->senderPort);

        QDataStream in(&datagram, QIODevice::ReadOnly);

        this->receiveTime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
        in >> receiveMsg;
    }
    if (this->senderIP.toString() == getIP())
    {
        return ;
    }
    else
    {
        emit receiveBroad();
    }
    //qDebug(QByteArray(receiveMsg.toLatin1()).data());
}

QString Udpcom::getIP(void)
{
    QList<QHostAddress> list = QNetworkInterface::allAddresses();
    foreach (QHostAddress address, list) {
        if(address.protocol() == QAbstractSocket::IPv4Protocol)
            return address.toString();
    }
    return 0;
}

QString Udpcom::getReceiveMsg(void)
{
    return this->receiveMsg;
}

QString Udpcom::getReceiveTime(void)
{
    return this->receiveTime;
}

QHostAddress Udpcom::getSenderIP(void)
{
    return this->senderIP;
}

quint16 Udpcom::getSenderPort(void)
{
    return this->senderPort;
}
