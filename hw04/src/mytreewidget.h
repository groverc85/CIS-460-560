#ifndef MYTREEWIDGET_H
#define MYTREEWIDGET_H

#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <mygl.h>

class MyTreeWidget : public QTreeWidget
{
    Q_OBJECT

private:
    Node *selected;
    QString newNodeName;

public:
    MyTreeWidget(QWidget* p);

public slots:
    void slot_scenegraph(QTreeWidgetItem* n);
};

#endif // MYTREEWIDGET_H
