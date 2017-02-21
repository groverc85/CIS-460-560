#include "mytreewidget.h"

MyTreeWidget::MyTreeWidget(QWidget *p) : QTreeWidget(p), newNodeName(), selected(nullptr)
{

}

void MyTreeWidget::slot_addNode(QTreeWidgetItem *n)
{
    addTopLevelItem(n);
}

void MyTreeWidget::slot_storeNewNodeName(QString s)
{
    newNodeName = s;
}

void MyTreeWidget::slot_storeSelectedNode(QTreeWidgetItem *n)
{
    selected = (Node*)n;
}

void MyTreeWidget::slot_addChildToSelected()
{
    selected->addChild(new Node(newNodeName));
}
