#include "graphics_items.h"

//=============================================================================
//
//=============================================================================
void PencilItem::addPoint(const QPointF &point)
{
    points.push_back(point);
}
//_____________________________________________________________________________

const QVector<QPointF> &PencilItem::getPoints()
{
	return points;
}
//_____________________________________________________________________________

QRectF PencilItem::boundingRect() const
{
    return QRectF{};
}
//_____________________________________________________________________________

void PencilItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->drawLines(points);
}
//=============================================================================