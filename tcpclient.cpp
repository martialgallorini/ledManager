#include "tcpclient.h"
#include <QDebug>


TCPClient::TCPClient(QObject *parent) :
    QObject(parent)
{
    socket = new QTcpSocket(this);
    connect(socket, SIGNAL(connected()), this, SLOT(connected()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(disconnected()));
    connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()));
    connect(socket, SIGNAL(bytesWritten(qint64)), this, SLOT(bytesWritten(qint64)));

    timer = new QTimer(this);
    timeout = 240000; // timeout in milliseconds
    connect(timer, SIGNAL(timeout()), this, SLOT(keepAlive()));
}

TCPClient::~TCPClient()
{
    if (socket->state() == QAbstractSocket::ConnectedState)
    {
        qDebug() << "closing connexion before destruction from : " << addr << " on port " << port;
        socket->abort();
    }
    qDebug() << "deleting socket...";
    delete socket;
}

bool TCPClient::connection(QString _addr, int _port)
{
    addr = _addr;
    port = _port;

    socket->connectToHost(addr, port);

    if(!socket->waitForConnected(2000))
    {
        qWarning() << "Couldn't connect to : " << addr << " on port " << port;
        return false;
    }
    return true;
}

bool TCPClient::sendCommand(QByteArray command)
{
    if (socket->state() != QAbstractSocket::ConnectedState)
    {
        qWarning() << "Couldn't send command : not connected !";
        return false;
    }
    if(socket->write(command))
    {
        qDebug() << "sending command : " << command;
        if(socket->waitForBytesWritten(2000) && socket->waitForReadyRead((2000)))
        {
            socket->readAll();
        }
    }
    timer->start(timeout);
    return true;
}

bool TCPClient::sendQuery(QByteArray query)
{
    if (socket->state() != QAbstractSocket::ConnectedState)
    {
        qWarning() << "Can't send command : not connected !";
        return false;
    }
    else
     {
      socket->write(query);
      return true;
     }
}

int TCPClient::isConnected()
{
    return socket->state();
}

void TCPClient::closeConnection()
{
    socket->abort();
}

// SLOTS ------------------

void TCPClient::connected()
{
    qDebug() << "Connected to : " << addr << " on port " << port;
    if(socket->waitForReadyRead((2000)))
    {
        socket->readAll();
    }
    timer->start(timeout);
    emit sigConnected();
}

void TCPClient::disconnected()
{
    emit sigDisconnected();
}

void TCPClient::keepAlive()
{
    sendCommand("?\n");
}

void TCPClient::bytesWritten(qint64 bytes)
{
    qDebug() << "\nSent " << bytes << "bytes";
}

void TCPClient::readyRead()
{
//    if(!socket->waitForBytesWritten(2000))
//   {
       timer->start(timeout);
       qDebug() << "\nReceived " << socket->bytesAvailable() << "bytes";
       QString resp;
       while(socket->bytesAvailable() > 0)
       {
            resp.append(socket->readLine());
       }
       qDebug() <<  "\nReading response : " << resp.toUtf8();
       qDebug() << "\n" << socket->bytesAvailable() << "bytes left to read";
//   }
}

