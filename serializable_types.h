#ifndef SERIALIZABLE_TYPES_H
#define SERIALIZABLE_TYPES_H

#include <QJsonArray>
#include <QMap>
#include <QPair>
#include <QString>

#include "graphics_items.h"
#include "serializers.h"


//=============================================================================
class RequestHeader : public Serializable
{
public:
    RequestHeader();
    RequestHeader(int client_id_, const QString &method_, int argument_size_);

    bool serialize(QJsonObject& json) const override;
    bool deserialize(const QJsonObject& json) override;

    int client_id;
    QString method;
    int argument_size;
};
//=============================================================================


//=============================================================================
class InitClientArgs : public Serializable
{
public:
    InitClientArgs();
    InitClientArgs(int client_id_);

    bool serialize(QJsonObject& json) const override;
    bool deserialize(const QJsonObject& json) override;

private:
    int client_id;
};
//=============================================================================


//=============================================================================
class AddLayerArgs : public Serializable
{
public:
    typedef QPair<int, int> Vec2i;

    AddLayerArgs();
    AddLayerArgs(const Vec2i &position_, const qreal &scale_, QString layer_type_);

    ~AddLayerArgs();

    bool serialize(QJsonObject& json) const override;
    bool deserialize(const QJsonObject& json) override;

    Serializable *takeLayerOwnership();

private:
    //void serializePencilArgs(QJsonObject& json) const;
    //void deserializePencilArgs(const QJsonObject& json);

    void deserializeLineArgs(const QJsonObject& json);
/*
    struct SerialAdapter
    {
        void(AddLayerArgs::*serialize)(QJsonObject& json) const;
        void(AddLayerArgs::*deserialize)(const QJsonObject& json);
    };
*/
    QMap<QString, void(AddLayerArgs::*)(const QJsonObject& json)> tool_deserialize {
        //{"pencil", {&AddLayerArgs::serializePencilArgs, &AddLayerArgs::deserializePencilArgs}}
        {"line",  &AddLayerArgs::deserializeLineArgs}
    };

    Vec2i position;
    qreal scale;
    QString layer_type;
    Serializable *layer;
};
//=============================================================================

#endif // SERIALIZABLE_TYPES_H
