#include "line.h"

static const int LNE_IDX_COUNT = 2;//760 tris * 3
static const int LNE_VERT_COUNT = 2;

Line::Line(GLWidget277 *context) : Drawable(context)
{
    mode = GL_LINES;
}

void Line::create()
{
    int size = lne_vert_pos.size();

    GLuint lne_idx[size];

    for (int i = 0; i < size; i++)
    {
        lne_idx[i] = i;
        if (i%2)
            lne_vert_col.push_back(glm::vec4(1,1,0,1));
        else
            lne_vert_col.push_back(glm::vec4(1,0,0,1));
    }

    count = size;

    // Create a VBO on our GPU and store its handle in bufIdx
    generateIdx();
    // Tell OpenGL that we want to perform subsequent operations on the VBO referred to by bufIdx
    // and that it will be treated as an element array buffer (since it will contain triangle indices)
    context->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufIdx);
    // Pass the data stored in cyl_idx into the bound buffer, reading a number of bytes equal to
    // SPH_IDX_COUNT multiplied by the size of a GLuint. This data is sent to the GPU to be read by shader programs.
    context->glBufferData(GL_ELEMENT_ARRAY_BUFFER, size * sizeof(GLuint), lne_idx, GL_STATIC_DRAW);

    // The next few sets of function calls are basically the same as above, except bufPos and bufNor are
    // array buffers rather than element array buffers, as they store vertex attributes like position.
    generatePos();
    context->glBindBuffer(GL_ARRAY_BUFFER, bufPos);
    context->glBufferData(GL_ARRAY_BUFFER, size * sizeof(glm::vec4), &lne_vert_pos[0], GL_STATIC_DRAW);

    generateCol();
    context->glBindBuffer(GL_ARRAY_BUFFER, bufCol);
    context->glBufferData(GL_ARRAY_BUFFER, size * sizeof(glm::vec4), &lne_vert_col[0], GL_STATIC_DRAW);
}
