#ifndef SERVER_H
#define SERVER_H

#include <QGraphicsScene>
#include <QHostAddress>
#include <QJsonDocument>
#include <QJsonObject>
#include <QByteArray>
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
#include "packagereadmanager.h"


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

    void saveHistory(QSharedPointer<Layer> layer, Client::HSCT change_type);

public slots:
    void slotNewConnection(); // New pending connection
    void slotPackageReceived(QTcpSocket* socket, const QJsonObject& header,
                             const QByteArray& argument);
    void slotDisconnected();  // Client disconnected

private:
    QMap<QString, void(Server::*)(const Serializer &)> api_func = {
        {"s_add_layer",    &Server::addLayer},
        {"s_clear_board",  &Server::clearBoard},
        {"s_delete_layer", &Server::deleteLayer},
        {"s_undo",         &Server::undo}
};
    QList<QSharedPointer<Client>> clients;
    QList<QSharedPointer<Layer>> scene;
    Client::id_type curr_sender_id;
    PackageReadManager read_manager;
};
//=============================================================================

#endif // SERVER_H
