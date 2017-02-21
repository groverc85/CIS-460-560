#include "mytreewidget.h"

MyTreeWidget::MyTreeWidget(QWidget *p) : QTreeWidget(p), newNodeName(), selected(nullptr)
{

}

void MyTreeWidget::slot_scenegraph(QTreeWidgetItem *n)
{
    addTopLevelItem(n);
}
