#include "face_draw.h"

Face_draw::Face_draw(GLWidget277 *context) : Drawable(context)
{
    mode = GL_LINES;
}

void Face_draw::create()
{
    int size = 2 * fce_vert_pos.size();

//    printf("Face_draw size:%d\n", size);

    GLuint fce_idx[size];

    for (int i = 0; i < size; i++)
    {
        fce_idx[i] = i;
    }

    count = size * 2;

    // Create a VBO on our GPU and store its handle in bufIdx
    generateIdx();
    // Tell OpenGL that we want to perform subsequent operations on the VBO referred to by bufIdx
    // and that it will be treated as an element array buffer (since it will contain triangle indices)
    context->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufIdx);
    // Pass the data stored in cyl_idx into the bound buffer, reading a number of bytes equal to
    // SPH_IDX_COUNT multiplied by the size of a GLuint. This data is sent to the GPU to be read by shader programs.
    context->glBufferData(GL_ELEMENT_ARRAY_BUFFER, size * sizeof(GLuint), fce_idx, GL_STATIC_DRAW);

    // The next few sets of function calls are basically the same as above, except bufPos and bufNor are
    // array buffers rather than element array buffers, as they store vertex attributes like position.
    generatePos();
    context->glBindBuffer(GL_ARRAY_BUFFER, bufPos);
    context->glBufferData(GL_ARRAY_BUFFER, size * sizeof(glm::vec4), &fce_vert_pos[0], GL_STATIC_DRAW);

    generateCol();
    context->glBindBuffer(GL_ARRAY_BUFFER, bufCol);
    context->glBufferData(GL_ARRAY_BUFFER, size * sizeof(glm::vec4), &fce_vert_col[0], GL_STATIC_DRAW);

}
