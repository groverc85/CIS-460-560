#ifndef MESH_H
#define MESH_H

#include <drawable.h>
#include <la.h>
#include <meshcomponent.h>

#include <QOpenGLContext>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <mesh.h>

class Cube : public Drawable {
public:
    Cube(GLWidget277 *context);
    virtual void create();
};


#endif // MESH_H
