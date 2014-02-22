#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QAbstractSocket>
#include <QTimer>

class TCPClient : public QObject
{
    Q_OBJECT
public:
    explicit TCPClient(QObject *parent = 0);
    ~TCPClient();

//    QString dataRead;

    QString addr;
    int port;

    int timeout;

    bool connection(QString _addr, int _port);
    void closeConnection();

    bool sendCommand(QString command);
    QString sendQuery(QString query);

    int isConnected();
    
signals:
    void sigConnected();
    void sigDisconnected();
//    void sigUdpDataReceived(QByteArray data);
//    void sigTcpDataReceived(QByteArray data);

public slots:
    void connected();
    void disconnected();
    void keepAlive();
//    void readyRead();
//    void bytesWritten(qint64 bytes);

private:
    QTcpSocket *socket;
    QTimer *timer;
};

#endif // TCPCLIENT_H
