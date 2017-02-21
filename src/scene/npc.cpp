#include "npc.h"

NPC::NPC(GLWidget277 *context):Drawable(context)
{

}
NPC::NPC(GLWidget277 *context, glm::vec3 p, NPCTYPE n, ACTIONTYPE a):
    Drawable(context),pos(p),npcType(n),actType(a),
    dir(glm::vec3(1.0,0,0)),isjumping(false),iswalking(true)
{}
void NPC::setPos(glm::vec3 p)
{
    pos=p;
}
void NPC::setNPC(NPCTYPE i)
{
    if(i<1)
    npcType=i;
}
void NPC::setAction(ACTIONTYPE i)
{
    if(i<1)
    actType=i;
}
void NPC::create()
{
    this->destroy();
    std::vector<glm::vec4> vert_data;
    glm::vec4 sideUV(2,0,2,0);                  //x and y represent uv, z represents cosin power,w is animation
    glm::vec4 topUV(2,0,2,0);
    glm::vec4 bottomUV(7,0,2,0);
    glm::vec4 frontUV(2,0,2,0);
    if(npcType==spongebob)
     {  frontUV=glm::vec4(8,10,0.2,0);
        sideUV=glm::vec4(9,10,0.2,0);
        topUV=glm::vec4(10,10,0.2,0);
        bottomUV=glm::vec4(10,11,0.2,0);}
    else if(npcType==patrick){
        frontUV=glm::vec4(11,10,0.2,0);
        sideUV=glm::vec4(12,10,0.2,0);
        topUV=glm::vec4(13,10,0.2,0);
        bottomUV=glm::vec4(13,11,0.2,0);
    }
    glm::vec4 color(0,1,0,0);
    float px=pos[0];
    float py=pos[1];
    float pz=pos[2];
    //Front Face
        glm::vec3 frontN(0,0,1);
        glm::vec3 frontT=glm::cross(glm::vec3(0,1,0),frontN);

        vert_data.push_back(glm::vec4(px+0.5f, py+0.5f, pz+0.5f, 1.0f));  //position
        vert_data.push_back(glm::vec4(frontN,0));              //normal
        vert_data.push_back(color);                 //color
        vert_data.push_back(glm::vec4((sideUV.x+1)*0.0625,(sideUV.y)*0.0625,sideUV.z,sideUV.w));     //uv
        vert_data.push_back(glm::vec4(frontT,0));                              //TANGENT
        //LR
        vert_data.push_back(glm::vec4(px+0.5f, py-1.5f, pz+0.5f, 1.0f));
        vert_data.push_back(glm::vec4(frontN,0));              //normal
        vert_data.push_back(color);                 //color
        vert_data.push_back(glm::vec4((sideUV.x+1)*0.0625,(sideUV.y+2)*0.0625,sideUV.z,sideUV.w));     //uv
        vert_data.push_back(glm::vec4(frontT,0));                              //TANGENT
        //LL
        vert_data.push_back(glm::vec4(px-0.5f, py-1.5f, pz+0.5f, 1.0f));
        vert_data.push_back(glm::vec4(frontN,0));              //normal
        vert_data.push_back(color);                 //color
        vert_data.push_back(glm::vec4(sideUV.x*0.0625,(sideUV.y+2)*0.0625,sideUV.z,sideUV.w));     //uv
        vert_data.push_back(glm::vec4(frontT,0));                              //TANGENT
        //UL
        vert_data.push_back(glm::vec4(px-0.5f, py+0.5f, pz+0.5f, 1.0f));
        vert_data.push_back(glm::vec4(frontN,0));              //normal
        vert_data.push_back(color);                 //color
        vert_data.push_back(glm::vec4(sideUV.x*0.0625,(sideUV.y)*0.0625,sideUV.z,sideUV.w));     //uv
        vert_data.push_back(glm::vec4(frontT,0));                              //TANGENT

    //Right face
        glm::vec3 rightN(1,0,0);
        glm::vec3 rightT=glm::cross(glm::vec3(0,1,0),rightN);
    //UR
    vert_data.push_back(glm::vec4(px+0.5f, py+0.5f, pz-0.5f, 1.0f));
    vert_data.push_back(glm::vec4(rightN,0));                          //normal
    vert_data.push_back(color);                           //color
    vert_data.push_back(glm::vec4((frontUV.x+1)*0.0625,(frontUV.y)*0.0625,frontUV.z,frontUV.w));     //uv
    vert_data.push_back(glm::vec4(rightT,0));                              //TANGENT
    //LR
    vert_data.push_back(glm::vec4(px+0.5f, py-1.5f, pz-0.5f, 1.0f));
    vert_data.push_back(glm::vec4(rightN,0));                          //normal
    vert_data.push_back(color);                            //color
    vert_data.push_back(glm::vec4((frontUV.x+1)*0.0625,(frontUV.y+2)*0.0625,frontUV.z,frontUV.w));     //uv
    vert_data.push_back(glm::vec4(rightT,0));                              //TANGENT
    //LL
    vert_data.push_back(glm::vec4(px+0.5f, py-1.5f, pz+0.5f, 1.0f));
   vert_data.push_back(glm::vec4(rightN,0));                          //normal
    vert_data.push_back(color);                              //color
    vert_data.push_back(glm::vec4((frontUV.x)*0.0625,(frontUV.y+2)*0.0625,frontUV.z,frontUV.w));     //uv
    vert_data.push_back(glm::vec4(rightT,0));                              //TANGENT
    //UL
    vert_data.push_back(glm::vec4(px+0.5f, py+0.5f, pz+0.5f, 1.0f));
    vert_data.push_back(glm::vec4(rightN,0));                          //normal
    vert_data.push_back(color);                              //color
    vert_data.push_back(glm::vec4((frontUV.x)*0.0625,(frontUV.y)*0.0625,frontUV.z,frontUV.w));     //uv
    vert_data.push_back(glm::vec4(rightT,0));                              //TANGENT


    //Left face

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
    vert_data.push_back(glm::vec4(px-0.5f, py-1.5f, pz+0.5f, 1.0f));
    vert_data.push_back(leftN);                           //normal
    vert_data.push_back(color);                            //color
    vert_data.push_back(glm::vec4((sideUV.x+1)*0.0625,(sideUV.y+2)*0.0625,sideUV.z,sideUV.w));     //uv
    vert_data.push_back(leftT);                              //TANGENT
    //LL
    vert_data.push_back(glm::vec4(px-0.5f, py-1.5f, pz-0.5f, 1.0f));
    vert_data.push_back(leftN);                           //normal
    vert_data.push_back(color);                            //color
    vert_data.push_back(glm::vec4(sideUV.x*0.0625,(sideUV.y+2)*0.0625,sideUV.z,sideUV.w));     //uv
    vert_data.push_back(leftT);                              //TANGENT
    //UL
    vert_data.push_back(glm::vec4(px-0.5f, py+0.5f, pz-0.5f, 1.0f));
    vert_data.push_back(leftN);                           //normal
    vert_data.push_back(color);                            //color
    vert_data.push_back(glm::vec4((sideUV.x)*0.0625,(sideUV.y)*0.0625,sideUV.z,sideUV.w));     //uv
    vert_data.push_back(leftT);                              //TANGENT

    //Back face

        glm::vec4 backN(0,0,-1,0);
        glm::vec4 backT=glm::vec4(glm::cross(glm::vec3(0,1,0),glm::vec3(backN)),0);
    //UR
    vert_data.push_back(glm::vec4(px-0.5f, py+0.5f, pz-0.5f, 1.0f));
    vert_data.push_back(backN);                             //normal
    vert_data.push_back(color);                             //color
    vert_data.push_back(glm::vec4(sideUV.x*0.0625,(sideUV.y)*0.0625,sideUV.z,sideUV.w));     //uv
    vert_data.push_back(backT);                              //TANGENT
    //LR
    vert_data.push_back(glm::vec4(px-0.5f, py-1.5f, pz-0.5f, 1.0f));
    vert_data.push_back(backN);                             //normal
    vert_data.push_back(color);                             //color
    vert_data.push_back(glm::vec4(sideUV.x*0.0625,(sideUV.y+2)*0.0625,sideUV.z,sideUV.w));     //uv
    vert_data.push_back(backT);                              //TANGENT
    //LL
    vert_data.push_back(glm::vec4(px+0.5f, py-1.5f, pz-0.5f, 1.0f));
    vert_data.push_back(backN);                             //normal
    vert_data.push_back(color);                             //color
    vert_data.push_back(glm::vec4((sideUV.x+1)*0.0625,(sideUV.y+2)*0.0625,sideUV.z,sideUV.w));     //uv
    vert_data.push_back(backT);                              //TANGENT
    //UL
    vert_data.push_back(glm::vec4(px+0.5f, py+0.5f, pz-0.5f, 1.0f));
    vert_data.push_back(backN);                             //normal
    vert_data.push_back(color);                             //color
    vert_data.push_back(glm::vec4((sideUV.x+1)*0.0625,(sideUV.y)*0.0625,sideUV.z,sideUV.w));     //uv
    vert_data.push_back(backT);                              //TANGENT

    //Top face

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

    //Bottom face

        glm::vec4 bottomN(0,-1,0,0);
        glm::vec4 bottomT(-1,0,0,0);
    //UR
    vert_data.push_back(glm::vec4(px+0.5f, py-1.5f, pz+0.5f, 1.0f));
    vert_data.push_back(bottomN);                            //normal
    vert_data.push_back(color);;                             //color
    vert_data.push_back(glm::vec4((bottomUV.x+1)*0.0625,(bottomUV.y+1)*0.0625,bottomUV.z,bottomUV.w));     //uv
     vert_data.push_back(bottomT);                              //TANGENT
    //LR
    vert_data.push_back(glm::vec4(px+0.5f, py-1.5f, pz-0.5f, 1.0f));
    vert_data.push_back(bottomN);                            //normal
    vert_data.push_back(color);;                             //color
    vert_data.push_back(glm::vec4((bottomUV.x+1)*0.0625,bottomUV.y*0.0625,bottomUV.z,bottomUV.w));     //uv
     vert_data.push_back(bottomT);                              //TANGENT
    //LR
    //LL
    vert_data.push_back(glm::vec4(px-0.5f, py-1.5f, pz-0.5f, 1.0f));
    vert_data.push_back(bottomN);                            //normal
    vert_data.push_back(color);;                             //color
    vert_data.push_back(glm::vec4(bottomUV.x*0.0625,bottomUV.y*0.0625,bottomUV.z,bottomUV.w));     //uv
     vert_data.push_back(bottomT);                              //TANGENT
    //LR
    //UL
    vert_data.push_back(glm::vec4(px-0.5f, py-1.5f, pz+0.5f, 1.0f));
    vert_data.push_back(bottomN);                            //normal
    vert_data.push_back(color);;                             //color
    vert_data.push_back(glm::vec4(bottomUV.x*0.0625,(bottomUV.y+1)*0.0625,bottomUV.z,bottomUV.w));     //uv
     vert_data.push_back(bottomT);                              //TANGENT



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
