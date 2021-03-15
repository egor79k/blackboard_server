#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QList>

#include "parser.h"

struct SApiArgs
{};

struct AddLayerArgs : public SApiArgs
{};


class Server : public QTcpServer
{
    Q_OBJECT
public:
    Server(Parser *parser_);
    ~Server();

    void startServer();       // Start listening incoming connections

    void addLayer(SApiArgs *args);
    void wrongRequest(SApiArgs *args);

public slots:
    void slotNewConnection(); // New pending connection
    void slotReadyRead();     // Client socket ready to read
    void slotDisconnected();  // Client disconnected

private:
    QList<QTcpSocket *> clients;
    Parser *parser;
};

#endif // SERVER_H
