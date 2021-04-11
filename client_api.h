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
    void initClient(const Serializer& args);  // Send init info for new client
    void addLayer(const Serializer& args);    // Add new layer

private:
    QTcpSocket *socket;
};
//=============================================================================

#endif // CLIENTAPI_H
