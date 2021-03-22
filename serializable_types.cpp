#include "serializable_types.h"


//=============================================================================
// RequestHeader
//=============================================================================
RequestHeader::RequestHeader()
{}

RequestHeader::RequestHeader(int client_id_, const QString &method_, int argument_size_) :
    client_id(client_id_),
    method(method_),
    argument_size(argument_size_)
{}
//_____________________________________________________________________________

void RequestHeader::serialize(QJsonObject& json) const
{
    json.insert("client_id", client_id);
    json.insert("method", method);
    json.insert("argument_size", argument_size);
}
//_____________________________________________________________________________

void RequestHeader::deserialize(const QJsonObject& json)
{
    client_id = json["client_id"].toInt();
    method = json["method"].toString();
    argument_size = json["argument_size"].toInt();
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
{}
//_____________________________________________________________________________

void InitClientArgs::serialize(QJsonObject& json) const
{
    json.insert("client_id", client_id);
}
//_____________________________________________________________________________

void InitClientArgs::deserialize(const QJsonObject& json)
{
    client_id = json["client_id"].toInt();
}
//=============================================================================


//=============================================================================
// AddLayerArgs
//=============================================================================
AddLayerArgs::AddLayerArgs()
{}
//_____________________________________________________________________________

AddLayerArgs::AddLayerArgs(const Vec2i &position_, const Vec2i &size_, QString tool_) :
    position(position_),
    size(size_),
    tool(tool_)
{}
//_____________________________________________________________________________

void AddLayerArgs::serialize(QJsonObject& json) const
{
    json.insert("position", QJsonArray{position.first, position.second});
    json.insert("size", QJsonArray{size.first, size.second});
    json.insert("tool", tool);
}
//_____________________________________________________________________________

void AddLayerArgs::deserialize(const QJsonObject& json)
{
    QJsonArray pos_arr = json["position"].toArray();
    position.first = pos_arr[0].toInt();
    position.second = pos_arr[1].toInt();

    QJsonArray size_arr = json["size"].toArray();
    size.first = size_arr[0].toInt();
    size.second = size_arr[1].toInt();

    tool = json["tool"].toString();
}
//=============================================================================
