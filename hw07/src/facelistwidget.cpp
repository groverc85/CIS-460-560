#include "facelistwidget.h"

FaceListWidget::FaceListWidget(QWidget* parent) : QListWidget(parent)
{

}

void FaceListWidget::slot_receiveFace(QListWidgetItem *item)
{
    this->addItem(item);
}
