#ifndef GLWIDGET277_H
#define GLWIDGET277_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_2_Core>
#include <QTimer>
#include <QListWidgetItem>


class GLWidget277
    : public QOpenGLWidget,
      public QOpenGLFunctions_3_2_Core
{
    Q_OBJECT

protected:
    /*** AUTOMATIC TESTING: DO NOT MODIFY ***/
    /*** If true, save a test image and exit */
    /***/ bool autotesting;

public:
    GLWidget277(QWidget *parent);
    ~GLWidget277();

    void debugContextVersion();
    void printGLErrorLog();
    void printLinkInfoLog(int prog);
    void printShaderInfoLog(int shader);

private slots:
    /*** AUTOMATIC TESTING: DO NOT MODIFY ***/
    /***/ void saveImageAndQuit();
    virtual void timerUpdate(){}

signals:
    void sig_sendVertex(QListWidgetItem*);
    void sig_sendFace(QListWidgetItem*);
    void sig_sendHalfEdge(QListWidgetItem*);

    void sig_updateVertexX(double);
    void sig_updateVertexY(double);
    void sig_updateVertexZ(double);

    void sig_updateFaceR(double);
    void sig_updateFaceG(double);
    void sig_updateFaceB(double);


public slots:
    virtual void slot_halfEdgeSelectChanged(QListWidgetItem*) = 0;
    virtual void slot_faceSelectChanged(QListWidgetItem*) = 0;
    virtual void slot_vertexSelectChanged(QListWidgetItem*) = 0;
    virtual void slot_receiveVertexTranslateX(double) = 0;
    virtual void slot_receiveVertexTranslateY(double) = 0;
    virtual void slot_receiveVertexTranslateZ(double) = 0;
    virtual void slot_receiveFaceColorR(double) = 0;
    virtual void slot_receiveFaceColorG(double) = 0;
    virtual void slot_receiveFaceColorB(double) = 0;
    virtual void slot_addVertex() = 0;
    virtual void slot_Triangulate() = 0;
    virtual void slot_subdivision() = 0;
    virtual void slot_extrude() = 0;

};


#endif // GLWIDGET277_H
