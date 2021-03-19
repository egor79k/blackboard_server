#ifndef CLIENTAPI_H
#define CLIENTAPI_H

#include <QObject>
#include <QTcpSocket>

#include "serializers.h"

//=============================================================================
class Client : public QTcpSocket
{
    Q_OBJECT
public:
    Client(QTcpSocket *socket);

    void initClient(const Serializer& args);
    void addLayer(const Serializer& args);
};
//=============================================================================

#endif // CLIENTAPI_H
