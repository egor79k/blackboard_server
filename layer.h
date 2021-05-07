#ifndef LAYER_H
#define LAYER_H

#include "client_api.h"
#include "serializable_types.h"

//=============================================================================
class Layer
{
public:
    Layer(const AddLayerArgs &layer_data, Client::id_type _creator_id);

    AddLayerArgs getAddLayerArgs() const;

    QSharedPointer<Serializable> layer;
    int layer_id;
    QPointF position;
    qreal scale;
    QString layer_type;
    Client::id_type creator_id;

private:
	static int next_free_layer_id;
};
//=============================================================================

#endif // LAYER_H
