#include "tcpclient.h"
#include <QDebug>
#include <QTextCodec>


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

    QTextCodec* cp1252 = QTextCodec::codecForName("cp1252");
    QByteArray eplCommand = cp1252->fromUnicode(command + "\r\n"); // send command in EPL330 compatible cp1252 encoded character
    if(socket->write(eplCommand))
    {
        QString resp;
        if(socket->waitForBytesWritten(2000)) {
            while(socket->bytesAvailable() > 0 || socket->waitForReadyRead(1000))
            {
                resp.append(QString::fromLatin1(socket->readLine()));
                if (resp.contains("@"))
                    return true;
            }
            qWarning() << "Waiting for data to read timed out. Nothing to read !";
        }
        else
        {
            qWarning() << "Problem sending command to device !";
        }
    }
    return false;
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
            socket->readLine();
        }

        socket->write(query.toLatin1() + "\r\n");

        QString resp;
        if(socket->waitForBytesWritten(2000)) {
            while(socket->bytesAvailable() > 0 || socket->waitForReadyRead(1000))
            {
                resp.append(QString::fromLatin1(socket->readLine()));
                if (resp.contains("@"))
                    return resp;
            }
        }
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
        while (socket->bytesAvailable() > 0 ) //purge socket buffer
        {
            socket->readLine();
        }
    }
    emit sigConnected();
}

void TCPClient::disconnected()
{
    emit sigDisconnected();
}
