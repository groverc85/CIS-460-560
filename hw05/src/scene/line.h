#ifndef LINE_H
#define LINE_H

#include <drawable.h>

class Line : public Drawable
{
public:
    Line(GLWidget277 *context);
    virtual void create();
    std::vector<glm::vec4> lne_vert_pos;
    std::vector<glm::vec4> lne_vert_col;
};

#endif // LINE_H
