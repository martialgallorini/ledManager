#include "ledbar.h"

ledBar::ledBar(QObject *parent) :
    QObject(parent)
{
    eplClient = new TCPClient(this);
    connect(eplClient, SIGNAL(sigConnected()), this, SLOT(led_connected()));
    connect(eplClient, SIGNAL(sigDisconnected()), this, SLOT(led_disconnected()));
    //connect(eplClient, SIGNAL(sigTcpDataReceived(QByteArray)), this, SLOT(tcpResponse(QByteArray)));
}

// ***************** METHODS ********************** //

void ledBar::connection(QString _ip, int port)
{
    eplClient->connection(_ip, port);
}

void ledBar::closeConnection()
{
    eplClient->closeConnection();
}

bool ledBar::isConnected()
{
    return eplClient->isConnected();
}

QString ledBar::getStoredMessage(int bank)
{
    QString query;

    if (bank < 10)
    {
        return sendQuery("COP0" + QString::number(bank) + "\r\n");
    }
    else
    {
        return sendQuery("COP" + QString::number(bank) + "\r\n");
    }
}

void ledBar::freeze()
{
    sendMessage("HLT\r\n");
}

void ledBar::reboot()
{
    sendMessage("DCH\r\n");
}

void ledBar::run()
{
    sendMessage("RUN\r\n");
}

void ledBar::clear()
{
    sendMessage("CLR\r\n");
}

void ledBar::brightness(int value)
{
    QString brightness;
    brightness = "SET D=" + QString::number(value) + "\r\n";
    sendMessage(brightness);
}

void ledBar::sendMessage(QString msg)
{
    eplClient->sendCommand(msg);
}

QString ledBar::sendQuery(QString msg)
{
    return eplClient->sendQuery(msg);
}

void ledBar::authorize(int bank)
{
        sendMessage("AUT " + QString::number(bank) + "\r\n");
}


// ***************** SLOTS ********************** //

void ledBar::led_connected()
{
    qDebug() << "Led bar connected !";
}

void ledBar::led_disconnected()
{
    qDebug() << "Led bar disconnected !";
}
