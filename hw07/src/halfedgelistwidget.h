#ifndef HALFEDGELISTWIDGET_H
#define HALFEDGELISTWIDGET_H

#include <QListWidget>

class HalfEdgeListWidget  : public QListWidget
{
    Q_OBJECT
public:
    HalfEdgeListWidget(QWidget* parent);
public slots:
    void slot_receiveHalfEdge(QListWidgetItem*);
};

#endif // HALFEDGELISTWIDGET_H
