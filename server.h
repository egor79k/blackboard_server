#ifndef SERVER_H
#define SERVER_H

#include <QGraphicsScene>
#include <QHostAddress>
#include <QJsonDocument>
#include <QJsonObject>
#include <QList>
#include <QMap>
#include <QSharedPointer>
#include <QString>
#include <QTcpServer>
#include <QTcpSocket>

#include "client_api.h"
#include "cyclic_stack.h"
#include "layer.h"
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
    void addLayer(const Serializer &args);     // Add new layer to layers_list
    void clearBoard(const Serializer &args);   // Delete all layers
    void deleteLayer(const Serializer &args);
    void undo(const Serializer &args);         // Cancel last changes

    void saveHistory(QSharedPointer<Layer> layer);

public slots:
    void slotNewConnection(); // New pending connection
    void slotReadyRead();     // Client socket ready to read
    void slotDisconnected();  // Client disconnected

private:
    QMap<QString, void(Server::*)(const Serializer &)> api_func = {
        {"s_add_layer",    &Server::addLayer},
        {"s_clear_board",  &Server::clearBoard},
        {"s_delete_layer", &Server::deleteLayer}
};
    QList<QSharedPointer<Client>> clients;
    QList<QSharedPointer<Layer>> scene;
    Client::id_type curr_sender_id;
    CyclicStack<QSharedPointer<QJsonObject>, 16> history;
};
//=============================================================================

#endif // SERVER_H
