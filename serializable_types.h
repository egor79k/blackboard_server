#ifndef SERIALIZABLE_TYPES_H
#define SERIALIZABLE_TYPES_H

#include <QJsonArray>
#include <QPair>
#include <QString>

#include "serializers.h"


//=============================================================================
class RequestHeader : public Serializable
{
public:
    RequestHeader();
    RequestHeader(int client_id_, const QString &method_, int argument_size_);

    virtual void serialize(QJsonObject& json) const override;
    virtual void deserialize(const QJsonObject& json) override;

private:
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

    virtual void serialize(QJsonObject& json) const override;
    virtual void deserialize(const QJsonObject& json) override;

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

    virtual void serialize(QJsonObject& json) const override;
    virtual void deserialize(const QJsonObject& json) override;

private:
    Vec2i position;
    Vec2i size;
    QString tool;
};
//=============================================================================

#endif // SERIALIZABLE_TYPES_H
