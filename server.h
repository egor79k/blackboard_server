#ifndef SERVER_H
#define SERVER_H

#define JSON_SERIALIZER

#include <QGraphicsScene>
#include <QJsonDocument>
#include <QJsonObject>
#include <QList>
#include <QMap>
#include <QSharedPointer>
#include <QString>
#include <QTcpServer>
#include <QTcpSocket>

#include "client_api.h"
#include "serializers.h"
#include "serializable_types.h"


//=============================================================================
class Server : public QTcpServer
{
    Q_OBJECT
public:
    Server();
    ~Server();

    void startServer();       // Start listening incoming connections

// Server API functions
//----------------------
    void addLayer(const Serializer &args);      // Add new layer to layers_list
    void wrongRequest(const Serializer &args);  // Unknown request

public slots:
    void slotNewConnection(); // New pending connection
    void slotReadyRead();     // Client socket ready to read
    void slotDisconnected();  // Client disconnected

private:
    QMap<QString, void(Server::*)(const Serializer &)> api_func = {
        {"s_add_layer", &Server::addLayer}
};
    QList<QSharedPointer<Client>> clients;
    Client::id_type curr_sender_id;
    //QGraphicsScene scene;
};
//=============================================================================

#endif // SERVER_H
