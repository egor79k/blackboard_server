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
//=============================================================================


//=============================================================================
// AddLayerArgs
//=============================================================================
AddLayerArgs::AddLayerArgs()
{}
//_____________________________________________________________________________

AddLayerArgs::AddLayerArgs(const Vec2i &position_, const qreal &scale_, QString layer_type_) :
    position(position_),
    scale(scale_),
    layer_type(layer_type_)
{
    empty = false;
}
//_____________________________________________________________________________

AddLayerArgs::~AddLayerArgs()
{
    if (!empty)
        delete layer;
}
//_____________________________________________________________________________

bool AddLayerArgs::serialize(QJsonObject& json) const
{
    if (empty)
        return false;

    json.insert("position", QJsonArray{position.first, position.second});
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
    QJsonArray pos_arr = json["position"].toArray();
    position.first = pos_arr[0].toInt();
    position.second = pos_arr[1].toInt();

    scale = json["scale"].toDouble();

    layer_type = json["layer_type"].toString();

    //(this->*tools_serializers[layer_type].deserialize)(json["layer_data"].toObject());
    (this->*tool_deserialize[layer_type])(json["layer_data"].toObject());

    empty = false;
    return true;
}
//_____________________________________________________________________________

Serializable *AddLayerArgs::takeLayerOwnership()
{
    if (empty)
        return nullptr;

    empty = true;
    return layer;
}
//_____________________________________________________________________________
/*
void AddLayerArgs::serializePencilArgs(QJsonObject& json) const
{
    QJsonArray points_arr;
    for (auto point: reinterpret_cast<PencilItem *>(layer)->getPoints())
    {
        points_arr.push_back(QJsonArray{point.x(), point.y()});
    }

    json.insert("coordinates", points_arr);
}
//_____________________________________________________________________________

void AddLayerArgs::deserializePencilArgs(const QJsonObject& json)
{
    QJsonArray points_arr = json["coordinates"].toArray();

    PencilItem *pencil_item = new PencilItem;
    layer = pencil_item;
    for (auto point : points_arr)
    {
        QJsonArray pair = point.toArray();
        pencil_item->addPoint(QPointF(pair[0].toDouble(), pair[1].toDouble()));
    }
}*/
//_____________________________________________________________________________

void AddLayerArgs::deserializeLineArgs(const QJsonObject& json)
{

    LineItem *line_item = new LineItem;
    line_item->deserialize(json);
    layer = line_item;
}
//=============================================================================
