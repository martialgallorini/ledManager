#ifndef LEDBAR_H
#define LEDBAR_H

#include <QObject>
#include <tcpclient.h>

class ledBar : public QObject
{
    Q_OBJECT
public:
    explicit ledBar(QObject *parent = 0);

    void connection(QString _ip, int port);
    void closeConnection();
    bool isConnected();

    void freeze();
    void reboot();
    void run();
    void clear();
    void authorize(int bank);
    void brightness(int value);
    void getStoredMessage(int bank);

    void sendMessage(QString msg);
    bool sendQuery(QString msg);

signals:

public slots:
    void led_connected();
    void led_disconnected();

private:
    TCPClient *eplClient;
};

#endif // LEDBAR_H
