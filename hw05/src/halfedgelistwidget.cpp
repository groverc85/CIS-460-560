#include "halfedgelistwidget.h"

HalfEdgeListWidget::HalfEdgeListWidget(QWidget* parent) : QListWidget(parent)
{

}


void HalfEdgeListWidget::slot_receiveHalfEdge(QListWidgetItem *item)
{
    this->addItem(item);
}
