#ifndef VERTEXLISTWIDGET_H
#define VERTEXLISTWIDGET_H

#include <QListWidget>
#include <meshcomponent.h>

class VertexListWidget  : public QListWidget
{
    Q_OBJECT
public:
    VertexListWidget(QWidget* parent);

public slots:
    void slot_receiveVertex(QListWidgetItem*);
};

#endif // VERTEXLISTWIDGET_H
