#pragma once

#include "drawable.h"

#include <QOpenGLContext>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <la.h>

class Cube : public Drawable
{
public:
    Cube(GLWidget277 *context);
    virtual void create();
};
