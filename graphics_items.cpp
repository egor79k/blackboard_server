#include "graphics_items.h"

void PencilItem::addPoint(const QPointF &point)
{
    points.push_back(point);
}

void PencilItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->drawLines(points);
}
