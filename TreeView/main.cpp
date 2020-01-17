#include "treeviewwidget.h"
#include <QApplication>
#include"treeviewwidget.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    TreeViewWidget w;
    w.show();

    return a.exec();
}
