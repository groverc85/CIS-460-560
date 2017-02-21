#pragma once

#include "drawable.h"
#include <la.h>

#include <QOpenGLContext>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>

class Cube : public Drawable
{
public:
    Cube(GLWidget277* context) : Drawable(context){}
    virtual void create();
};
