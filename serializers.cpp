#include <QJsonDocument>
#include <QJsonObject>

#include "serializers.h"

//=============================================================================
// JsonSerializer
//=============================================================================
JsonSerializer::JsonSerializer(const Serializable& object)
{
    serialize(object);
}
//_____________________________________________________________________________

JsonSerializer::JsonSerializer(const QByteArray& json)
{
    set(json);
}
//_____________________________________________________________________________

JsonSerializer::JsonSerializer(const QJsonObject& json)
{
    set(json);
}
//_____________________________________________________________________________

void JsonSerializer::serialize(const Serializable& object)
{
    QJsonObject jsonObj;
    object.serialize(jsonObj);
    json_doc.setObject(jsonObj);
}
//_____________________________________________________________________________

void JsonSerializer::deserialize(Serializable& object) const
{
    object.deserialize(json_doc.object());
}
//_____________________________________________________________________________
QByteArray JsonSerializer::getData() const
{
    return json_doc.toJson();
}
//_____________________________________________________________________________

QJsonParseError JsonSerializer::lastError() const
{
    return last_error;
}
//_____________________________________________________________________________

void JsonSerializer::set(const QByteArray& json)
{
    json_doc = QJsonDocument::fromJson(json, &last_error);
}
//_____________________________________________________________________________

void JsonSerializer::set(const QJsonObject& json)
{
    json_doc.setObject(json);
}
//=============================================================================
