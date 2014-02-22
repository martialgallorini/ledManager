#include "tcpclient.h"
#include <QDebug>


TCPClient::TCPClient(QObject *parent) :
    QObject(parent)
{
    socket = new QTcpSocket(this);
    connect(socket, SIGNAL(connected()), this, SLOT(connected()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(disconnected()));
//    connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()));
//    connect(socket, SIGNAL(bytesWritten(qint64)), this, SLOT(bytesWritten(qint64)));

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

bool TCPClient::sendCommand(QString command)
{
    if (socket->state() != QAbstractSocket::ConnectedState)
    {
        qWarning() << "Couldn't send command : not connected !";
        return false;
    }
    if(socket->write(command.toLatin1()))
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

QString TCPClient::sendQuery(QString query)
{
    if (socket->state() != QAbstractSocket::ConnectedState)
    {
        qWarning() << "Can't send command : not connected !";
    }
    else
    {
        socket->write(query.toLatin1());
        timer->start(timeout);
    }

    QString resp;
    if(socket->waitForReadyRead(2000))
    {
      qDebug() << "\nReceived " << socket->bytesAvailable() << "bytes";
      while(socket->bytesAvailable() > 0)
      {
           resp.append(socket->readLine());
      }
      qDebug() <<  "\nReading response : " << resp.toUtf8();
      qDebug() << "\n" << socket->bytesAvailable() << "bytes left to read";
      return resp.toUtf8();
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

//void TCPClient::bytesWritten(qint64 bytes)
//{
//    qDebug() << "\nSent " << bytes << "bytes";
//}

//void TCPClient::readyRead()
//{
//    if(socket->waitForReadyRead(2000))
//    {
//       timer->start(timeout);
//       qDebug() << "\nReceived " << socket->bytesAvailable() << "bytes";
//       QString resp;
//       while(socket->bytesAvailable() > 0)
//       {
//            resp.append(socket->readLine());
//       }
//       qDebug() <<  "\nReading response : " << resp.toUtf8();
//       qDebug() << "\n" << socket->bytesAvailable() << "bytes left to read";
//       emit sigTcpDataReceived(resp.toUtf8());
//    }
//    else
//    {
//        qWarning() << "Waiting for data to read timed out. No data received !";
//    }
//}

