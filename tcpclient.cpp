#include "tcpclient.h"
#include <QDebug>


TCPClient::TCPClient(QObject *parent) :
    QObject(parent)
{
    socket = new QTcpSocket(this);
    connect(socket, SIGNAL(connected()), this, SLOT(connected()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(disconnected()));
}

TCPClient::~TCPClient()
{
    if (socket->state() == QAbstractSocket::ConnectedState)
    {
        socket->abort();
    }
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

bool TCPClient::sendCommand(QString command)
{
    if (socket->state() != QAbstractSocket::ConnectedState)
    {
        qWarning() << "Couldn't send command : not connected !";
        return false;
    }
    if(socket->write(command.toLatin1()))
    {
        if(socket->waitForBytesWritten(2000) && socket->waitForReadyRead((2000)))
        {
            socket->readAll();
        }
    }
    return true;
}

QString TCPClient::sendQuery(QString query)
{
    if (socket->state() != QAbstractSocket::ConnectedState)
    {
        qWarning() << "Can't send command : not connected !";
    }
    else
    {
        while (socket->bytesAvailable() > 0) // purge socket buffer
        {
            socket->readAll();
        }
        socket->write(query.toLatin1());
    }

    QString resp;
    if(socket->waitForBytesWritten(2000) && socket->waitForReadyRead(2000))
    {
      while(socket->bytesAvailable() > 0 || !resp.contains("@CV"))
      {
           resp.append(QString::fromLatin1(socket->readLine()));
      }
      return resp;
    }
    else
    {
           qWarning() << "Waiting for data to read timed out. Nothing to read !";
    }
    return QString();
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
    if(socket->waitForReadyRead((2000)))
    {
        while (socket->bytesAvailable() > 0) //purge socket buffer
        {
            socket->readAll();
        }
    }
    emit sigConnected();
}

void TCPClient::disconnected()
{
    emit sigDisconnected();
}
