#ifndef INVENTORY_H
#define INVENTORY_H
#include <drawable.h>
#include <la.h>

#include <QOpenGLContext>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <scene/scene.h>
///------------------------------------------
/// -----------added by Xi Yang--------------
/// -----------------------------------------
class Inventory : public Drawable
{
public:
    Inventory(GLWidget277* context);
    virtual void create();
    glm::ivec2 frameLW;

    std::map<BlockType,int> blockNum;
    std::vector<BlockType> blockList;
    unsigned int selected_BlockType;
    int NumList[8];
    void AddBlock(BlockType);
    void UseBlock(BlockType);
    void UpdateNumList();
};

#endif // INVENTORY_H
