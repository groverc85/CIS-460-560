#include "node.h"

Node::Node(const QString &name)
    : name(name), children()
{
    setText(0, name);
}

void Node::addChild(QTreeWidgetItem *c)
{
    QTreeWidgetItem::addChild(c);
    children.push_back((Node*)c);
}
