#ifndef NPC_H
#define NPC_H
#include <drawable.h>
#include <la.h>

#include <QOpenGLContext>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <scene/scene.h>
///------------------------------------------
/// -----------added by Xi Yang--------------
/// -----------------------------------------
enum NPCTYPE{spongebob=0,patrick=1};
enum ACTIONTYPE{wander=0,target=1};

class NPC : public Drawable
{
public:
    NPC(GLWidget277* context);
    NPC(GLWidget277* context,glm::vec3 p,NPCTYPE n,ACTIONTYPE a);
    void setPos(glm::vec3);
    void setNPC(NPCTYPE);
    void setAction(ACTIONTYPE);
    virtual void create();
//private:
    glm::vec3 pos;
    NPCTYPE npcType;
    ACTIONTYPE actType;
    glm::vec3 dir;
    bool isjumping;
    bool iswalking;
    glm::vec3 oldpos;
};

#endif // NPC_H
