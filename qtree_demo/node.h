#ifndef NODE_H
#define NODE_H

#include <QTreeWidgetItem>
#include <QString>
#include <vector>


class Node : public QTreeWidgetItem
{
private:
    QString name;
    std::vector<Node*> children;
public:
    Node(const QString &name);
    virtual void addChild(QTreeWidgetItem*);
};

#endif // NODE_H
