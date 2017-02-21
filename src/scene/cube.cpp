#include "cube.h"
#include <la.h>
#include <iostream>

static const int CUB_IDX_COUNT = 36;
static const int CUB_VERT_COUNT = 24;

glm::vec4 GetCubeNormal(const glm::vec4& P)
{
    int idx = 0;
    float val = -1;
    for(int i = 0; i < 3; i++){
        if(glm::abs(P[i]) > val){
            idx = i;
            val = glm::abs(P[i]);
        }
    }
    glm::vec4 N(0,0,0,0);
    N[idx] = glm::sign(P[idx]);
    return N;
}

 void createCubeVertexData(glm::vec4 (&cub_vert_data)[CUB_VERT_COUNT*3]){
int idx =0;
//Front face
//UR
cub_vert_data[idx++]=glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);  //position
cub_vert_data[idx++]= glm::vec4(0,0,1,0);              //normal
cub_vert_data[idx++]=glm::vec4(0,0,1,0);                 //color
//LR
cub_vert_data[idx++] = glm::vec4(0.5f, -0.5f, 0.5f, 1.0f);
cub_vert_data[idx++]= glm::vec4(0,0,1,0);              //normal
cub_vert_data[idx++]=glm::vec4(0,0,1,0);                 //color

//LL
cub_vert_data[idx++]  = glm::vec4(-0.5f, -0.5f, 0.5f, 1.0f);
cub_vert_data[idx++]= glm::vec4(0,0,1,0);              //normal
cub_vert_data[idx++]=glm::vec4(0,0,1,0);                 //color
//UL
cub_vert_data[idx++]  = glm::vec4(-0.5f, 0.5f, 0.5f, 1.0f);
cub_vert_data[idx++]= glm::vec4(0,0,1,0);              //normal
cub_vert_data[idx++]=glm::vec4(0,0,1,0);                 //color
//Right face
//UR
cub_vert_data[idx++]  = glm::vec4(0.5f, 0.5f, -0.5f, 1.0f);
cub_vert_data[idx++]= glm::vec4(1,0,0,0);              //normal
cub_vert_data[idx++]=glm::vec4(0,0,1,0);                 //color
//LR
cub_vert_data[idx++]  = glm::vec4(0.5f, -0.5f, -0.5f, 1.0f);
cub_vert_data[idx++]= glm::vec4(1,0,0,0);              //normal
cub_vert_data[idx++]=glm::vec4(0,0,1,0);                 //color
//LL
cub_vert_data[idx++]  = glm::vec4(0.5f, -0.5f, 0.5f, 1.0f);
cub_vert_data[idx++]= glm::vec4(1,0,0,0);              //normal
cub_vert_data[idx++]=glm::vec4(0,0,1,0);                 //color
//UL
cub_vert_data[idx++]  = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
cub_vert_data[idx++]= glm::vec4(1,0,0,0);              //normal
cub_vert_data[idx++]=glm::vec4(0,0,1,0);                 //color

//Left face
//UR
cub_vert_data[idx++]  = glm::vec4(-0.5f, 0.5f, 0.5f, 1.0f);
cub_vert_data[idx++]= glm::vec4(-1,0,0,0);              //normal
cub_vert_data[idx++]=glm::vec4(0,0,1,0);                 //color
//LR
cub_vert_data[idx++]  = glm::vec4(-0.5f, -0.5f, 0.5f, 1.0f);
cub_vert_data[idx++]= glm::vec4(-1,0,0,0);              //normal
cub_vert_data[idx++]=glm::vec4(0,0,1,0);                 //color
//LL
cub_vert_data[idx++]  = glm::vec4(-0.5f, -0.5f, -0.5f, 1.0f);
cub_vert_data[idx++]= glm::vec4(-1,0,0,0);              //normal
cub_vert_data[idx++]=glm::vec4(0,0,1,0);                 //color
//UL
cub_vert_data[idx++]  = glm::vec4(-0.5f, 0.5f, -0.5f, 1.0f);
cub_vert_data[idx++]= glm::vec4(-1,0,0,0);              //normal
cub_vert_data[idx++]=glm::vec4(0,0,1,0);                 //color

//Back face
//UR
cub_vert_data[idx++]  = glm::vec4(-0.5f, 0.5f, -0.5f, 1.0f);
cub_vert_data[idx++]= glm::vec4(0,0,-1,0);              //normal
cub_vert_data[idx++]=glm::vec4(0,0,1,0);                 //color
//LR
cub_vert_data[idx++]  = glm::vec4(-0.5f, -0.5f, -0.5f, 1.0f);
cub_vert_data[idx++]= glm::vec4(0,0,-1,0);              //normal
cub_vert_data[idx++]=glm::vec4(0,0,1,0);                 //color
//LL
cub_vert_data[idx++]  = glm::vec4(0.5f, -0.5f, -0.5f, 1.0f);
cub_vert_data[idx++]= glm::vec4(0,0,-1,0);              //normal
cub_vert_data[idx++]=glm::vec4(0,0,1,0);                 //color
//UL
cub_vert_data[idx++]  = glm::vec4(0.5f, 0.5f, -0.5f, 1.0f);
cub_vert_data[idx++]= glm::vec4(0,0,-1,0);              //normal
cub_vert_data[idx++]=glm::vec4(0,0,1,0);                 //color

//Top face
//UR
cub_vert_data[idx++]  = glm::vec4(0.5f, 0.5f, -0.5f, 1.0f);
cub_vert_data[idx++]= glm::vec4(0,1,0,0);              //normal
cub_vert_data[idx++]=glm::vec4(0,0,1,0);                 //color
//LR
cub_vert_data[idx++]  = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
cub_vert_data[idx++]= glm::vec4(0,1,0,0);              //normal
cub_vert_data[idx++]=glm::vec4(0,0,1,0);                 //color
//LL
cub_vert_data[idx++]  = glm::vec4(-0.5f, 0.5f, 0.5f, 1.0f);
cub_vert_data[idx++]= glm::vec4(0,1,0,0);              //normal
cub_vert_data[idx++]=glm::vec4(0,0,1,0);                 //color
//UL
cub_vert_data[idx++]  = glm::vec4(-0.5f, 0.5f, -0.5f, 1.0f);
cub_vert_data[idx++]= glm::vec4(0,1,0,0);              //normal
cub_vert_data[idx++]=glm::vec4(0,0,1,0);                 //color

//Bottom face
//UR
cub_vert_data[idx++]  = glm::vec4(0.5f, -0.5f, 0.5f, 1.0f);
cub_vert_data[idx++]= glm::vec4(0,-1,0,0);              //normal
cub_vert_data[idx++]=glm::vec4(0,0,1,0);                 //color
//LR
cub_vert_data[idx++]  = glm::vec4(0.5f, -0.5f, -0.5f, 1.0f);
cub_vert_data[idx++]= glm::vec4(0,-1,0,0);              //normal
cub_vert_data[idx++]=glm::vec4(0,0,1,0);                 //color
//LR
//LL
cub_vert_data[idx++]  = glm::vec4(-0.5f, -0.5f, -0.5f, 1.0f);
cub_vert_data[idx++]= glm::vec4(0,-1,0,0);              //normal
cub_vert_data[idx++]=glm::vec4(0,0,1,0);                 //color
//LR
//UL
cub_vert_data[idx++]  = glm::vec4(-0.5f, -0.5f, 0.5f, 1.0f);
cub_vert_data[idx++]= glm::vec4(0,-1,0,0);              //normal
cub_vert_data[idx++]=glm::vec4(0,0,1,0);                 //color
//LR

}
//These are functions that are only defined in this cpp file. They're used for organizational purposes
//when filling the arrays used to hold the vertex and index data.
void createCubeVertexPositions(glm::vec4 (&cub_vert_pos)[CUB_VERT_COUNT])
{
    int idx = 0;
    //Front face
    //UR
    cub_vert_pos[idx++] = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
    //LR
    cub_vert_pos[idx++] = glm::vec4(0.5f, -0.5f, 0.5f, 1.0f);
    //LL
    cub_vert_pos[idx++] = glm::vec4(-0.5f, -0.5f, 0.5f, 1.0f);
    //UL
    cub_vert_pos[idx++] = glm::vec4(-0.5f, 0.5f, 0.5f, 1.0f);

    //Right face
    //UR
    cub_vert_pos[idx++] = glm::vec4(0.5f, 0.5f, -0.5f, 1.0f);
    //LR
    cub_vert_pos[idx++] = glm::vec4(0.5f, -0.5f, -0.5f, 1.0f);
    //LL
    cub_vert_pos[idx++] = glm::vec4(0.5f, -0.5f, 0.5f, 1.0f);
    //UL
    cub_vert_pos[idx++] = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);

    //Left face
    //UR
    cub_vert_pos[idx++] = glm::vec4(-0.5f, 0.5f, 0.5f, 1.0f);
    //LR
    cub_vert_pos[idx++] = glm::vec4(-0.5f, -0.5f, 0.5f, 1.0f);
    //LL
    cub_vert_pos[idx++] = glm::vec4(-0.5f, -0.5f, -0.5f, 1.0f);
    //UL
    cub_vert_pos[idx++] = glm::vec4(-0.5f, 0.5f, -0.5f, 1.0f);

    //Back face
    //UR
    cub_vert_pos[idx++] = glm::vec4(-0.5f, 0.5f, -0.5f, 1.0f);
    //LR
    cub_vert_pos[idx++] = glm::vec4(-0.5f, -0.5f, -0.5f, 1.0f);
    //LL
    cub_vert_pos[idx++] = glm::vec4(0.5f, -0.5f, -0.5f, 1.0f);
    //UL
    cub_vert_pos[idx++] = glm::vec4(0.5f, 0.5f, -0.5f, 1.0f);

    //Top face
    //UR
    cub_vert_pos[idx++] = glm::vec4(0.5f, 0.5f, -0.5f, 1.0f);
    //LR
    cub_vert_pos[idx++] = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
    //LL
    cub_vert_pos[idx++] = glm::vec4(-0.5f, 0.5f, 0.5f, 1.0f);
    //UL
    cub_vert_pos[idx++] = glm::vec4(-0.5f, 0.5f, -0.5f, 1.0f);

    //Bottom face
    //UR
    cub_vert_pos[idx++] = glm::vec4(0.5f, -0.5f, 0.5f, 1.0f);
    //LR
    cub_vert_pos[idx++] = glm::vec4(0.5f, -0.5f, -0.5f, 1.0f);
    //LL
    cub_vert_pos[idx++] = glm::vec4(-0.5f, -0.5f, -0.5f, 1.0f);
    //UL
    cub_vert_pos[idx++] = glm::vec4(-0.5f, -0.5f, 0.5f, 1.0f);
}


void createCubeVertexNormals(glm::vec4 (&cub_vert_nor)[CUB_VERT_COUNT])
{
    int idx = 0;
    //Front
    for(int i = 0; i < 4; i++){
        cub_vert_nor[idx++] = glm::vec4(0,0,1,0);
    }
    //Right
    for(int i = 0; i < 4; i++){
        cub_vert_nor[idx++] = glm::vec4(1,0,0,0);
    }
    //Left
    for(int i = 0; i < 4; i++){
        cub_vert_nor[idx++] = glm::vec4(-1,0,0,0);
    }
    //Back
    for(int i = 0; i < 4; i++){
        cub_vert_nor[idx++] = glm::vec4(0,0,-1,0);
    }
    //Top
    for(int i = 0; i < 4; i++){
        cub_vert_nor[idx++] = glm::vec4(0,1,0,0);
    }
    //Bottom
    for(int i = 0; i < 4; i++){
        cub_vert_nor[idx++] = glm::vec4(0,-1,0,0);
    }
}

void createCubeIndices(GLuint (&cub_idx)[CUB_IDX_COUNT])
{
    int idx = 0;
    for(int i = 0; i < 6; i++){
        cub_idx[idx++] = i*4;
        cub_idx[idx++] = i*4+1;
        cub_idx[idx++] = i*4+2;
        cub_idx[idx++] = i*4;
        cub_idx[idx++] = i*4+2;
        cub_idx[idx++] = i*4+3;
    }
}

void Cube::create()
{
    GLuint sph_idx[CUB_IDX_COUNT];
    glm::vec4 sph_vert_pos[CUB_VERT_COUNT];
    glm::vec4 sph_vert_nor[CUB_VERT_COUNT];
    glm::vec4 cub_vert_col[CUB_VERT_COUNT];
    glm::vec4 cub_vert_data[CUB_VERT_COUNT*3];
   // VertexData vertData[CUB_VERT_COUNT];
    createCubeVertexPositions(sph_vert_pos);
    createCubeVertexNormals(sph_vert_nor);
    createCubeIndices(sph_idx);
    createCubeVertexData(cub_vert_data);

    for(int i = 0; i < CUB_VERT_COUNT; i++){
        cub_vert_col[i] = glm::vec4(0.2f, 1.0f, 0.6f, 1);
    }

    count = CUB_IDX_COUNT;

    // Create a VBO on our GPU and store its handle in bufIdx
    generateIdx();
    // Tell OpenGL that we want to perform subsequent operations on the VBO referred to by bufIdx
    // and that it will be treated as an element array buffer (since it will contain triangle indices)
    context->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufIdx);
    // Pass the data stored in cyl_idx into the bound buffer, reading a number of bytes equal to
    // SPH_IDX_COUNT multiplied by the size of a GLuint. This data is sent to the GPU to be read by shader programs.
    context->glBufferData(GL_ELEMENT_ARRAY_BUFFER, CUB_IDX_COUNT * sizeof(GLuint), sph_idx, GL_STATIC_DRAW);

    // The next few sets of function calls are basically the same as above, except bufPos and bufNor are
    // array buffers rather than element array buffers, as they store vertex attributes like position.
    /*
    generatePos();
    context->glBindBuffer(GL_ARRAY_BUFFER, bufPos);
    context->glBufferData(GL_ARRAY_BUFFER, CUB_VERT_COUNT * sizeof(glm::vec4), sph_vert_pos, GL_STATIC_DRAW);

    generateNor();
    context->glBindBuffer(GL_ARRAY_BUFFER, bufNor);
    context->glBufferData(GL_ARRAY_BUFFER, CUB_VERT_COUNT * sizeof(glm::vec4), sph_vert_nor, GL_STATIC_DRAW);

    generateCol();
    context->glBindBuffer(GL_ARRAY_BUFFER, bufCol);
    context->glBufferData(GL_ARRAY_BUFFER, CUB_VERT_COUNT * sizeof(glm::vec4), cub_vert_col, GL_STATIC_DRAW);
      */
       generateVData();
       context->glBindBuffer(GL_ARRAY_BUFFER,bufVData);
       context->glBufferData(GL_ARRAY_BUFFER, CUB_VERT_COUNT * sizeof(glm::vec4)*3, cub_vert_data, GL_STATIC_DRAW);
}
