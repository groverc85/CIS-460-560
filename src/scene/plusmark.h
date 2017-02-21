#ifndef PLUSMARK_H
#define PLUSMARK_H
#include "drawable.h"
#include <la.h>

#include <QOpenGLContext>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
///------------------------------------------
/// -----------added by Xi Yang--------------
/// ---Draw plus mark in the screen center---
/// -----------------------------------------
class PlusMark : public Drawable
{
public:
    PlusMark(GLWidget277* context);
    virtual void create();
    virtual GLenum drawMode();
    void SetPlusLength(float);
    glm::ivec2 frameLW;
private:
    float pluslength;
};

#endif // PLUSMARK_H
