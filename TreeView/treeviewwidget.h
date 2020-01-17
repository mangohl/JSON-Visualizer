#ifndef TREEVIEWWIDGET_H
#define TREEVIEWWIDGET_H

#include <QWidget>
class TreeView;
namespace Ui {
class TreeViewWidget;
}

class TreeViewWidget : public QWidget
{
    Q_OBJECT

public:
    explicit TreeViewWidget(QWidget *parent = nullptr);
    ~TreeViewWidget();

    void readFile(const QString &fileName);
private slots:
    void on_pushButton_clicked();

private:
    Ui::TreeViewWidget *ui;

    TreeView *m_treeView;
};

#endif // TREEVIEWWIDGET_H
