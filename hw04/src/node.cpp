#include "node.h"


Node::Node()
{
    geometry = NULL;
    color = glm::vec4(1, 1, 1, 1);
}

Node::Node(glm::mat4 transformationMatrix, QString name)
{
    this->TransformationMatrix = transformationMatrix;
    this->name = name;
    setText(0, name);
}

void Node::addChild(QTreeWidgetItem *c) {

    QTreeWidgetItem::addChild(c);
    children.push_back((Node*)c);
}

void Node::setGeometry(Drawable* g){
    geometry = g;
}

void Node::setGeomColor(const glm::vec4& c){
    color = c;
}

glm::mat4 Node::MatrixStored() {
    return TransformationMatrix;
}

TranslateNode::TranslateNode(float x, float y, float z) {
    TransformationMatrix = glm::mat4{{1,0,0,0}, {0,1,0,0}, {0,0,1,0}, {x,y,z,1}};
}

RotateNode::RotateNode(float angle, float x, float y, float z) {
    float angle_radian = glm::radians(angle);
    float cos_angle = cos(angle_radian), sin_angle = sin(angle_radian);

    glm::vec4 col1, col2, col3, col4;

    col1 = glm::vec4{cos_angle+x*x*(1-cos_angle), y*x*(1-cos_angle)+z*sin_angle, z*x*(1-cos_angle)-y*sin_angle, 0};
    col2 = glm::vec4{x*y*(1-cos_angle)-z*sin_angle, cos_angle+y*y*(1-cos_angle), z*y*(1-cos_angle)+x*sin_angle, 0};
    col3 = glm::vec4{x*z*(1-cos_angle)+y*sin_angle, y*z*(1-cos_angle) - x*sin_angle, cos_angle+z*z*(1-cos_angle), 0};
    col4 = glm::vec4{0.f, 0.f, 0.f, 1.f};

    TransformationMatrix = glm::mat4{col1, col2, col3, col4};
}

ScaleNode::ScaleNode(float x, float y, float z) {
    TransformationMatrix = glm::mat4{{x,0,0,0}, {0,y,0,0}, {0,0,z,0}, {0,0,0,1}};
}
