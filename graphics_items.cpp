#include "graphics_items.h"


//=============================================================================
// LineItem
//=============================================================================
#ifdef JSON_SERIALIZER
bool LineItem::deserialize(const QJsonObject& json)
{
    Q_ASSERT(!json.isEmpty());

    QLineF new_line;

    // points
    QJsonValue cur_value = json.value("first_point");
    if (!cur_value.isArray()) {
        return false;
    }
    QJsonValue x = cur_value.toArray().at(0);
    QJsonValue y = cur_value.toArray().at(1);
    if (!x.isDouble() || !y.isDouble()) {
        return false;
    }
    new_line.setP1(QPointF(x.toDouble(), y.toDouble()));

    cur_value = json.value("second_point");
    if (!cur_value.isArray()) {
        return false;
    }
    x = cur_value.toArray().at(0);
    y = cur_value.toArray().at(1);
    if (!x.isDouble() || !y.isDouble()) {
        return false;
    }
    new_line.setP2(QPointF(x.toDouble(), y.toDouble()));

    QPen new_pen(QBrush(), 1, Qt::SolidLine, Qt::RoundCap);

    // color
    cur_value = json.value("color");
    if (!cur_value.isArray()) {
        return false;
    }
    QJsonValue r = cur_value.toArray().at(0);
    QJsonValue g = cur_value.toArray().at(1);
    QJsonValue b = cur_value.toArray().at(2);
    QJsonValue a = cur_value.toArray().at(3);
    if (!a.isDouble() || !r.isDouble() ||
        !g.isDouble() || !b.isDouble()) {
        return false;
    }
    new_pen.setColor(QColor(r.toInt(), g.toInt(),
                            b.toInt(), a.toInt()));

    // width
    cur_value = json.value("width");
    if (!cur_value.isDouble()) {
        return false;
    }
    new_pen.setWidthF(cur_value.toDouble());

    setPen(new_pen);
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

    QColor color = pen().color();
    json.insert("color", QJsonArray{
            color.red(),
            color.green(),
            color.blue(),
            color.alpha()
        });

    json.insert("width", pen().widthF());

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
    setPen(QPen(QBrush(), 1, Qt::SolidLine, Qt::RoundCap));
    verticesToPath();
}
//_____________________________________________________________________________

PencilItem::PencilItem(const QPolygonF&& vertices) : vertices(vertices)
{
    setPen(QPen(QBrush(), 1, Qt::SolidLine, Qt::RoundCap));
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

void PencilItem::addVertex(const QPointF& vertex)
{
    vertices.append(vertex);
    verticesToPath();
}
//_____________________________________________________________________________

void PencilItem::translateVertices(const QPointF& offset)
{
    vertices.translate(offset);
    verticesToPath();
}
//_____________________________________________________________________________

#ifdef JSON_SERIALIZER
bool PencilItem::deserialize(const QJsonObject& json)
{
    Q_ASSERT(!json.isEmpty());

    // vertices
    QJsonValue cur_value = json.value("coordinates");
    if (!cur_value.isArray()) {
        return false;
    }
    QPolygonF new_vertices;
    new_vertices.reserve(cur_value.toArray().size());

    for (const QJsonValue& vertex : cur_value.toArray()) {
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

    QPen new_pen = defaultPen();

    // color
    cur_value = json.value("color");
    if (!cur_value.isArray()) {
        return false;
    }
    QJsonValue r = cur_value.toArray().at(0);
    QJsonValue g = cur_value.toArray().at(1);
    QJsonValue b = cur_value.toArray().at(2);
    QJsonValue a = cur_value.toArray().at(3);
    if (!a.isDouble() || !r.isDouble() ||
        !g.isDouble() || !b.isDouble()) {
        return false;
    }
    new_pen.setColor(QColor(r.toInt(), g.toInt(),
                            b.toInt(), a.toInt()));

    // width
    cur_value = json.value("width");
    if (!cur_value.isDouble()) {
        return false;
    }
    new_pen.setWidthF(cur_value.toDouble());

    setPen(new_pen);
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

    QColor color = pen().color();
    json.insert("color", QJsonArray{
            color.red(),
            color.green(),
            color.blue(),
            color.alpha()
        });

    json.insert("width", pen().widthF());

    return true;
}
#else
    static_assert(false, "No serializer defined.");
#endif
//_____________________________________________________________________________

QPen PencilItem::defaultPen()
{
    return QPen(QBrush(), 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
}
//_____________________________________________________________________________

void PencilItem::verticesToPath()
{
    QPainterPath path;
    path.addPolygon(vertices);
    setPath(path);
}
//=============================================================================


//=============================================================================
// RectangleItem
//=============================================================================
#ifdef JSON_SERIALIZER
bool RectangleItem::deserialize(const QJsonObject& json)
{
    Q_ASSERT(!json.isEmpty());

    // size
    QJsonValue cur_value = json.value("size");
    if (!cur_value.isArray()) {
        return false;
    }
    QJsonValue width = cur_value.toArray().at(0);
    QJsonValue height = cur_value.toArray().at(1);
    if (!width.isDouble() || !height.isDouble()) {
        return false;
    }

    QPen new_pen(QBrush(), 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);

    // color
    cur_value = json.value("color");
    if (!cur_value.isArray()) {
        return false;
    }
    QJsonValue r = cur_value.toArray().at(0);
    QJsonValue g = cur_value.toArray().at(1);
    QJsonValue b = cur_value.toArray().at(2);
    QJsonValue a = cur_value.toArray().at(3);
    if (!a.isDouble() || !r.isDouble() ||
        !g.isDouble() || !b.isDouble()) {
        return false;
    }
    new_pen.setColor(QColor(r.toInt(), g.toInt(),
                            b.toInt(), a.toInt()));

    // width
    cur_value = json.value("width");
    if (!cur_value.isDouble()) {
        return false;
    }
    new_pen.setWidthF(cur_value.toDouble());

    setPen(new_pen);
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

    QColor color = pen().color();
    json.insert("color", QJsonArray{
            color.red(),
            color.green(),
            color.blue(),
            color.alpha()
        });

    json.insert("width", pen().widthF());

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

    // size
    QJsonValue cur_value = json.value("size");
    if (!cur_value.isArray()) {
        return false;
    }
    QJsonValue width = cur_value.toArray().at(0);
    QJsonValue height = cur_value.toArray().at(1);
    if (!width.isDouble() || !height.isDouble()) {
        return false;
    }

    QPen new_pen(QBrush(), 1, Qt::SolidLine, Qt::RoundCap);

    // color
    cur_value = json.value("color");
    if (!cur_value.isArray()) {
        return false;
    }
    QJsonValue r = cur_value.toArray().at(0);
    QJsonValue g = cur_value.toArray().at(1);
    QJsonValue b = cur_value.toArray().at(2);
    QJsonValue a = cur_value.toArray().at(3);
    if (!a.isDouble() || !r.isDouble() ||
        !g.isDouble() || !b.isDouble()) {
        return false;
    }
    new_pen.setColor(QColor(r.toInt(), g.toInt(),
                            b.toInt(), a.toInt()));

    // width
    cur_value = json.value("width");
    if (!cur_value.isDouble()) {
        return false;
    }
    new_pen.setWidthF(cur_value.toDouble());

    setPen(new_pen);
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

    QColor color = pen().color();
    json.insert("color", QJsonArray{
            color.red(),
            color.green(),
            color.blue(),
            color.alpha()
        });

    json.insert("width", pen().widthF());

    return true;
}
#else
static_assert(false, "No serializer defined.");
#endif
//=============================================================================
