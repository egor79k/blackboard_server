#include "graphics_items.h"


//=============================================================================
// LineItem
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
PencilItem::PencilItem(const QPolygonF& vertices) : vertices(vertices)
{
    verticesToPath();
}
//_____________________________________________________________________________

PencilItem::PencilItem(const QPolygonF&& vertices) : vertices(vertices)
{
    verticesToPath();
}
//_____________________________________________________________________________

void PencilItem::setVertices(const QPolygonF& vertices) {
    this->vertices = vertices;
    verticesToPath();
}
//_____________________________________________________________________________

void PencilItem::setVertices(const QPolygonF&& vertices)
{
    this->vertices = vertices;
    verticesToPath();
}
//_____________________________________________________________________________

#ifdef JSON_SERIALIZER
bool PencilItem::deserialize(const QJsonObject& json)
{
    Q_ASSERT(!json.isEmpty());

    QPolygonF new_vertices;

    QJsonValue json_vertices = json.value("coordinates");
    if (!json_vertices.isArray()) {
        return false;
    }

    new_vertices.reserve(json_vertices.toArray().size());

    for (const QJsonValue& vertex : json_vertices.toArray()) {
        if (!vertex.isArray()) {
            return false;
        }

        QJsonValue x = vertex.toArray().at(0);
        QJsonValue y = vertex.toArray().at(1);
        if (!x.isDouble() || !y.isDouble()) {
            return false;
        }

        new_vertices.append(QPointF(x.toDouble(), y.toDouble()));
    }

    vertices = new_vertices;
    verticesToPath();

    return true;
}
//_____________________________________________________________________________

bool PencilItem::serialize(QJsonObject& json) const
{
    json = QJsonObject();

    if (vertices.isEmpty()) {
        return false;
    }

    QJsonArray json_vertices;
    for (const QPointF& vertex : vertices) {
        json_vertices.append(QJsonArray{ vertex.x(), vertex.y() });
    }

    json.insert("coordinates", json_vertices);
    return true;
}
#else
    static_assert(false, "No serializer defined.");
#endif
//_____________________________________________________________________________

void PencilItem::verticesToPath()
{
    QPainterPath path;
    path.addPolygon(vertices);
    setPath(path);
}
//=============================================================================


/*
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
*/

//=============================================================================
// RectangleItem
//=============================================================================
#ifdef JSON_SERIALIZER
bool RectangleItem::deserialize(const QJsonObject& json)
{
    Q_ASSERT(!json.isEmpty());

    QJsonValue size = json.value("size");
    if (!size.isArray()) {
        return false;
    }
    QJsonValue width = size.toArray().at(0);
    QJsonValue height = size.toArray().at(1);
    if (!width.isDouble() || !height.isDouble()) {
        return false;
    }
    setRect(0, 0, width.toDouble(), height.toDouble());

    return true;
}
//_____________________________________________________________________________

bool RectangleItem::serialize(QJsonObject& json) const
{
    json = QJsonObject();

    QRectF cur_rect = rect();
    if (!cur_rect.isValid()) {
        return false;
    }

    json.insert("size", QJsonArray{ cur_rect.width(), cur_rect.height() });

    return true;
}
#else
static_assert(false, "No serializer defined.");
#endif
//=============================================================================


//=============================================================================
// EllipseItem
//=============================================================================
#ifdef JSON_SERIALIZER
bool EllipseItem::deserialize(const QJsonObject& json)
{
    Q_ASSERT(!json.isEmpty());

    QJsonValue size = json.value("size");
    if (!size.isArray()) {
        return false;
    }
    QJsonValue width = size.toArray().at(0);
    QJsonValue height = size.toArray().at(1);
    if (!width.isDouble() || !height.isDouble()) {
        return false;
    }
    setRect(0, 0, width.toDouble(), height.toDouble());

    return true;
}
//_____________________________________________________________________________

bool EllipseItem::serialize(QJsonObject& json) const
{
    json = QJsonObject();

    QRectF cur_rect = rect();
    if (!cur_rect.isValid()) {
        return false;
    }

    json.insert("size", QJsonArray{ cur_rect.width(), cur_rect.height() });

    return true;
}
#else
static_assert(false, "No serializer defined.");
#endif
//=============================================================================