#include "edge.h"
#include "node.h"
#include "treeview.h"
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QStyleOption>
#include <QGraphicsLayout>
#include <QFontMetrics>
//! [0]
Node::Node(TreeView *TreeView)
    : graph(TreeView)
{
    setFlag(ItemIsMovable);
    setFlag(ItemSendsGeometryChanges);
    setCacheMode(DeviceCoordinateCache);
    setZValue(-1);

    setFont(QFont("times",30));


}
//! [0]

//! [1]
void Node::addEdge(Edge *edge)
{
    edgeList << edge;
    edge->adjust();
}

QList<Edge *> Node::edges() const
{
    return edgeList;
}
//! [1]





//! [8]
QRectF Node::boundingRect() const
{
    QString text = this->toolTip();
    QFontMetrics fm(this->font());
    QSize size = fm.size(Qt::TextSingleLine,text);

    int width = size.width()+10;
    int height = size .height()+10;
    return QRectF(0, -height/2, width, height);


//    qreal adjust = 2;
//    return QRectF( -10 - adjust, -10 - adjust, 23 + adjust, 23 + adjust);
}
//! [8]

//! [9]
QPainterPath Node::shape() const
{
    QString text = this->toolTip();
    QFontMetrics fm(this->font());
    QSize size = fm.size(Qt::TextSingleLine,text);

    int width = size.width()+10;
    int height = size .height()+10;

    QPainterPath path;
    path.addRect(0, -height/2, width, height);
    return path;

//    QPainterPath path;
//    path.addEllipse(-10, -10, 20, 20);
//    return path;
}
//! [9]

//! [10]
void Node::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *)
{


    QRectF rect = this->boundingRect();
    painter->setPen(Qt::NoPen);

    QString bdColor = this->data(Qt::UserRole+1).toString();
    QColor color=QColor(bdColor);
    painter->setBrush(color);
    painter->drawRoundedRect(rect,10,10);


    painter->setPen(Qt::white);
    painter->setFont(this->font());
    painter->drawText(rect,Qt::AlignCenter,this->toolTip());

#if 0
    painter->setPen(Qt::NoPen);
    painter->setBrush(Qt::darkGray);
    painter->drawEllipse(-7, -7, 20, 20);

    QRadialGradient gradient(-3, -3, 10);
    if (option->state & QStyle::State_Sunken) {
        gradient.setCenter(3, 3);
        gradient.setFocalPoint(3, 3);
        gradient.setColorAt(1, QColor(Qt::yellow).light(120));
        gradient.setColorAt(0, QColor(Qt::darkYellow).light(120));
    } else {
        gradient.setColorAt(0, Qt::yellow);
        gradient.setColorAt(1, Qt::darkYellow);
    }
    painter->setBrush(gradient);

    painter->setPen(QPen(Qt::black, 0));
    painter->drawEllipse(-10, -10, 20, 20);

#endif
}


//! [10]

//! [11]
QVariant Node::itemChange(GraphicsItemChange change, const QVariant &value)
{
    switch (change) {
    case ItemPositionHasChanged:
        foreach (Edge *edge, edgeList)
            edge->adjust();
        break;
    default:
        break;
    };

    return QGraphicsItem::itemChange(change, value);
}
//! [11]

//! [12]
void Node::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    update();
    QGraphicsItem::mousePressEvent(event);
}

void Node::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    update();
    QGraphicsItem::mouseReleaseEvent(event);
}
//! [12]
