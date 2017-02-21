#pragma once

#include "drawable.h"

#include <QOpenGLContext>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <la.h>

#ifndef CONE_H
#define CONE_H


class Cone : public Drawable
{
public:
    Cone(GLWidget277 *context);
    virtual void create();
};

#endif // CONE_H
