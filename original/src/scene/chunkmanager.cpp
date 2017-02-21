#include <scene/chunkmanager.h>
ChunkManager::ChunkManager(){

}
ChunkManager::~ChunkManager(){
    for(Chunk* chunk:allChunks){
        //delete chunk;
        //chunk->~Chunk();
        chunk->destroy();
    }

    for(Chunk* chunk:updateList){
        chunk->destroy();
    }

    for(Chunk* chunk:renderList){
        chunk->destroy();
    }

    for(Chunk* chunk:newLoadList){
        chunk->destroy();
    }

}

void ChunkManager::updatenewLoadList(){
    for(Chunk* chunk:newLoadList){
        allChunks.push_back(chunk);
    }
       newLoadList.clear();
}
void ChunkManager::updateRenderList(Camera camera){
    glm::vec3 eye=camera.eye;

if(renderList.size()!=0){
    for(Chunk* chunk:renderList){
        chunk->setActive(true);
        glm::vec3 chunkPos=chunk->getPos();
        int taxtab=fabs(eye.x-chunkPos.x)+fabs(eye.y-chunkPos.y);
        glm::vec3 chunk_to_camer=chunkPos-eye;
        int back=glm::dot(chunk_to_camer,camera.look);
        if(taxtab>=512 ||fabs(eye.z-chunkPos.z)>128){
            chunk->setActive(false);
        }


    }
}

}
void ChunkManager::updateUpdateList(Camera camera){

    if(updateList.size()!=0){
        for(Chunk* chunk:updateList){
             chunk->create();
           renderList.push_back(chunk);

        }
    }
   updateList.clear();
}
void ChunkManager::update(Camera camera){
  updateUpdateList(camera);
  updateRenderList(camera);
  updatenewLoadList();
}
