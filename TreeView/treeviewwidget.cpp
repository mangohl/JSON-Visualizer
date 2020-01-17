#include "treeviewwidget.h"
#include "ui_treeviewwidget.h"
#include"treeview.h"
#include"QHBoxLayout"
#include "qfile.h"
#include "qdebug.h"
#include "qqueue.h"
#include "QJsonDocument"
#include "qjsonobject.h"
#include "QJsonValue"
#include "qjsonarray.h"
TreeViewWidget::TreeViewWidget(QWidget *parent) :
    QWidget(parent),m_treeView(NULL),
    ui(new Ui::TreeViewWidget)
{
    ui->setupUi(this);

    m_treeView = new TreeView();
    QHBoxLayout *layout = new QHBoxLayout(ui->widget);
    layout->addWidget(m_treeView);
    this->resize(1000,600);
    this->setWindowTitle("JSON Visualizer");


    readFile(":/demo.json");
   // readFile(":/swagger.json");

}

TreeViewWidget::~TreeViewWidget()
{
    delete ui;
}

void TreeViewWidget::on_pushButton_clicked()
{
    QJsonParseError json_error;
    QString text = ui->textEdit->toPlainText();
    QJsonDocument jsonDoc(QJsonDocument::fromJson(text.toUtf8(), &json_error));

    if(json_error.error != QJsonParseError::NoError)
    {

    }

    QJsonObject rootObj = jsonDoc.object();

    m_treeView->setupJsonView(rootObj);
}

void TreeViewWidget::readFile(const QString &fileName)
{
    QFile loadFile(fileName);

    if(!loadFile.open(QIODevice::ReadOnly))
    {
        qDebug() << "could't open projects json";
        return ;
    }

    QByteArray allData = loadFile.readAll();
    ui->textEdit->setText(allData);
    loadFile.close();

}
