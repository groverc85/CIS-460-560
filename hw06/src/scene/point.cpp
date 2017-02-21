#include "point.h"

static const int PNT_IDX_COUNT = 1;
static const int PNT_VERT_COUNT = 1;


Point::Point(GLWidget277 *context) : Drawable(context)
{
    mode = GL_POINTS;
}


void Point::create()
{
    int size = pnt_vert_pos.size();

    GLuint pnt_idx[size];

    for (int i = 0; i < size; i++)
    {
        pnt_idx[i] = i;
        pnt_vert_col.push_back(glm::vec4(1,1,1,1));
    }

    count = size;

    // Create a VBO on our GPU and store its handle in bufIdx
    generateIdx();
    // Tell OpenGL that we want to perform subsequent operations on the VBO referred to by bufIdx
    // and that it will be treated as an element array buffer (since it will contain triangle indices)
    context->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufIdx);
    // Pass the data stored in cyl_idx into the bound buffer, reading a number of bytes equal to
    // SPH_IDX_COUNT multiplied by the size of a GLuint. This data is sent to the GPU to be read by shader programs.
    context->glBufferData(GL_ELEMENT_ARRAY_BUFFER, size * sizeof(GLuint), pnt_idx, GL_STATIC_DRAW);

    // The next few sets of function calls are basically the same as above, except bufPos and bufNor are
    // array buffers rather than element array buffers, as they store vertex attributes like position.
    generatePos();
    context->glBindBuffer(GL_ARRAY_BUFFER, bufPos);
    context->glBufferData(GL_ARRAY_BUFFER, size * sizeof(glm::vec4), &pnt_vert_pos[0], GL_STATIC_DRAW);

    generateCol();
    context->glBindBuffer(GL_ARRAY_BUFFER, bufCol);
    context->glBufferData(GL_ARRAY_BUFFER, size * sizeof(glm::vec4), &pnt_vert_col[0], GL_STATIC_DRAW);
}
