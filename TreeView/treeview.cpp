#include "treeview.h"
#include "edge.h"
#include "node.h"
#include <math.h>
#include"QGraphicsLinearLayout"
#include <QKeyEvent>
#include "QGraphicsLinearLayout"
#include "qfile.h"
#include "qdebug.h"
#include "QJsonDocument"
#include "qjsonobject.h"
#include "QJsonValue"
#include "qjsonarray.h"
TreeView::TreeView(QWidget *parent)
    : QGraphicsView(parent),_scene(NULL)
{
    _scene = new QGraphicsScene(this);
    _scene->setItemIndexMethod(QGraphicsScene::NoIndex);

    setScene(_scene);
    setCacheMode(CacheBackground);
    setViewportUpdateMode(BoundingRectViewportUpdate);
    setRenderHint(QPainter::Antialiasing);
    setTransformationAnchor(AnchorUnderMouse);

    m_levelColors.push_back("#205374");
    m_levelColors.push_back("#27a09e");
    m_levelColors.push_back("#30ce88");
    m_levelColors.push_back("#7de393");
    m_levelColors.push_back("#d3f5ce");



}

QJsonObject TreeView::getJsonRoot(const QString &fileName)
{
    QFile loadFile(fileName);

       if(!loadFile.open(QIODevice::ReadOnly))
       {
           qDebug() << "could't open projects json";
           return QJsonObject();
       }

       QByteArray allData = loadFile.readAll();
       loadFile.close();

       QJsonParseError json_error;
       QJsonDocument jsonDoc(QJsonDocument::fromJson(allData, &json_error));

       if(json_error.error != QJsonParseError::NoError)
       {
           qDebug() << "json error!";
           return QJsonObject();
       }

       QJsonObject rootObj = jsonDoc.object();

       return rootObj;

}

void TreeView::setupJsonView(QJsonObject &root)
{

    _scene->clear();

    QGraphicsLayout *layout = createObjectNode(NULL,"root",root,0);
    layout->setContentsMargins(0,0,0,0);
    QGraphicsWidget *widget = new QGraphicsWidget();
    //widget->setMinimumSize(800,800);


    widget->setLayout(layout);
    _scene->addItem(widget);



}

QGraphicsLayout *TreeView::createObjectNode(Node *parentNode, const QString &key, const QJsonObject &jsonObj,int level)
{

    QGraphicsLinearLayout *hlayout  = new QGraphicsLinearLayout;
    hlayout->setOrientation(Qt::Horizontal);

    QGraphicsLinearLayout *vlayout  = new QGraphicsLinearLayout;
    vlayout->setOrientation(Qt::Vertical);

    Node *objectNode = createNode(parentNode,key,level);

    hlayout->setSpacing(200);

    if(objectNode)
    {
        QGraphicsLinearLayout *layout = new QGraphicsLinearLayout();
        layout->setOrientation(Qt::Vertical);
        layout->addStretch();
        layout->addItem(objectNode);
        layout->addStretch();

        hlayout->addItem(layout);
    }


    for(auto it = jsonObj.begin(); it != jsonObj.end(); ++it){
        auto child = it.value();
        auto childName = it.key();
        auto childType = it.value().type();

        QGraphicsLayout *rt = NULL;
        if(childType == QJsonValue::Object)
        {
            rt = createObjectNode(objectNode, childName, child.toObject(),level+1);

        }
        else if(childType == QJsonValue::Array)
        {
            rt = createArrayNode(objectNode, childName, child.toArray(),level+1);
        }
        else
        {
            rt = createLeafNode(objectNode, childName, child,level+1);
        }
        if(rt == nullptr)
        {
            qWarning()<<"Create child node error!"<<childName<<childType;
            return nullptr;
        }

        vlayout->addItem(rt);
    }

    hlayout->addItem(vlayout);
    hlayout->addStretch();

    this->centerNode = objectNode;
    return hlayout;
}

QGraphicsLayout *TreeView::createLeafNode(Node *parentNode, const QString &key, const QJsonValue &jsonVal,int level)
{
    QGraphicsLinearLayout *layout  = new QGraphicsLinearLayout;
    layout->setOrientation(Qt::Vertical);

    Node *node = NULL;
    QString data = "";

    QJsonValue::Type leafType = jsonVal.type();
    switch (leafType)
    {
    case QJsonValue::Bool:
        data = key + " : " + QString(jsonVal.toBool()? "true" : "false");
        node = createNode(parentNode,data,level);
        break;
    case QJsonValue::Double:
        data = key + " : " + QString::number(jsonVal.toDouble());
        node = createNode(parentNode,data,level);
        break;
    case QJsonValue::String:
        data = key + " : " + jsonVal.toString();
        node = createNode(parentNode,data,level);
        break;
    default:
        break;
    }

    if(node)
    {
        layout->addStretch();
        layout->addItem(node);
        layout->addStretch();
    }
    return layout;
}

QGraphicsLayout *TreeView::createArrayNode(Node *parentNode, const QString &key, const QJsonArray &jsonArray,int level)
{
    QGraphicsLinearLayout *hlayout  = new QGraphicsLinearLayout;
    hlayout->setOrientation(Qt::Horizontal);

    QGraphicsLinearLayout *vlayout  = new QGraphicsLinearLayout;
    vlayout->setOrientation(Qt::Vertical);

    Node *arrayNode = createNode(parentNode,key,level);


    hlayout->setSpacing(100);
    if(arrayNode)
    {
        QGraphicsLinearLayout *layout = new QGraphicsLinearLayout();
        layout->setOrientation(Qt::Vertical);
        layout->addStretch();
        layout->addItem(arrayNode);
        layout->addStretch();

        hlayout->addItem(layout);
    }

    for(int i = 0; i != jsonArray.size(); ++i)
    {
        auto element = jsonArray[i];
        auto type = element.type();
        QGraphicsLayout*rt = NULL;
        auto elementKey = QString("[%1]").arg(i);
        if(type == QJsonValue::Object){
            rt = createObjectNode(arrayNode, elementKey, element.toObject(),level+1);
        }
        else if(type == QJsonValue::Array){
            rt = createArrayNode(arrayNode, elementKey, element.toArray(),level+1);
        }
        else{
            rt = createLeafNode(arrayNode, elementKey, element,level+1);
        }
        if(rt == NULL){
            qWarning()<<"Create array element error!"<<elementKey<<type;
            return nullptr;
        }

        vlayout->addItem(rt);
    }


    hlayout->addItem(vlayout);

    return hlayout;
}

Node *TreeView::createNode(Node *parentNode, const QString &data,int level)
{


    int size = m_levelColors.size();
    QString  color;
    if(level>=size)
        color = (m_levelColors.last());
    else
        color = (m_levelColors.at(level));


    Node *node = new Node(this);
    node->setToolTip(data);
    node->setData(Qt::UserRole+1,color);


    if(parentNode!=NULL)
        _scene->addItem(new Edge(parentNode,node));

    return node;
}

void TreeView::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Up:
        centerNode->moveBy(0, -20);
        break;
    case Qt::Key_Down:
        centerNode->moveBy(0, 20);
        break;
    case Qt::Key_Left:
        centerNode->moveBy(-20, 0);
        break;
    case Qt::Key_Right:
        centerNode->moveBy(20, 0);
        break;
    case Qt::Key_Plus:
        zoomIn();
        break;
    case Qt::Key_Minus:
        zoomOut();
        break;
    default:
        QGraphicsView::keyPressEvent(event);
    }
}


void TreeView::wheelEvent(QWheelEvent *event)
{
   // scaleView(pow((double)2, -event->delta() / 240.0));
    return QGraphicsView::wheelEvent(event);
}


void TreeView::scaleView(qreal scaleFactor)
{
    qreal factor = transform().scale(scaleFactor, scaleFactor).mapRect(QRectF(0, 0, 1, 1)).width();
    if (factor < 0.07 || factor > 100)
        return;

    scale(scaleFactor, scaleFactor);
}



void TreeView::zoomIn()
{
    scaleView(qreal(1.2));
}

void TreeView::zoomOut()
{
    scaleView(1 / qreal(1.2));
}
