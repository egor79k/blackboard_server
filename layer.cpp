#include "layer.h"

//=============================================================================
// Layer
//=============================================================================
Layer::Layer(const AddLayerArgs &layer_data, Client::id_type _creator_id) :
    layer(layer_data.layer),
    layer_id(next_free_layer_id),
    position(layer_data.position),
    scale(layer_data.scale),
    layer_type(layer_data.layer_type),
    creator_id(_creator_id)
{
    Q_ASSERT(!layer_data.isNull() && "Attempt to create Layer from empty AddLayerArgs");
    ++next_free_layer_id;
}
//_____________________________________________________________________________

AddLayerArgs Layer::getAddLayerArgs() const
{
    return AddLayerArgs(layer, layer_id, position, scale, layer_type);
}
//_____________________________________________________________________________

int Layer::next_free_layer_id = 1; // Start value for unique layer_id generator
//=============================================================================
