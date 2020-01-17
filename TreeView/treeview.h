#ifndef TreeView_H
#define TreeView_H

#include <QGraphicsView>

class Node;
class QGraphicsLayout;

//! [0]
class TreeView : public QGraphicsView
{
    Q_OBJECT

public:
    TreeView(QWidget *parent = 0);


//[2]json show start
public :

    QJsonObject getJsonRoot(const QString& fileName);
    void setupJsonView(QJsonObject &);

private:
    QGraphicsLayout * createObjectNode(Node *parentNode, const QString &key, const QJsonObject &jsonObj,int leverl);
    QGraphicsLayout * createLeafNode(Node *parentNode, const QString &key, const QJsonValue &jsonVal ,int leverl);
    QGraphicsLayout * createArrayNode(Node *parentNode, const QString &key, const QJsonArray &jsonArray,int level);

    Node *createNode(Node * parentNode,const QString& data,int lever);

//[2]json show end

public slots:
    void zoomIn();
    void zoomOut();

protected:
    void keyPressEvent(QKeyEvent *event) Q_DECL_OVERRIDE;
#ifndef QT_NO_WHEELEVENT
    void wheelEvent(QWheelEvent *event) Q_DECL_OVERRIDE;
#endif

    void scaleView(qreal scaleFactor);

private:


    Node *centerNode;

    QGraphicsScene *_scene;

    QList<QString> m_levelColors;


};
//! [0]

#endif // TreeView_H
