#include "layer.h"

//=============================================================================
// Layer
//=============================================================================
Layer::Layer(AddLayerArgs layer_data, Client::id_type _creator_id) :
    layer(layer_data.layer),
    position(layer_data.position),
    scale(layer_data.scale),
    layer_type(layer_data.layer_type),
    creator_id(_creator_id)
{
    Q_ASSERT(layer_data.isNull() && "Attempt to create Layer from empty AddLayerArgs");
}

AddLayerArgs Layer::getAddLayerArgs() const
{
    return AddLayerArgs(layer, position, scale, layer_type);
}
//=============================================================================
