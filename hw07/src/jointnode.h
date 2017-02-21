#ifndef JOINTNODE_H
#define JOINTNODE_H

#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <scene/joint.h>


class JointNode : public QTreeWidget
{
    Q_OBJECT
public:
    JointNode(QWidget* parent = NULL);
    ~JointNode();
    Joint* selection = nullptr;

public slots:
    void slot_receiveJoint(QTreeWidgetItem*);
    void slot_itemChanged(QTreeWidgetItem*,int);
    void slot_translateX(double x);
    void slot_translateY(double y);
    void slot_translateZ(double z);
    void slot_rotateXPlus();
    void slot_rotateXMinus();
    void slot_rotateYPlus();
    void slot_rotateYMinus();
    void slot_rotateZPlus();
    void slot_rotateZMinus();

signals:
    void sig_updateVertex();
    void sig_translateX(double);
    void sig_translateY(double);
    void sig_translateZ(double);
};

#endif // JOINTNODE_H
