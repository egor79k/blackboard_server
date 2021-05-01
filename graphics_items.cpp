#include "graphics_items.h"


//=============================================================================
// PencilItem
//=============================================================================
#ifdef JSON_SERIALIZER
bool LineItem::deserialize(const QJsonObject& json)
{
    Q_ASSERT(!json.isEmpty());

    QLineF new_line;

    QJsonValue cur_value = json.value("first_point");
    if (!cur_value.isArray()) {
        return false;
    }
    QJsonValue x = cur_value.toArray().at(0);
    QJsonValue y = cur_value.toArray().at(0);
    if (!x.isDouble() || !y.isDouble()) {
        return false;
    }
    new_line.setP1(QPointF(x.toDouble(), y.toDouble()));

    cur_value = json.value("second_point");
    if (!cur_value.isArray()) {
        return false;
    }
    x = cur_value.toArray().at(0);
    y = cur_value.toArray().at(0);
    if (!x.isDouble() || !y.isDouble()) {
        return false;
    }
    new_line.setP2(QPointF(x.toDouble(), y.toDouble()));

    setLine(new_line);

    return true;
}
//_____________________________________________________________________________

bool LineItem::serialize(QJsonObject& json) const
{
    json = QJsonObject();

    QLineF cur_line = line();
    if (cur_line.isNull()) {
        return false;
    }

    json.insert("first_point", QJsonArray{ cur_line.p1().x(), cur_line.p1().y() });
    json.insert("second_point", QJsonArray{ cur_line.p2().x(), cur_line.p2().y() });

    return true;
}
#else
static_assert(false, "No serializer defined.");
#endif
//=============================================================================

//=============================================================================
// PencilItem
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
