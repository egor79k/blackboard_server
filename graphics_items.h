#ifndef GRAPHICS_ITEMS_H
#define GRAPHICS_ITEMS_H

#include <QGraphicsItem>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QVector>

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
