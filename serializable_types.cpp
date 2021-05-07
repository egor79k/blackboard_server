#include "serializable_types.h"


//=============================================================================
// RequestHeader
//=============================================================================
RequestHeader::RequestHeader()
{}
//_____________________________________________________________________________

RequestHeader::RequestHeader(int client_id_, const QString &method_, int argument_size_) :
    client_id(client_id_),
    method(method_),
    argument_size(argument_size_)
{
    empty = false;
}
//_____________________________________________________________________________

#ifdef JSON_SERIALIZER
bool RequestHeader::serialize(QJsonObject& json) const
{
    if (empty)
        return false;

    json.insert("client_id", client_id);
    json.insert("method", method);
    json.insert("argument_size", argument_size);
    return true;
}
//_____________________________________________________________________________

bool RequestHeader::deserialize(const QJsonObject& json)
{
    client_id = json["client_id"].toInt();
    method = json["method"].toString();
    argument_size = json["argument_size"].toInt();
    Q_ASSERT(argument_size > 0);
    empty = false;
    return true;
}
#else
static_assert(false, "No serializer defined.");
#endif
//=============================================================================


//=============================================================================
// InitClientArgs
//=============================================================================
InitClientArgs::InitClientArgs()
{}
//_____________________________________________________________________________

InitClientArgs::InitClientArgs(int client_id_) :
    client_id(client_id_)
{
    empty = false;
}
//_____________________________________________________________________________

#ifdef JSON_SERIALIZER
bool InitClientArgs::serialize(QJsonObject& json) const
{
    if (empty)
        return false;
    
    json.insert("client_id", client_id);
    return true;
}
//_____________________________________________________________________________

bool InitClientArgs::deserialize(const QJsonObject& json)
{
    client_id = json["client_id"].toInt();
    empty = false;
    return true;
}
#else
static_assert(false, "No serializer defined.");
#endif
//=============================================================================


//=============================================================================
// AddLayerArgs
//=============================================================================
AddLayerArgs::AddLayerArgs()
{}
//_____________________________________________________________________________

AddLayerArgs::AddLayerArgs(QSharedPointer<Serializable> layer_, const int layer_id_, const QPointF &position_, const qreal &scale_, QString layer_type_) :
    layer(layer_),
    layer_id(layer_id_),
    position(position_),
    scale(scale_),
    layer_type(layer_type_)
{
    empty = false;
}
//_____________________________________________________________________________

AddLayerArgs::~AddLayerArgs()
{
    //if (!empty)
        //delete layer;
}
//_____________________________________________________________________________
/*
Serializable *AddLayerArgs::takeLayerOwnership()
{
    if (empty)
        return nullptr;

    empty = true;
    return layer;
}*/
//_____________________________________________________________________________

#ifdef JSON_SERIALIZER
bool AddLayerArgs::serialize(QJsonObject& json) const
{
    if (empty)
        return false;

    json.insert("layer_id", layer_id);
    json.insert("position", QJsonArray{position.x(), position.y()});
    json.insert("scale", scale);
    json.insert("layer_type", layer_type);
    QJsonObject layer_data;
    //(this->*tools_serializers[layer_type].serialize)(layer_data);
    layer->serialize(layer_data);
    json.insert("layer_data", layer_data);
    return true;
}
//_____________________________________________________________________________

bool AddLayerArgs::deserialize(const QJsonObject& json)
{
    layer_id = json["layer_id"].toInt();
    QJsonArray pos_arr = json["position"].toArray();
    position.rx() = pos_arr[0].toDouble();
    position.ry() = pos_arr[1].toDouble();

    scale = json["scale"].toDouble();

    layer_type = json["layer_type"].toString();

    //(this->*tools_serializers[layer_type].deserialize)(json["layer_data"].toObject());
    (this->*layer_deserialize[layer_type])(json["layer_data"].toObject());

    empty = false;
    return true;
}
//_____________________________________________________________________________

template <typename T>
void AddLayerArgs::deserializeLayerArgs(const QJsonObject& json)
{
    layer = QSharedPointer<Serializable> (new T);
    layer->deserialize(json);
/*
    T *item = new T;
    item->deserialize(json);
    layer = item;*/
}
#else
static_assert(false, "No serializer defined.");
#endif
//=============================================================================
