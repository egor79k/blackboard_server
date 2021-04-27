#include "serializable_types.h"


//=============================================================================
// RequestHeader
//=============================================================================
const char *RequestHeader::ClientID {"client_id"};
const char *RequestHeader::Method {"method"};
const char *RequestHeader::ArgsSize {"argument_size"};
//_____________________________________________________________________________

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
const char *InitClientArgs::ClientID {"client_id"};
//_____________________________________________________________________________

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
const char *AddLayerArgs::Position {"position"};
const char *AddLayerArgs::Size {"size"};
const char *AddLayerArgs::LayerType {"layer_type"};
const char *AddLayerArgs::LayerData {"layer_data"};
//_____________________________________________________________________________

AddLayerArgs::AddLayerArgs()
{}
//_____________________________________________________________________________

AddLayerArgs::AddLayerArgs(const Vec2i &position_, const Vec2i &size_, QString tool_) :
    position(position_),
    size(size_),
    tool(tool_)
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
    json.insert("size", QJsonArray{size.first, size.second});
    json.insert("tool", tool);
    QJsonObject parameters;
    (this->*tools_serializers[tool].serialize)(parameters);
    json.insert("parameters", parameters);
    return true;
}
//_____________________________________________________________________________

bool AddLayerArgs::deserialize(const QJsonObject& json)
{
    QJsonArray pos_arr = json["position"].toArray();
    position.first = pos_arr[0].toInt();
    position.second = pos_arr[1].toInt();

    QJsonArray size_arr = json["size"].toArray();
    size.first = size_arr[0].toInt();
    size.second = size_arr[1].toInt();

    tool = json["tool"].toString();

    (this->*tools_serializers[tool].deserialize)(json["parameters"].toObject());

    empty = false;
    return true;
}
//_____________________________________________________________________________

QGraphicsItem *AddLayerArgs::takeLayerOwnership()
{
    if (empty)
        return nullptr;

    empty = true;
    return layer;
}
//_____________________________________________________________________________

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
        pencil_item->addPoint(QPointF(pair[0].toInt(), pair[1].toInt()));
    }
}
//=============================================================================
