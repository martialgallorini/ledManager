#ifndef UDPCLIENT_H
#define UDPCLIENT_H

#include <QObject>
#include <QUdpSocket>

class UDPClient : public QObject
{
    Q_OBJECT
public:
    explicit UDPClient(QObject *parent = 0);

    bool bind(qint16 port);

signals:
    void sigDataReceived(QString data);

public slots:
    void readyRead();

private:
    QUdpSocket *udpSocket;

};

#endif // UDPCLIENT_H
