#ifndef MESH_H
#define MESH_H

#include "drawable.h"
#include <la.h>
#include "meshcomponent.h"
#include "scene/joint.h"

class Mesh : public Drawable
{
public:
    std::vector<Vertex*> Vertices;
    std::vector<HalfEdge*> HalfEdges;
    std::vector<Face*> Faces;

    std::vector<glm::vec4> mesh_vert_col;
    std::vector<glm::vec4> mesh_vert_pos;
    std::vector<glm::vec4> mesh_vert_nor;

    Mesh(GLWidget277 *context);
    virtual void create();
    int getIDX();
    void LoadCube();
    void bindVertex(Joint* root);
    void updateVertex(Joint* root, ShaderProgram &prog_flat);
};

#endif // MESH_H
