#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QSet>

#include "logger.h"

class server : public QTcpServer
{
    Q_OBJECT
public:
    server();
    ~server();

    void setLogger(Logger *_log);
    void startListening(const QHostAddress &address = QHostAddress::Any, quint16 port = 0);       // Start listening incoming connections
    void stopListening();

public slots:
    void slotNewConnection(); // New pending connection
    void slotReadyRead();     // Client socket ready to read
    void slotDisconnected();    // Client disconnected

private:
    QSet<QTcpSocket *> clients;
    Logger *log;
};

#endif // SERVER_H
