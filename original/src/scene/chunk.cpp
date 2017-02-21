#include <scene/chunk.h>
#include <la.h>
#include <iostream>

 Chunk::Chunk(GLWidget277 *context):Drawable(context){

     // Create the blocks
         active= new bool**[CHUNK_SIZE];
         for(int i = 0; i < CHUNK_SIZE; i++)
         {
             active[i] = new bool*[CHUNK_SIZE];

             for(int j = 0; j < CHUNK_SIZE; j++)
             {
                 active[i][j] = new bool[CHUNK_SIZE];
             }
         }
         type= new int**[CHUNK_SIZE];
         for(int i = 0; i < CHUNK_SIZE; i++)
         {
             type[i] = new int*[CHUNK_SIZE];

             for(int j = 0; j < CHUNK_SIZE; j++)
             {
                 type[i][j] = new int[CHUNK_SIZE];
             }
         }
 }
 Chunk::Chunk(GLWidget277 *context, glm::vec3 pos):Drawable(context),pos(pos){
     // Create the blocks
     sizeX=this->pos.x+CHUNK_SIZE;
     sizeY=this->pos.y+CHUNK_SIZE;
     sizeZ=this->pos.z+CHUNK_SIZE;
     active= new bool**[CHUNK_SIZE];
     for(int i = 0; i < CHUNK_SIZE; i++)
     {
         active[i] = new bool*[CHUNK_SIZE];

         for(int j = 0; j < CHUNK_SIZE; j++)
         {
             active[i][j] = new bool[CHUNK_SIZE];
             for(int k=0;k < CHUNK_SIZE; k++){
                 active[i][j][k]=false;
             }
         }
     }
     type= new int**[CHUNK_SIZE];
     for(int i = 0; i < CHUNK_SIZE; i++)
     {
         type[i] = new int*[CHUNK_SIZE];

         for(int j = 0; j < CHUNK_SIZE; j++)
         {
             type[i][j] = new int[CHUNK_SIZE];
         }
     }
 }

Chunk::~Chunk(){
    // Delete the blocks
        for (int i = 0; i < CHUNK_SIZE; ++i)
        {
            for (int j = 0; j < CHUNK_SIZE; ++j)
            {
                delete [] active[i][j];
                delete [] type[i][j];
            }

            delete [] active[i];
            delete [] type[i];
        }
        delete [] active;
        delete [] type;
}

void Chunk::setActive(bool isActive)
{
    m_Active=isActive;
}
bool Chunk::isActive(){
    return m_Active;
}

void Chunk::create(){
    std::vector<glm::vec4>vert_data;
    for (int x = 0; x < CHUNK_SIZE; x++)
        {
            for (int y = 0; y < CHUNK_SIZE; y++)
            {
                for (int z = 0; z < CHUNK_SIZE; z++)
                {
                    if(active[x][y][z] == false)
                    {
                        // Don't create triangle data for inactive blocks
                        continue;
                    }
                  glm::vec3 position;
                  position.x=x;
                  position.y=y;
                  position.z=z;
                 // position.x=x+this->pos.x;
                  //position.y=y+this->pos.y;
                 // position.z=z+this->pos.z;
                  creatCube(position,vert_data);
                }
            }
        }
 int faceNum=vert_data.size()/12;
 int CUB_VERT_COUNT=faceNum*4;
 int CUB_IDX_COUNT=faceNum*6;
//unsigned int cub_idx[CUB_IDX_COUNT];
 std::vector<unsigned int> cub_idx;

 for(int i = 0; i < faceNum; i++){
  cub_idx.push_back(i*4);
  cub_idx.push_back(i*4+1);
  cub_idx.push_back(i*4+2);
  cub_idx.push_back(i*4);
  cub_idx.push_back( i*4+2);
  cub_idx.push_back(i*4+3);
     }

count = CUB_IDX_COUNT;
// Create a VBO on our GPU and store its handle in bufIdx
generateIdx();
// Tell OpenGL that we want to perform subsequent operations on the VBO referred to by bufIdx
// and that it will be treated as an element array buffer (since it will contain triangle indices)
context->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufIdx);
// Pass the data stored in cyl_idx into the bound buffer, reading a number of bytes equal to
// SPH_IDX_COUNT multiplied by the size of a GLuint. This data is sent to the GPU to be read by shader programs.
context->glBufferData(GL_ELEMENT_ARRAY_BUFFER, CUB_IDX_COUNT * sizeof(unsigned int), cub_idx.data(), GL_STATIC_DRAW);

generateVData();
context->glBindBuffer(GL_ARRAY_BUFFER,bufVData);
context->glBufferData(GL_ARRAY_BUFFER, CUB_VERT_COUNT * sizeof(glm::vec4)*3, vert_data.data(), GL_STATIC_DRAW);


int a = 0;

printf("%d",a);
}


void Chunk::creatCube(glm::vec3 pos,std::vector<glm::vec4>(&vert_data)){
    int x=pos.x;
    int y=pos.y;
    int z=pos.z;
    int px=x+this->pos.x;
    int py=y+this->pos.y;
    int pz=z+this->pos.z;
    //Front Face
    if(z+1>=CHUNK_SIZE||active[x][y][z+1]==false){
    vert_data.push_back(glm::vec4(px+0.5f, py+0.5f, pz+0.5f, 1.0f));  //position
    vert_data.push_back(glm::vec4(0,0,1,0));              //normal
    vert_data.push_back(glm::vec4(0,1,0,0));                 //color
    //LR
    vert_data.push_back(glm::vec4(px+0.5f, py-0.5f, pz+0.5f, 1.0f));
    vert_data.push_back(glm::vec4(0,0,1,0));              //normal
    vert_data.push_back(glm::vec4(0,1,0,0));                 //color

    //LL
    vert_data.push_back(glm::vec4(px-0.5f, py-0.5f, pz+0.5f, 1.0f));
    vert_data.push_back(glm::vec4(0,0,1,0));              //normal
    vert_data.push_back(glm::vec4(0,1,0,0));                 //color
    //UL
    vert_data.push_back(glm::vec4(px-0.5f, py+0.5f, pz+0.5f, 1.0f));
    vert_data.push_back(glm::vec4(0,0,1,0));              //normal
    vert_data.push_back(glm::vec4(0,1,0,0));                //color
    }
    //Right face
    if(x+1>=CHUNK_SIZE||active[x+1][y][z]==false){
    //UR
    vert_data.push_back(glm::vec4(px+0.5f, py+0.5f, pz-0.5f, 1.0f));
    vert_data.push_back(glm::vec4(1,0,0,0));              //normal
    vert_data.push_back(glm::vec4(0,1,0,0));                 //color
    //LR
    vert_data.push_back(glm::vec4(px+0.5f, py-0.5f, pz-0.5f, 1.0f));
    vert_data.push_back(glm::vec4(1,0,0,0));              //normal
    vert_data.push_back(glm::vec4(0,1,0,0));                //color
    //LL
    vert_data.push_back(glm::vec4(px+0.5f, py-0.5f, pz+0.5f, 1.0f));
    vert_data.push_back(glm::vec4(1,0,0,0));              //normal
    vert_data.push_back(glm::vec4(0,1,0,0));                //color
    //UL
    vert_data.push_back(glm::vec4(px+0.5f, py+0.5f, pz+0.5f, 1.0f));
    vert_data.push_back(glm::vec4(1,0,0,0));              //normal
    vert_data.push_back(glm::vec4(0,1,0,0));                 //color
    }
    //Left face
    if(x-1<0||active[x-1][y][z]==false){
    //UR
    vert_data.push_back(glm::vec4(px-0.5f, py+0.5f, pz+0.5f, 1.0f));
    vert_data.push_back(glm::vec4(-1,0,0,0));              //normal
    vert_data.push_back(glm::vec4(0,1,0,0));                //color
    //LR
    vert_data.push_back(glm::vec4(px-0.5f, py-0.5f, pz+0.5f, 1.0f));
    vert_data.push_back(glm::vec4(-1,0,0,0));              //normal
    vert_data.push_back(glm::vec4(0,1,0,0));                //color
    //LL
    vert_data.push_back(glm::vec4(px-0.5f, py-0.5f, pz-0.5f, 1.0f));
    vert_data.push_back(glm::vec4(-1,0,0,0));              //normal
    vert_data.push_back(glm::vec4(0,1,0,0));                //color
    //UL
    vert_data.push_back(glm::vec4(px-0.5f, py+0.5f, pz-0.5f, 1.0f));
    vert_data.push_back(glm::vec4(-1,0,0,0));              //normal
    vert_data.push_back(glm::vec4(0,1,0,0));                 //color
      }
    //Back face
    if(z-1<0||active[x][y][z-1]==false){
    //UR
    vert_data.push_back(glm::vec4(px-0.5f, py+0.5f, pz-0.5f, 1.0f));
    vert_data.push_back(glm::vec4(0,0,-1,0));              //normal
    vert_data.push_back(glm::vec4(0,1,0,0));                //color
    //LR
    vert_data.push_back(glm::vec4(px-0.5f, py-0.5f, pz-0.5f, 1.0f));
    vert_data.push_back(glm::vec4(0,0,-1,0));              //normal
    vert_data.push_back(glm::vec4(0,1,0,0));                //color
    //LL
    vert_data.push_back(glm::vec4(px+0.5f, py-0.5f, pz-0.5f, 1.0f));
    vert_data.push_back(glm::vec4(0,0,-1,0));              //normal
    vert_data.push_back(glm::vec4(0,1,0,0));                 //color
    //UL
    vert_data.push_back(glm::vec4(px+0.5f, py+0.5f, pz-0.5f, 1.0f));
    vert_data.push_back(glm::vec4(0,0,-1,0));              //normal
    vert_data.push_back(glm::vec4(0,1,0,0));                 //color
    }
    //Top face
    if(y+1>=CHUNK_SIZE||active[x][y+1][z]==false){
    //UR
    vert_data.push_back(glm::vec4(px+0.5f, py+0.5f, pz-0.5f, 1.0f));
    vert_data.push_back(glm::vec4(0,1,0,0));              //normal
   vert_data.push_back(glm::vec4(0,1,0,0));               //color
    //LR
    vert_data.push_back(glm::vec4(px+0.5f, py+0.5f, pz+0.5f, 1.0f));
    vert_data.push_back(glm::vec4(0,1,0,0));              //normal
   vert_data.push_back(glm::vec4(0,1,0,0));                 //color
    //LL
    vert_data.push_back(glm::vec4(px-0.5f, py+0.5f, pz+0.5f, 1.0f));
    vert_data.push_back(glm::vec4(0,1,0,0));              //normal
   vert_data.push_back(glm::vec4(0,1,0,0));               //color
    //UL
    vert_data.push_back(glm::vec4(px-0.5f, py+0.5f, pz-0.5f, 1.0f));
    vert_data.push_back(glm::vec4(0,1,0,0));              //normal
    vert_data.push_back(glm::vec4(0,1,0,0));                //color
      }
    //Bottom face
    if(y-1<0||active[x][y-1][z]==false){
    //UR
    vert_data.push_back(glm::vec4(px+0.5f, py-0.5f, pz+0.5f, 1.0f));
    vert_data.push_back(glm::vec4(0,-1,0,0));              //normal
    vert_data.push_back(glm::vec4(0,1,0,0));;                 //color
    //LR
    vert_data.push_back(glm::vec4(px+0.5f, py-0.5f, pz-0.5f, 1.0f));
    vert_data.push_back(glm::vec4(0,-1,0,0));              //normal
    vert_data.push_back(glm::vec4(0,1,0,0));                //color
    //LR
    //LL
    vert_data.push_back(glm::vec4(px-0.5f, py-0.5f, pz-0.5f, 1.0f));
    vert_data.push_back(glm::vec4(0,-1,0,0));              //normal
    vert_data.push_back(glm::vec4(0,1,0,0));                //color
    //LR
    //UL
    vert_data.push_back(glm::vec4(px-0.5f, py-0.5f, pz+0.5f, 1.0f));
    vert_data.push_back(glm::vec4(0,-1,0,0));              //normal
    vert_data.push_back(glm::vec4(0,1,0,0));                 //color
    }
}
void Chunk::setPos(glm::vec3 pos){
    this->pos=pos;
}
glm::vec3 Chunk::getPos(){
    return pos;
}
