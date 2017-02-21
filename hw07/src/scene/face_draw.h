#ifndef FACE_DRAW_H
#define FACE_DRAW_H

#include <drawable.h>

class Face_draw  : public Drawable
{
public:
    Face_draw(GLWidget277 *context);
    virtual void create();
    std::vector<glm::vec4> fce_vert_pos;
    std::vector<glm::vec4> fce_vert_col;
//    std::vector<glm::vec4> fce_vert_nor;
};

#endif // FACE_H
