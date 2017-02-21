#include "jointnode.h"


JointNode::JointNode(QWidget* parent) : QTreeWidget(parent)
{}

JointNode::~JointNode() {
}

void JointNode::slot_receiveJoint(QTreeWidgetItem *p)
{
    QTreeWidget::addTopLevelItem(p);
}

void JointNode::slot_itemChanged(QTreeWidgetItem* item,int num)
{
    if (selection != nullptr) {
        selection->create(false);
    }
     selection = static_cast<Joint*>(item);

     glm::vec3 position = selection->pos;

     emit sig_translateX(position.x);
     emit sig_translateY(position.y);
     emit sig_translateZ(position.z);

     selection->create(true);
}

void JointNode::slot_translateX(double x)
{
    if (selection != nullptr)
        selection->pos = glm::vec3(x,selection->pos[1],selection->pos[2]);

    emit sig_updateVertex();

    selection->create(true);
}

void JointNode::slot_translateY(double y)
{
    if (selection != nullptr)
        selection->pos = glm::vec3(selection->pos[0], y,selection->pos[2]);

    emit sig_updateVertex();

    selection->create(true);
}

void JointNode::slot_translateZ(double z)
{
    if (selection != nullptr)
        selection->pos = glm::vec3(selection->pos[0], selection->pos[1], z);

    emit sig_updateVertex();

    selection->create(true);
}

void JointNode::slot_rotateXPlus()
{
    if (selection != nullptr) {
        selection->rot = glm::quat(glm::angleAxis(1.0f, glm::vec3(1,0,0)) * selection->rot);
    }

    emit sig_updateVertex();

    selection->create(true);
}

void JointNode::slot_rotateXMinus()
{
    if (selection != nullptr) {
        selection->rot = glm::quat(glm::angleAxis(-1.0f, glm::vec3(1,0,0)) * selection->rot);
    }

    emit sig_updateVertex();

    selection->create(true);
}

void JointNode::slot_rotateYPlus()
{
    if (selection != nullptr) {
        selection->rot = glm::quat(glm::angleAxis(1.0f, glm::vec3(0,1,0)) * selection->rot);
    }

    emit sig_updateVertex();

    selection->create(true);
}

void JointNode::slot_rotateYMinus()
{
    if (selection != nullptr) {
        selection->rot = glm::quat(glm::angleAxis(-1.0f, glm::vec3(0,1,0)) * selection->rot);
    }

    emit sig_updateVertex();

    selection->create(true);
}

void JointNode::slot_rotateZPlus()
{
    if (selection != nullptr) {
        selection->rot = glm::quat(glm::angleAxis(1.0f, glm::vec3(0,0,1)) * selection->rot);
    }

    emit sig_updateVertex();

    selection->create(true);
}

void JointNode::slot_rotateZMinus()
{
    if (selection != nullptr) {
        selection->rot = glm::quat(glm::angleAxis(-1.0f, glm::vec3(0,0,1)) * selection->rot);
    }

    emit sig_updateVertex();

    selection->create(true);
}
