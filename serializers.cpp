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

bool JsonSerializer::serialize(const Serializable& object)
{
    QJsonObject jsonObj;
    if (object.serialize(jsonObj)) {
        json_doc.setObject(jsonObj);
        Q_ASSERT(!json_doc.isNull());
        return true;
    }

    return false;
}
//_____________________________________________________________________________

bool JsonSerializer::deserialize(Serializable& object) const
{
    return object.deserialize(json_doc.object());
}
//_____________________________________________________________________________

QByteArray JsonSerializer::getData() const
{
    if (!json_doc.isNull()) {
        return json_doc.toJson();
    } else {
        return {};
    }
}
//_____________________________________________________________________________

bool JsonSerializer::set(const QByteArray& json)
{
    json_doc = QJsonDocument::fromJson(json, &last_error);
    return !json_doc.isNull();
}
//_____________________________________________________________________________

bool JsonSerializer::set(const QJsonObject& json)
{
    json_doc.setObject(json);
    return !json_doc.isNull();
}
//_____________________________________________________________________________

bool JsonSerializer::isNull() const
{
    return json_doc.isNull();
}
//_____________________________________________________________________________

QJsonParseError JsonSerializer::lastError() const
{
    return last_error;
}
//=============================================================================
