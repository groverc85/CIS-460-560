
#pragma once
#include <la.h>
#include<scene/chunk.h>
#include<scene/scene.h>
#include<camera.h>
class ChunkManager
{
public:
    ChunkManager();
    ~ChunkManager();
    std::vector<Chunk*> allChunks;
    std::vector<Chunk*> updateList;
    //std::vector<Chunk*>newLoadList;
    std::vector<Chunk*> renderList;
    void updatenewLoadList();
    void updateUpdateList();
    void updateRenderList(Camera camera);
    void update(Camera camera);
};
