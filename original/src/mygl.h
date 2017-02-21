#ifndef MYGL_H
#define MYGL_H

#include <glwidget277.h>
#include <utils.h>
#include <shaderprogram.h>
#include <scene/cube.h>
#include "camera.h"
#include <scene/scene.h>
#include <scene/chunk.h>
#include <scene/plusmark.h>
#include<scene/chunkmanager.h>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLShaderProgram>


class MyGL
    : public GLWidget277
{
private:
    Cube geom_cube;// The instance of a unit cube we can use to render any cube. Should NOT be used in final version of your project.
    ShaderProgram prog_lambert;// A shader program that uses lambertian reflection
    ShaderProgram prog_flat;// A shader program that uses "flat" reflection (no shadowing at all)

    GLuint vao; // A handle for our vertex array object. This will store the VBOs created in our geometry classes.
                // Don't worry too much about this. Just know it is necessary in order to render geometry.

    Camera gl_camera;
    Scene scene;

    /// Timer linked to timerUpdate(). Fires approx. 60 times per second
    QTimer timer;
    ChunkManager chunkManager;
    Chunk chunk;
    int oldlowerBounderX=0,oldlowerBounderY=0,oldlowerBounderZ=0,
    oldhigherBoundX=64,oldhigherBoundY=64,oldhigherBoundZ=64;
    PlusMark geom_plus;   //add by Xi Yang
    bool first_load = true;
public:
    explicit MyGL(QWidget *parent = 0);
    ~MyGL();

    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();

    void GLDrawScene();
    void updateChunk();
    void pushBlock(Chunk* chunk);
    void updateBlock(Block block);
    ///-----added by Xi Yang---------
    /// -----------------------------
    void BreakBlock();
    void PlaceBlock();
    void TranslateAlongLook_new(float amt);
    void TranslateAlongRight_new(float amt);
    void CollisionCheckLook(float amt);
    void CollisionCheckRight(float amt);
    void GroundCheck();
    float PlayerFalling(int t);
    void PlayerJump();
    ///------------------------------
protected:
    void keyPressEvent(QKeyEvent *e);
    ///-----added by Xi Yang---------
    /// -----------------------------
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    ///------------------------------

private slots:
    /// Slot that gets called ~60 times per second
    virtual void timerUpdate();
};


#endif // MYGL_H
