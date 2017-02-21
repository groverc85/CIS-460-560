#ifndef MYGL_H
#define MYGL_H

#include <glwidget277.h>
#include <utils.h>
#include <shaderprogram.h>
#include <scene/cylinder.h>
#include <scene/sphere.h>
#include <scene/cube.h>
#include <scene/cone.h>
#include <scene/pipe.h>
#include <QTreeWidgetItem>
#include <node.h>

#include <QOpenGLVertexArrayObject>
#include <QOpenGLShaderProgram>

class MyGL : public GLWidget277
{
    Q_OBJECT
private:
    Cylinder geom_cylinder;// The instance of a unit cylinder we can use to render any cylinder
    Sphere geom_sphere;// The instance of a unit sphere we can use to render any sphere
    Cube geom_cube;// The instance of a unit we can use to render any sphere
    Cone geom_cone;
    Pipe geom_pipe;
    ShaderProgram prog_lambert;// A shader program that uses lambertian reflection
    ShaderProgram prog_flat;// A shader program that uses "flat" reflection (no shadowing at all)

    GLuint vao; // A handle for our vertex array object. This will store the VBOs created in our geometry classes.
                // Don't worry too much about this. Just know it is necessary in order to render geometry.
public:
    explicit MyGL(QWidget *parent = 0);
    ~MyGL();
    Node* Root;
    Node* Body;
    Node* Head;
    Node* UpperLimb;
    Node* LowerLimb;

    glm::vec4 Body_color = glm::vec4(0.5,0.5,0.5,1);
    glm::vec4 Head_color = glm::vec4(0,0,1,1);
    glm::vec4 UpperLimb_color = glm::vec4(1,1,0,1);
    glm::vec4 LowerLimb_color = glm::vec4(0,1,0,1);

    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();
    void traverse(Node* node, glm::mat4 matrix, ShaderProgram p);
signals:
    void sig_scenegraph(QTreeWidgetItem*);

public slots:
    void slot_changeGeometry(QTreeWidgetItem*, int);

protected:
    void keyPressEvent(QKeyEvent *e);

};


#endif // MYGL_H
