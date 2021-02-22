#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include <QTcpSocket>

class server : public QTcpServer
{
    Q_OBJECT
public:
    server();
    ~server();

    void startServer();       // Start listening incoming connections

public slots:
    void slotNewConnection(); // New pending connection
    void slotReadyRead();     // Client socket ready to read
    void slotDisconnected();    // Client disconnected

private:
    QTcpSocket* socket;
};

#endif // SERVER_H
