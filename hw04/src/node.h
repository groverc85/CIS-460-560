#ifndef NODE_H
#define NODE_H

#include <glwidget277.h>
#include <utils.h>
#include <shaderprogram.h>

#include <QTreeWidgetItem>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLShaderProgram>

class Node : public QTreeWidgetItem
{
public:
    // A set of pointers to the node's children
    std::vector<Node*> children;
    // A pointer to one instance of geometry
    Drawable* geometry;
    // The color with which to draw the geometry pointed to by the node, if applicable
    glm::vec4 color;
    // A 4x4 matrix representing the 3D homogeneous transformation stored in this node.
    glm::mat4 TransformationMatrix;
    // A QString to represent a name for the node.
    QString name;
    // A function that returns the 4x4 matrix stored in the node.
    glm::mat4 MatrixStored();
    Node();
    Node(glm::mat4 transformationMatrix, QString name);

    void addChild(QTreeWidgetItem* c);
    void setGeomColor(const glm::vec4& c);
    void setGeometry(Drawable* g);
};

class TranslateNode : public Node {
public:
    TranslateNode(float x, float y, float z);
};

class RotateNode : public Node {
public:
    RotateNode(float angle, float x, float y, float z);
};

class ScaleNode : public Node {
public:
    ScaleNode(float x, float y, float z);
};


#endif // NODE_H
