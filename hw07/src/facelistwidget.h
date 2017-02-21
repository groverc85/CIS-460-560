#ifndef FACELISTWIDGET_H
#define FACELISTWIDGET_H

#include <QListWidget>

class FaceListWidget : public QListWidget
{
    Q_OBJECT
public:
    FaceListWidget(QWidget* parent);

public slots:
    void slot_receiveFace(QListWidgetItem*);
};

#endif // FACELISTWIDGET_H
