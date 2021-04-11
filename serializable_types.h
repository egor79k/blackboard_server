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
    AddLayerArgs(const Vec2i &position_, const Vec2i &size_, QString tool_);

    ~AddLayerArgs();

    bool serialize(QJsonObject& json) const override;
    bool deserialize(const QJsonObject& json) override;

    QGraphicsItem *takeLayerOwnership();

private:
    void serializePencilArgs(QJsonObject& json) const;
    void deserializePencilArgs(const QJsonObject& json);

    struct SerialAdapter
    {
        void(AddLayerArgs::*serialize)(QJsonObject& json) const;
        void(AddLayerArgs::*deserialize)(const QJsonObject& json);
    };

    QMap<QString, SerialAdapter> tools_serializers {
        {"pencil", {&AddLayerArgs::serializePencilArgs, &AddLayerArgs::deserializePencilArgs}}
    };

    Vec2i position;
    Vec2i size;
    QString tool;
    QGraphicsItem *layer;
};
//=============================================================================

#endif // SERIALIZABLE_TYPES_H
