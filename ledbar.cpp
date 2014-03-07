#include "ledbar.h"

ledBar::ledBar(QObject *parent) :
    QObject(parent)
{
    eplClient = new TCPClient(this);
    connect(eplClient, SIGNAL(sigConnected()), this, SLOT(led_connected()));
    connect(eplClient, SIGNAL(sigDisconnected()), this, SLOT(led_disconnected()));
}

// ***************** METHODS ********************** //

QString ledBar::getStatus()
{
    return sendQuery("ETA\r\n");
}

QString ledBar::getCurrentMsgNumber()
{
    QString msgNum;
    QString key;
    key = "Texte en cours : ";
    msgNum = getStatus();
    msgNum.remove(0, msgNum.indexOf(key) + key.count());
    msgNum = msgNum.left(2);
    return msgNum;
}

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

void ledBar::authorizeAll()
{
    sendMessage("AUT\r\n");
}

void ledBar::blank()
{
    sendMessage("MSG00 \r\n");
    authorize(0);
    freeze();
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

void ledBar::clearBank(int bank)
{
    QString msg;
    if (bank < 10)
    {
        msg = "MSG0" + QString::number(bank) + "\r\n";
    }
    else
    {
        msg = "MSG" + QString::number(bank) + "\r\n";
    }
    sendMessage(msg);
}

void ledBar::clearSchedule(int bank)
{
    QString msg;
    if (bank < 10)
    {
        msg = "PLE0" + QString::number(bank) + "\r\n";
    }
    else
    {
        msg = "PLE" + QString::number(bank) + "\r\n";
    }
    sendMessage(msg);
}

void ledBar::setIpAddress(QString ip, QString mask, QString gateway)
{
    QString msg;
    msg = "! " + ip + mask + gateway + "\r\n";
     sendMessage(msg);
}



// ***************** SLOTS ********************** //

void ledBar::led_connected()
{
    emit sigLedConnected();
}

void ledBar::led_disconnected()
{
    emit sigLedDisconnected();
}
