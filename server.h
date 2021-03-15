#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QList>

class Server : public QTcpServer
{
    Q_OBJECT
public:
    Server();
    ~Server();

    void startServer();       // Start listening incoming connections

    void createLayer();

public slots:
    void slotNewConnection(); // New pending connection
    void slotReadyRead();     // Client socket ready to read
    void slotDisconnected();  // Client disconnected

private:
    QList<QTcpSocket *> clients;
};

#endif // SERVER_H
