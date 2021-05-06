#ifndef LAYER_H
#define LAYER_H

#include "client_api.h"
#include "serializable_types.h"

//=============================================================================
struct Layer
{
    Layer(AddLayerArgs layer_data, Client::id_type _creator_id);

    AddLayerArgs getAddLayerArgs() const;

    QSharedPointer<Serializable> layer;
    QPointF position;
    qreal scale;
    QString layer_type;
    Client::id_type creator_id;
};
//=============================================================================

#endif // LAYER_H
