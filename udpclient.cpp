#include "udpclient.h"
#include <QUdpSocket>

UDPClient::UDPClient(QObject *parent) :
    QObject(parent)
{
    udpSocket = new QUdpSocket(this);
    //udpSocket->bind(QHostAddress::LocalHost, port);
    connect(udpSocket, SIGNAL(readyRead()), this, SLOT(readyRead()));
}

bool UDPClient::bind(qint16 port)
{
    return udpSocket->bind(QHostAddress::LocalHost, port);
}

void UDPClient::readyRead()
{
    // when data comes in
    QByteArray datagram;
    datagram.resize(udpSocket->pendingDatagramSize());

    QHostAddress senderAddress;
    quint16 senderPort;

    // qint64 QUdpSocket::readDatagram(char * data, qint64 maxSize,
    //                 QHostAddress * address = 0, quint16 * port = 0)
    // Receives a datagram no larger than maxSize bytes and stores it in data.
    // The sender's host address and port is stored in *address and *port
    // (unless the pointers are 0).

    udpSocket->readDatagram(datagram.data(), datagram.size(), &senderAddress, &senderPort);

    qDebug() << "Message from: " << senderAddress.toString();
    qDebug() << "Message port: " << senderPort;
    qDebug() << "Message: " << datagram;

    emit sigUdpDataReceived(datagram);
}
