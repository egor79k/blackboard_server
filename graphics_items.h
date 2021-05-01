#ifndef GRAPHICS_ITEMS_H
#define GRAPHICS_ITEMS_H

#include <QGraphicsItem>
#include <QJsonArray>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QVector>

#include "serializers.h"

//=============================================================================
class LineItem : public QGraphicsLineItem, public Serializable
{
    using QGraphicsLineItem::QGraphicsLineItem;

public:
#ifdef JSON_SERIALIZER
    virtual bool deserialize(const QJsonObject& json) override;
    virtual bool serialize(QJsonObject& json) const override;
#else
    static_assert(false, "No serializer defined.");
#endif
};
//=============================================================================


//=============================================================================
class PencilItem : public QGraphicsItem
{
public:
    void addPoint(const QPointF &point);
    const QVector<QPointF> &getPoints();

    virtual QRectF boundingRect() const override;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0) override;

private:
    QVector<QPointF> points;
};
//=============================================================================

#endif // GRAPHICS_ITEMS_H
