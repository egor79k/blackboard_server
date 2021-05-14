#ifndef CLIENTAPI_H
#define CLIENTAPI_H

#include <QObject>
#include <QTcpSocket>

#include "cyclic_stack.h"
#include "serializers.h"
#include "serializable_types.h"

//=============================================================================
class Client : public QObject
{
    Q_OBJECT

public:
    struct HistorySlot
    {
        enum ChangeType
        {
            ADD_LAYER,
            CHANGE_LAYER,
            DELETE_LAYER
        };

        QJsonObject layer;
        ChangeType change_type;
    };

    using HSCT = HistorySlot::ChangeType;

    typedef int id_type;

    Client(QTcpSocket *socket_);

    bool operator==(const Client &other) const;

    id_type getID() const;
    QTcpSocket *getSocket() const;
    void callMethod(const char *method, const Serializer &args);

    void saveHistory(QSharedPointer<HistorySlot> slot);
    QSharedPointer<HistorySlot> rollBackHistory();

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
    CyclicStack<QSharedPointer<HistorySlot>, 100> history;
};
//=============================================================================

#endif // CLIENTAPI_H
