#include "myudpcom.h"

#include <iostream>
using namespace std;

myUdpcom::myUdpcom(QWidget *parent) :
    QMainWindow(parent)
{
    udpSocket = new QUdpSocket();
    port = 8888;
    udpSocket->bind(port, QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint);
    connect(udpSocket, SIGNAL(readyRead()), this, SLOT(processPendingDatagrams()));
}

//Udpcom::~Udpcom()
//{
//    //;
//}

void myUdpcom::sendMessage(QString sendMsg)
{
    QByteArray data;
    QDataStream out(&data, QIODevice::WriteOnly);
    out << sendMsg;
    udpSocket->writeDatagram(data,data.length(),QHostAddress::Broadcast, port);
}

void myUdpcom::processPendingDatagrams()
{
    while(udpSocket->hasPendingDatagrams())
    {
        QByteArray datagram;
        datagram.resize(udpSocket->pendingDatagramSize());
        udpSocket->readDatagram(datagram.data(), datagram.size());
        QDataStream in(&datagram, QIODevice::ReadOnly);

        receiveTime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
        in >> receiveMsg;

    }
    //cout<<receiveMsg<<endl;
    qDebug(QByteArray(receiveMsg.toLatin1()).data());
}

QString myUdpcom::getReceiveMsg(void)
{
    return this->receiveMsg;
}

QString myUdpcom::getReceiveTime(void)
{
    return this->receiveTime;
}
