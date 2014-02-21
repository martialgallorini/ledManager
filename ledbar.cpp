#include "ledbar.h"

ledBar::ledBar(QObject *parent) :
    QObject(parent)
{
    eplClient = new TCPClient(this);
    connect(eplClient, SIGNAL(sigConnected()), this, SLOT(led_connected()));
    connect(eplClient, SIGNAL(sigDisconnected()), this, SLOT(led_disconnected()));
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

void ledBar::getStoredMessage(int bank)
{
//    QString message;
//    QByteArray msg;

    QByteArray message;

    if (bank < 10)
    {
        message.append("COP0" + QString::number(bank) + "\r\n");
    }
    else
    {
        message.append("COP" + QString::number(bank) + "\r\n");
    }
//    msg.append(message.toLatin1());
    sendQuery(message);
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
    QByteArray text;
    text.append(msg.toLatin1());
    eplClient->sendCommand(text);
}

bool ledBar::sendQuery(QString msg)
{
    QByteArray text;
    text.append(msg.toLatin1());
    return eplClient->sendQuery(text);
}

void ledBar::authorize(int bank)
{
        sendMessage("AUT " + QString::number(bank) + "\r\n");
}


// ***************** SIGNALS ********************** //

void ledBar::led_connected()
{
    qDebug() << "Led bar connected !";
}

void ledBar::led_disconnected()
{
    qDebug() << "Led bar disconnected !";
}
