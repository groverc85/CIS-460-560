#include "vertexlistwidget.h"

VertexListWidget::VertexListWidget(QWidget* parent) : QListWidget(parent)
{
}

void VertexListWidget::slot_receiveVertex(QListWidgetItem *item)
{
    this->addItem(item);
}
