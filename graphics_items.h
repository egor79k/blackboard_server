#ifndef GRAPHICS_ITEMS_H
#define GRAPHICS_ITEMS_H

#include <QGraphicsItem>
#include <QJsonArray>
#include <QPainter>
#include <QPen>
#include <QStyleOptionGraphicsItem>
#include <QVector>

#include "serializers.h"

//=============================================================================
class LineItem : public QGraphicsLineItem, public Serializable
{
public:
    template<typename ...Args>
    LineItem(Args... args)
        : QGraphicsLineItem(std::forward<Args>(args)...) {
        setPen(QPen(QBrush(), 1, Qt::SolidLine, Qt::RoundCap));
    }

#ifdef JSON_SERIALIZER
    virtual bool deserialize(const QJsonObject& json) override;
    virtual bool serialize(QJsonObject& json) const override;
#else
    static_assert(false, "No serializer defined.");
#endif
};
//=============================================================================


//=============================================================================
class PencilItem : public QGraphicsPathItem, public Serializable
{
public:
    PencilItem() = default;
    PencilItem(const QPolygonF& vertices);
    PencilItem(const QPolygonF&& vertices);

    void setVertices(const QPolygonF& vertices);
    void setVertices(const QPolygonF&& vertices);
    void addVertex(const QPointF& vertex);
    void translateVertices(const QPointF& offset);

#ifdef JSON_SERIALIZER
    virtual bool deserialize(const QJsonObject& json) override;
    virtual bool serialize(QJsonObject& json) const override;
#else
    static_assert(false, "No serializer defined.");
#endif

private:
    QPen defaultPen();
    void verticesToPath();

    QPolygonF vertices;
};
//=============================================================================


//=============================================================================
class RectangleItem : public QGraphicsRectItem, public Serializable
{
public:
    template<typename ...Args>
    RectangleItem(Args... args)
        : QGraphicsRectItem(std::forward<Args>(args)...) {
        setPen(QPen(QBrush(), 1, Qt::SolidLine, Qt::RoundCap));
    }

#ifdef JSON_SERIALIZER
    virtual bool deserialize(const QJsonObject& json) override;
    virtual bool serialize(QJsonObject& json) const override;
#else
    static_assert(false, "No serializer defined.");
#endif
};
//=============================================================================


//=============================================================================
class EllipseItem : public QGraphicsEllipseItem, public Serializable
{
public:
    template<typename ...Args>
    EllipseItem(Args... args)
        : QGraphicsEllipseItem(std::forward<Args>(args)...) {
        setPen(QPen(QBrush(), 1, Qt::SolidLine, Qt::RoundCap));
    }

#ifdef JSON_SERIALIZER
    virtual bool deserialize(const QJsonObject& json) override;
    virtual bool serialize(QJsonObject& json) const override;
#else
    static_assert(false, "No serializer defined.");
#endif
};
//=============================================================================

#endif // GRAPHICS_ITEMS_H
