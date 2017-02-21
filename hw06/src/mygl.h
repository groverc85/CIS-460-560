#ifndef MYGL_H
#define MYGL_H

#include <glwidget277.h>
#include <utils.h>
#include <shaderprogram.h>
#include <scene/cylinder.h>
#include <scene/sphere.h>
#include <scene/face_draw.h>
#include <scene/line.h>
#include <scene/point.h>
#include <mesh.h>
#include "camera.h"

#include <QOpenGLVertexArrayObject>
#include <QOpenGLShaderProgram>
#include <QDoubleSpinBox>
#include <QListWidgetItem>
#include <meshcomponent.h>


class MyGL : public GLWidget277
{
private:
    Cylinder geom_cylinder;// The instance of a unit cylinder we can use to render any cylinder
    Sphere geom_sphere;// The instance of a unit sphere we can use to render any sphere

    ShaderProgram prog_lambert;// A shader program that uses lambertian reflection
    ShaderProgram prog_flat;// A shader program that uses "flat" reflection (no shadowing at all)

    GLuint vao; // A handle for our vertex array object. This will store the VBOs created in our geometry classes.
                // Don't worry too much about this. Just know it is necessary in order to render geometry.

    Camera gl_camera;

    /// Timer linked to timerUpdate(). Fires approx. 60 times per second
    QTimer timer;

    int timeCount;
    int renderMode;

public:
    Mesh geom_mesh;
    int cube_display = 1;

    HalfEdge *edgeSelection;
    Face* faceSelection;
    Vertex* vertexSelection;

    Face_draw face_draw;
    Point point;
    Line line;

    std::map<int, Vertex*> centroid;
    std::map<int, std::vector<Vertex*>> mid; // each face -> midpoint
    std::map<int, Vertex*> midpoint; // each original halfedge -> midpoint

    explicit MyGL(QWidget *parent = 0);
    ~MyGL();

    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();

    void addVertex(HalfEdge* edge, int mode);
    void Quadrangle(Face* face, Vertex* centroid, std::vector<HalfEdge*> mid);



private slots:
    /// Slot that gets called ~60 times per second
    void timerUpdate();

public slots:
    virtual void slot_vertexSelectChanged(QListWidgetItem*);
    virtual void slot_halfEdgeSelectChanged(QListWidgetItem*);
    virtual void slot_faceSelectChanged(QListWidgetItem*);

    virtual void slot_receiveVertexTranslateX(double);
    virtual void slot_receiveVertexTranslateY(double);
    virtual void slot_receiveVertexTranslateZ(double);

    virtual void slot_receiveFaceColorR(double);
    virtual void slot_receiveFaceColorG(double);
    virtual void slot_receiveFaceColorB(double);

    virtual void slot_addVertex();
    virtual void slot_Triangulate();
    virtual void slot_subdivision();
    virtual void slot_extrude();


protected:
    void keyPressEvent(QKeyEvent *e);


};

#endif // MYGL_H
