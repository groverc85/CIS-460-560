#pragma once

#include "drawable.h"
#include <la.h>

#include <QOpenGLContext>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>

#ifndef PIPE_H
#define PIPE_H

class Pipe : public Drawable
{
public:
    Pipe(GLWidget277* context);
    virtual void create();
};

#endif // PIPE_H
