#ifndef CLIENTAPI_H
#define CLIENTAPI_H

#include <QObject>
#include <QTcpSocket>

#include "serializers.h"
#include "serializable_types.h"

//=============================================================================
class Client : public QObject
{
    Q_OBJECT

public:
    typedef int id_type;

    Client(QTcpSocket *socket_);

    bool operator==(const Client &other) const;

    id_type getID() const;
    QTcpSocket *getSocket() const;
    void callMethod(const char *method, const Serializer &args);

// Client API functions
//----------------------
    void initClient(const Serializer &args);  // Send init info for new client
    void finishBoardInit();                   // Called after adding all layers to new client
    void addLayer(const Serializer &args);    // Add new layer
    void confirmAddLayer(const Serializer &args); // Confirm successful adding and send new layer unique id
    void clearBoard();                        // Delete all layers
    void deleteLayer(const Serializer &args);

private:
    QTcpSocket *socket;
};
//=============================================================================

#endif // CLIENTAPI_H
