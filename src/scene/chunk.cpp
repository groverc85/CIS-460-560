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
                  int t=type[x][y][z];
                 // position.x=x+this->pos.x;
                  //position.y=y+this->pos.y;
                 // position.z=z+this->pos.z;

                  creatCube(position,vert_data,t);
                }
            }
        }
 int faceNum=vert_data.size()/20;
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
context->glBufferData(GL_ARRAY_BUFFER, CUB_VERT_COUNT * sizeof(glm::vec4)*5, vert_data.data(), GL_STATIC_DRAW);


}


void Chunk::creatCube(glm::vec3 pos,std::vector<glm::vec4>(&vert_data),int type){
    int x=pos.x;
    int y=pos.y;
    int z=pos.z;
    int px=x+this->pos.x;
    int py=y+this->pos.y;
    int pz=z+this->pos.z;
    //std::cout<<"type is"<<type<<std::endl;
    glm::vec4 sideUV(2,0,2,0);                  //x and y represent uv, z represents cosin power,w is animation
    glm::vec4 topUV(2,0,2,0);
    glm::vec4 bottomUV(2,0,2,0);
       if(type==Water){                               //water
             sideUV=glm::vec4(14,12,0.2,1);
             topUV=glm::vec4(14,12,0.2,1);
             bottomUV=glm::vec4(14,12,0.2,1);
       }
       else if(type==GrassLand){                          //GrassLand
           sideUV=glm::vec4(3,0,1,0);
           topUV=glm::vec4(8,2,1,0);
           bottomUV=glm::vec4(2,0,1,0);
       }
       else if(type==Stone){                          //stone
           sideUV=glm::vec4(1,0,0.2,0);
           topUV=glm::vec4(1,0,0.2,0);
           bottomUV=glm::vec4(1,0,0.2,0);
       }
       else if(type==Lava){
           sideUV=glm::vec4(14,14,0.2,1);                    //lava
           topUV=glm::vec4(14,14,0.2,1);
           bottomUV=glm::vec4(14,14,0.2,1);
       }
       else if(type==Dirt){
           sideUV  =glm::vec4(2,0,2,0);
           topUV   =glm::vec4(2,0,2,0);
           bottomUV=glm::vec4(2,0,2,0);                     //Dirt
       }
       else if(type==Wood){
           sideUV=glm::vec4(4,1,2,0);
           topUV=glm::vec4(5,1,2,0);                          //wood
           bottomUV=glm::vec4(5,1,2,0);
       }
       else if(type==BedRock){
           sideUV=glm::vec4(1,1,2,0);
            topUV=glm::vec4(1,1,2,0);                          //BedRock
           bottomUV=glm::vec4(1,1,2,0);
       }
       else if(type==Coal){
           sideUV=glm::vec4(2,2,5,0);
            topUV=glm::vec4(2,2,5,0);                          //Coal
           bottomUV=glm::vec4(2,2,5,0);
       }
       else if(type==IronOre){
           sideUV=glm::vec4(2,1,1,0);
            topUV=glm::vec4(2,1,1,0);                          //IronOre
           bottomUV=glm::vec4(2,1,1,0);
       }
       else if(type==Leaf){
           sideUV=glm::vec4(5,3,1,0);
            topUV=glm::vec4(5,3,1,0);                          //Leaf
           bottomUV=glm::vec4(5,3,1,0);
       }
       else if(type==Flower){
           sideUV=glm::vec4(12,0,1,0);
            topUV=glm::vec4(5,12,1,0);                          //Flower
           bottomUV=glm::vec4(8,2,1,0);
       }
       else if(type==Grass){
           sideUV=glm::vec4(7,2,1,0);
           topUV=glm::vec4(4,11,1,0);                          //Grass
           bottomUV=glm::vec4(4,2,1,0);
       }
       else if(type==Snow){
           sideUV=glm::vec4(4,4,1,0);
            topUV=glm::vec4(2,4,0.1,0);                          //Snow
           bottomUV=glm::vec4(2,0,1,0);
       }
       else if(type==SnowSpike){
           sideUV=glm::vec4(11,0,1,0);
            topUV=glm::vec4(11,0,1,0);                          //SnowSpike
           bottomUV=glm::vec4(11,0,1,0);
       }
       else if(type==Cactus){
           sideUV=glm::vec4(6,4,1,0);
            topUV=glm::vec4(5,4,1,0);                          //Cactus
           bottomUV=glm::vec4(6,4,1,0);
       }
       else if(type==Fire){
           sideUV=glm::vec4(8,12,0.02,1);
            topUV=glm::vec4(8,12,0.02,1);                          //Fire
           bottomUV=glm::vec4(8,12,0.02,1);
       }
       else if(type==WoodOnFire){
           sideUV=glm::vec4(4,1,1,2);
           topUV=glm::vec4(5,1,1,2);                          //wood
           bottomUV=glm::vec4(5,1,1,2);
       }
    glm::vec4 color(0,1,0,0);

    //Front Face
    if(z+1>=CHUNK_SIZE||active[x][y][z+1]==false){
        glm::vec3 frontN(0,0,1);
        glm::vec3 frontT=glm::cross(glm::vec3(0,1,0),frontN);
      //  std::cout<<"front tan is"<<frontT.x<<" "<<frontT.y<<" "<<frontT.z<<std::endl;
    vert_data.push_back(glm::vec4(px+0.5f, py+0.5f, pz+0.5f, 1.0f));  //position
    vert_data.push_back(glm::vec4(frontN,0));              //normal
    vert_data.push_back(color);                 //color
    vert_data.push_back(glm::vec4((sideUV.x+1)*0.0625,(sideUV.y)*0.0625,sideUV.z,sideUV.w));     //uv
    vert_data.push_back(glm::vec4(frontT,0));                              //TANGENT
    //LR
    vert_data.push_back(glm::vec4(px+0.5f, py-0.5f, pz+0.5f, 1.0f));
    vert_data.push_back(glm::vec4(frontN,0));              //normal
    vert_data.push_back(color);                 //color
    vert_data.push_back(glm::vec4((sideUV.x+1)*0.0625,(sideUV.y+1)*0.0625,sideUV.z,sideUV.w));     //uv
    vert_data.push_back(glm::vec4(frontT,0));                              //TANGENT
    //LL
    vert_data.push_back(glm::vec4(px-0.5f, py-0.5f, pz+0.5f, 1.0f));
    vert_data.push_back(glm::vec4(frontN,0));              //normal
    vert_data.push_back(color);                 //color
    vert_data.push_back(glm::vec4(sideUV.x*0.0625,(sideUV.y+1)*0.0625,sideUV.z,sideUV.w));     //uv
    vert_data.push_back(glm::vec4(frontT,0));                              //TANGENT
    //UL
    vert_data.push_back(glm::vec4(px-0.5f, py+0.5f, pz+0.5f, 1.0f));
    vert_data.push_back(glm::vec4(frontN,0));              //normal
    vert_data.push_back(color);                 //color
    vert_data.push_back(glm::vec4(sideUV.x*0.0625,(sideUV.y)*0.0625,sideUV.z,sideUV.w));     //uv
    vert_data.push_back(glm::vec4(frontT,0));                              //TANGENT
    }
    //Right face
    if(x+1>=CHUNK_SIZE||active[x+1][y][z]==false){
        glm::vec3 rightN(1,0,0);
        glm::vec3 rightT=glm::cross(glm::vec3(0,1,0),rightN);
    //UR
    vert_data.push_back(glm::vec4(px+0.5f, py+0.5f, pz-0.5f, 1.0f));
    vert_data.push_back(glm::vec4(rightN,0));                          //normal
    vert_data.push_back(color);                           //color
    vert_data.push_back(glm::vec4((sideUV.x+1)*0.0625,(sideUV.y)*0.0625,sideUV.z,sideUV.w));     //uv
    vert_data.push_back(glm::vec4(rightT,0));                              //TANGENT
    //LR
    vert_data.push_back(glm::vec4(px+0.5f, py-0.5f, pz-0.5f, 1.0f));
    vert_data.push_back(glm::vec4(rightN,0));                          //normal
    vert_data.push_back(color);                            //color
    vert_data.push_back(glm::vec4((sideUV.x+1)*0.0625,(sideUV.y+1)*0.0625,sideUV.z,sideUV.w));     //uv
    vert_data.push_back(glm::vec4(rightT,0));                              //TANGENT
    //LL
    vert_data.push_back(glm::vec4(px+0.5f, py-0.5f, pz+0.5f, 1.0f));
   vert_data.push_back(glm::vec4(rightN,0));                          //normal
    vert_data.push_back(color);                              //color
    vert_data.push_back(glm::vec4((sideUV.x)*0.0625,(sideUV.y+1)*0.0625,sideUV.z,sideUV.w));     //uv
    vert_data.push_back(glm::vec4(rightT,0));                              //TANGENT
    //UL
    vert_data.push_back(glm::vec4(px+0.5f, py+0.5f, pz+0.5f, 1.0f));
    vert_data.push_back(glm::vec4(rightN,0));                          //normal
    vert_data.push_back(color);                              //color
    vert_data.push_back(glm::vec4((sideUV.x)*0.0625,(sideUV.y)*0.0625,sideUV.z,sideUV.w));     //uv
    vert_data.push_back(glm::vec4(rightT,0));                              //TANGENT
    }
    //Left face
    if(x-1<0||active[x-1][y][z]==false){
        glm::vec4 leftN(-1,0,0,0);
        glm::vec4 leftT=glm::vec4(glm::cross(glm::vec3(0,1,0),glm::vec3(leftN)),0);
       // std::cout<<"letf tan is"<<leftT.x<<" "<<leftT.y<<" "<<leftT.z<<std::endl;
    //UR
    vert_data.push_back(glm::vec4(px-0.5f, py+0.5f, pz+0.5f, 1.0f));
    vert_data.push_back(leftN);                           //normal
    vert_data.push_back(color);                            //color
    vert_data.push_back(glm::vec4((sideUV.x+1)*0.0625,(sideUV.y)*0.0625,sideUV.z,sideUV.w));     //uv
    vert_data.push_back(leftT);                              //TANGENT
    //LR
    vert_data.push_back(glm::vec4(px-0.5f, py-0.5f, pz+0.5f, 1.0f));
    vert_data.push_back(leftN);                           //normal
    vert_data.push_back(color);                            //color
    vert_data.push_back(glm::vec4((sideUV.x+1)*0.0625,(sideUV.y+1)*0.0625,sideUV.z,sideUV.w));     //uv
    vert_data.push_back(leftT);                              //TANGENT
    //LL
    vert_data.push_back(glm::vec4(px-0.5f, py-0.5f, pz-0.5f, 1.0f));
    vert_data.push_back(leftN);                           //normal
    vert_data.push_back(color);                            //color
    vert_data.push_back(glm::vec4(sideUV.x*0.0625,(sideUV.y+1)*0.0625,sideUV.z,sideUV.w));     //uv
    vert_data.push_back(leftT);                              //TANGENT
    //UL
    vert_data.push_back(glm::vec4(px-0.5f, py+0.5f, pz-0.5f, 1.0f));
    vert_data.push_back(leftN);                           //normal
    vert_data.push_back(color);                            //color
    vert_data.push_back(glm::vec4((sideUV.x)*0.0625,(sideUV.y)*0.0625,sideUV.z,sideUV.w));     //uv
    vert_data.push_back(leftT);                              //TANGENT
      }
    //Back face
    if(z-1<0||active[x][y][z-1]==false){
        glm::vec4 backN(0,0,-1,0);
        glm::vec4 backT=glm::vec4(glm::cross(glm::vec3(0,1,0),glm::vec3(backN)),0);
    //UR
    vert_data.push_back(glm::vec4(px-0.5f, py+0.5f, pz-0.5f, 1.0f));
    vert_data.push_back(backN);                             //normal
    vert_data.push_back(color);                             //color
    vert_data.push_back(glm::vec4(sideUV.x*0.0625,(sideUV.y)*0.0625,sideUV.z,sideUV.w));     //uv
    vert_data.push_back(backT);                              //TANGENT
    //LR
    vert_data.push_back(glm::vec4(px-0.5f, py-0.5f, pz-0.5f, 1.0f));
    vert_data.push_back(backN);                             //normal
    vert_data.push_back(color);                             //color
    vert_data.push_back(glm::vec4(sideUV.x*0.0625,(sideUV.y+1)*0.0625,sideUV.z,sideUV.w));     //uv
    vert_data.push_back(backT);                              //TANGENT
    //LL
    vert_data.push_back(glm::vec4(px+0.5f, py-0.5f, pz-0.5f, 1.0f));
    vert_data.push_back(backN);                             //normal
    vert_data.push_back(color);                             //color
    vert_data.push_back(glm::vec4((sideUV.x+1)*0.0625,(sideUV.y+1)*0.0625,sideUV.z,sideUV.w));     //uv
    vert_data.push_back(backT);                              //TANGENT
    //UL
    vert_data.push_back(glm::vec4(px+0.5f, py+0.5f, pz-0.5f, 1.0f));
    vert_data.push_back(backN);                             //normal
    vert_data.push_back(color);                             //color
    vert_data.push_back(glm::vec4((sideUV.x+1)*0.0625,(sideUV.y)*0.0625,sideUV.z,sideUV.w));     //uv
    vert_data.push_back(backT);                              //TANGENT
    }
    //Top face
    if(y+1>=CHUNK_SIZE||active[x][y+1][z]==false){
        glm::vec4 topN(0,1,0,0);
        glm::vec4 topT(1,0,0,0);
    //UR
    vert_data.push_back(glm::vec4(px+0.5f, py+0.5f, pz-0.5f, 1.0f));
    vert_data.push_back(topN);                              //normal
    vert_data.push_back(color);                              //color
    vert_data.push_back(glm::vec4((topUV.x+1)*0.0625,topUV.y*0.0625,topUV.z,topUV.w));     //uv
    vert_data.push_back(topT);                              //TANGENT

    //LR
    vert_data.push_back(glm::vec4(px+0.5f, py+0.5f, pz+0.5f, 1.0f));
    vert_data.push_back(topN);                              //normal
    vert_data.push_back(color);                              //color
    vert_data.push_back(glm::vec4((topUV.x+1)*0.0625,(topUV.y+1)*0.0625,topUV.z,topUV.w));     //uv
    vert_data.push_back(topT);                              //TANGENT
    //LL
    vert_data.push_back(glm::vec4(px-0.5f, py+0.5f, pz+0.5f, 1.0f));
    vert_data.push_back(topN);                              //normal
    vert_data.push_back(color);                              //color
   vert_data.push_back(glm::vec4(topUV.x*0.0625,(topUV.y+1)*0.0625,topUV.z,topUV.w));     //uv
   vert_data.push_back(topT);                              //TANGENT
    //UL
    vert_data.push_back(glm::vec4(px-0.5f, py+0.5f, pz-0.5f, 1.0f));
    vert_data.push_back(topN);                              //normal
    vert_data.push_back(color);                              //color
    vert_data.push_back(glm::vec4(topUV.x*0.0625,topUV.y*0.0625,topUV.z,topUV.w));     //uv
    vert_data.push_back(topT);                              //TANGENT
      }
    //Bottom face
    if(y-1<0||active[x][y-1][z]==false){
        glm::vec4 bottomN(0,-1,0,0);
        glm::vec4 bottomT(-1,0,0,0);
    //UR
    vert_data.push_back(glm::vec4(px+0.5f, py-0.5f, pz+0.5f, 1.0f));
    vert_data.push_back(bottomN);                            //normal
    vert_data.push_back(color);;                             //color
    vert_data.push_back(glm::vec4((bottomUV.x+1)*0.0625,(bottomUV.y+1)*0.0625,bottomUV.z,bottomUV.w));     //uv
     vert_data.push_back(bottomT);                              //TANGENT
    //LR
    vert_data.push_back(glm::vec4(px+0.5f, py-0.5f, pz-0.5f, 1.0f));
    vert_data.push_back(bottomN);                            //normal
    vert_data.push_back(color);;                             //color
    vert_data.push_back(glm::vec4((bottomUV.x+1)*0.0625,bottomUV.y*0.0625,bottomUV.z,bottomUV.w));     //uv
     vert_data.push_back(bottomT);                              //TANGENT
    //LR
    //LL
    vert_data.push_back(glm::vec4(px-0.5f, py-0.5f, pz-0.5f, 1.0f));
    vert_data.push_back(bottomN);                            //normal
    vert_data.push_back(color);;                             //color
    vert_data.push_back(glm::vec4(bottomUV.x*0.0625,bottomUV.y*0.0625,bottomUV.z,bottomUV.w));     //uv
     vert_data.push_back(bottomT);                              //TANGENT
    //LR
    //UL
    vert_data.push_back(glm::vec4(px-0.5f, py-0.5f, pz+0.5f, 1.0f));
    vert_data.push_back(bottomN);                            //normal
    vert_data.push_back(color);;                             //color
    vert_data.push_back(glm::vec4(bottomUV.x*0.0625,(bottomUV.y+1)*0.0625,bottomUV.z,bottomUV.w));     //uv
     vert_data.push_back(bottomT);                              //TANGENT
    }
}
void Chunk::setPos(glm::vec3 pos){
    this->pos=pos;
}
glm::vec3 Chunk::getPos(){
    return pos;
}
