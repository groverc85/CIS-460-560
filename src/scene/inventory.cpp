#include "inventory.h"
#include <iostream>
const int ColumnNum=8;
Inventory::Inventory(GLWidget277 *context):Drawable(context),frameLW(glm::ivec2(1)),blockList(),selected_BlockType(0)
{
      blockNum[Default]=0;
      blockNum[Grass]=0;
      blockNum[Dirt]=0;
      blockNum[Water]=0;
      blockNum[Stone]=0;
      blockNum[Wood]=0;
      blockNum[Sand]=0;
      blockNum[Lava]=0;
      blockNum[Leaf]=0;
      blockNum[BedRock]=0;
      blockNum[Coal]=0;
      blockNum[IronOre]=0;
      blockNum[Flower]=0;
      for(int i=0;i<ColumnNum;i++)
      {
          NumList[i]=0;
      }

}
void Inventory::AddBlock(BlockType t)
{
    if(blockNum[t]==0)
    {
        if(blockList.size()==ColumnNum)
        {
            blockNum[blockList[0]]=0;
            blockList.erase(blockList.begin());
        }
        blockList.push_back(t);
        this->destroy();
        this->create();
    }
    blockNum[t]++;
    UpdateNumList();
}
void Inventory::UseBlock(BlockType t)
{
    blockNum[t]--;
    if(blockNum[t]==0)
    {
        int index=0;
        for(unsigned int i=0;i<blockList.size();i++)
        {
            if(blockList[i]==t)
            {
                blockList.erase(blockList.begin()+index);
            }else
                index++;
        }
        if((selected_BlockType>0)&&(selected_BlockType>blockList.size()-1))
            selected_BlockType--;
        this->destroy();
        this->create();
    }
    UpdateNumList();
}
void Inventory::UpdateNumList()
{

    unsigned int num=blockList.size();
    for(unsigned int i=0;i<ColumnNum;i++)
    {
        NumList[i]=0;
    }
    for(unsigned int i=0;i<num;i++)
    {
        NumList[i]=blockNum[blockList[i]];
    }

}
void Inventory::create()
{
    this->destroy();
    std::vector<glm::vec4> vert_data;
    glm::vec4 itemframeUV;
    glm::vec4 blockUV(2,0,0.5,0);
    float ratio=frameLW[0]/float(frameLW[1]);
    glm::vec4 color(0,1,0,0);
    float length=0.15;
    float lengthX=0.15*ratio;
    float startx=-0.75;
    float starty=-0.8;
    float lengthblock=length*0.8;
    float lengthblockX=lengthX*0.8;
    for(unsigned int i=0;i<ColumnNum;i++)
    {
    //Front Face
        glm::vec3 frontN(0,0,1);
        glm::vec3 frontT=glm::cross(glm::vec3(0,1,0),frontN);
        float px=startx+lengthX*2*i;
        float py=starty;
        if(i==selected_BlockType)
            itemframeUV=glm::vec4(14,11,0.05,0);
        else
            itemframeUV=glm::vec4(14,11,0.3,0);
        vert_data.push_back(glm::vec4(px+lengthX, py+length, 0.0f, 1.0f));  //position
        vert_data.push_back(glm::vec4(frontN,0));              //normal
        vert_data.push_back(color);                 //color
        vert_data.push_back(glm::vec4((itemframeUV.x+1)*0.0625,(itemframeUV.y)*0.0625,itemframeUV.z,itemframeUV.w));     //uv
        vert_data.push_back(glm::vec4(frontT,0));                              //TANGENT
        //LR
        vert_data.push_back(glm::vec4(px+lengthX, py-length, 0.0f, 1.0f));
        vert_data.push_back(glm::vec4(frontN,0));              //normal
        vert_data.push_back(color);                 //color
        vert_data.push_back(glm::vec4((itemframeUV.x+1)*0.0625,(itemframeUV.y+1)*0.0625,itemframeUV.z,itemframeUV.w));     //uv
        vert_data.push_back(glm::vec4(frontT,0));                              //TANGENT
        //LL
        vert_data.push_back(glm::vec4(px-lengthX, py-length, 0.0f, 1.0f));
        vert_data.push_back(glm::vec4(frontN,0));              //normal
        vert_data.push_back(color);                 //color
        vert_data.push_back(glm::vec4(itemframeUV.x*0.0625,(itemframeUV.y+1)*0.0625,itemframeUV.z,itemframeUV.w));     //uv
        vert_data.push_back(glm::vec4(frontT,0));                              //TANGENT
        //UL
        vert_data.push_back(glm::vec4(px-lengthX, py+length, 0.0f, 1.0f));
        vert_data.push_back(glm::vec4(frontN,0));              //normal
        vert_data.push_back(color);                 //color
        vert_data.push_back(glm::vec4(itemframeUV.x*0.0625,(itemframeUV.y)*0.0625,itemframeUV.z,itemframeUV.w));     //uv
        vert_data.push_back(glm::vec4(frontT,0));
    }
    for(unsigned int i=0;i<blockList.size();i++)
    {
        if(blockList[i]==Water)
            blockUV=glm::vec4(14,12,0.2,0);
        else if(blockList[i]==Grass)
            blockUV=glm::vec4(3,0,2 ,0);
        else if(blockList[i]==Stone)
            blockUV=glm::vec4(1,0,0.4,0);
        else if(blockList[i]==Lava)
            blockUV=glm::vec4(14,14,0.2,0);
        else if(blockList[i]==Dirt)
            blockUV=glm::vec4(2,0,1,0);
        else if(blockList[i]==Wood)
            blockUV=glm::vec4(4,1,2,0);
        else if(blockList[i]==Coal)
            blockUV=glm::vec4(2,2,3,0);
        else if(blockList[i]==IronOre)
            blockUV=glm::vec4(2,1,1,0);
        else if(blockList[i]==Leaf)
            blockUV=glm::vec4(5,3,1,0);
        else if(blockList[i]==Flower)
            blockUV=glm::vec4(12,0,1,0);
        else if(blockList[i]==Default)
            blockUV=glm::vec4(2,0,2,0);
        //Front Face
            glm::vec3 frontN(0,0,1);
            glm::vec3 frontT=glm::cross(glm::vec3(0,1,0),frontN);
            float px=startx+lengthX*2*i;
            float py=starty;
            vert_data.push_back(glm::vec4(px+lengthblockX, py+lengthblock, 0.0f, 1.0f));  //position
            vert_data.push_back(glm::vec4(frontN,0));              //normal
            vert_data.push_back(color);                 //color
            vert_data.push_back(glm::vec4((blockUV.x+1)*0.0625,(blockUV.y)*0.0625,blockUV.z,blockUV.w));     //uv
            vert_data.push_back(glm::vec4(frontT,0));                              //TANGENT
            //LR
            vert_data.push_back(glm::vec4(px+lengthblockX, py-lengthblock, 0.0f, 1.0f));
            vert_data.push_back(glm::vec4(frontN,0));              //normal
            vert_data.push_back(color);                 //color
            vert_data.push_back(glm::vec4((blockUV.x+1)*0.0625,(blockUV.y+1)*0.0625,blockUV.z,blockUV.w));     //uv
            vert_data.push_back(glm::vec4(frontT,0));                              //TANGENT
            //LL
            vert_data.push_back(glm::vec4(px-lengthblockX, py-lengthblock, 0.0f, 1.0f));
            vert_data.push_back(glm::vec4(frontN,0));              //normal
            vert_data.push_back(color);                 //color
            vert_data.push_back(glm::vec4(blockUV.x*0.0625,(blockUV.y+1)*0.0625,blockUV.z,blockUV.w));     //uv
            vert_data.push_back(glm::vec4(frontT,0));                              //TANGENT
            //UL
            vert_data.push_back(glm::vec4(px-lengthblockX, py+lengthblock, 0.0f, 1.0f));
            vert_data.push_back(glm::vec4(frontN,0));              //normal
            vert_data.push_back(color);                 //color
            vert_data.push_back(glm::vec4(blockUV.x*0.0625,(blockUV.y)*0.0625,blockUV.z,blockUV.w));     //uv
            vert_data.push_back(glm::vec4(frontT,0));
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
