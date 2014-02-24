#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QAbstractSocket>
//#include <QTimer>

class TCPClient : public QObject
{
    Q_OBJECT
public:
    explicit TCPClient(QObject *parent = 0);
    ~TCPClient();

    QString addr;
    int port;

    bool connection(QString _addr, int _port);
    void closeConnection();

    bool sendCommand(QString command);
    QString sendQuery(QString query);

    int isConnected();
    
signals:
    void sigConnected();
    void sigDisconnected();

public slots:
    void connected();
    void disconnected();

private:
    QTcpSocket *socket;
};

#endif // TCPCLIENT_H
