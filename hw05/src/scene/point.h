#ifndef POINT_H
#define POINT_H

#include <drawable.h>

class Point : public Drawable
{
public:
    Point(GLWidget277 *context);
    virtual void create();
    std::vector<glm::vec4> pnt_vert_pos;
    std::vector<glm::vec4> pnt_vert_col;
};

#endif // POINT_H
