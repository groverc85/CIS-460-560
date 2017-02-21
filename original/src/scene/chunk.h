#pragma once
#include <drawable.h>
#include <la.h>

#include <QOpenGLContext>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <scene/scene.h>
class Chunk:public Drawable
{
public:
    Chunk(GLWidget277* context);
    Chunk (GLWidget277 *context,glm::vec3 pos);
    ~Chunk();
    virtual void create();
    int sizeX;
    int sizeY;
    int sizeZ;
    static const int CHUNK_SIZE = 16;
    bool*** active;
    int *** type;
    void setActive(bool isActive);
    bool isActive();
    void setPos(glm::vec3 pos);
    glm::vec3 getPos();
private:
    bool m_Active;
    glm::vec3 pos;
   // void creatCube(Block*** m_pBlocks,glm::vec3 pos,std::vector<glm::vec4>(&vert_data));
      void creatCube(glm::vec3 pos,std::vector<glm::vec4>(&vert_data));
};
