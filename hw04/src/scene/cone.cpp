#include "cone.h"
#include<la.h>
#include <iostream>


static const int CON_IDX_COUNT = 120;
static const int CON_VERT_COUNT = 41;

Cone::Cone(GLWidget277 *context) : Drawable(context)
{}

void createConeVertexPositions(glm::vec4 (&con_vert_pos)[CON_VERT_COUNT]){

    for(int i = 0; i < 20; i++){
        glm::vec4 v = glm::rotate(glm::mat4(1.0f), (i)*18.0f, glm::vec3(0, 1, 0)) * glm::vec4(0.5f,-0.5f,0,1);
        con_vert_pos[i] = v;
    }

    for(int i = 20; i < 40; i++){
        glm::vec4 v = glm::rotate(glm::mat4(1.0f), (i-20)*18.0f, glm::vec3(0, 1, 0)) * glm::vec4(0.5f,-0.5f,0,1);
        con_vert_pos[i] = v;
    }

    con_vert_pos[40] = glm::vec4(0,0.5f,0,1);
}


void createConeVertexNormals(glm::vec4 (&con_vert_nor)[CON_VERT_COUNT]){
    //Store side normals (IDX 20 - 39)
    for(int i = 0; i < 20; i++){
        glm::vec4 v = glm::rotate(glm::mat4(1.0f), i*18.0f, glm::vec3(0, 1, 0)) * glm::vec4(1,1,0,0);
        con_vert_nor[i] = v;
    }

    //Store bottom cap normals (IDX 20 - 39)
    for(int i = 0; i < 20; i++){
        con_vert_nor[i+20] =  glm::vec4(0,-1,0,0);
    }
}


void createConeIndices(GLuint (&con_idx)[CON_IDX_COUNT]){
    //Build indices for the top cap (18 tris, indices 0 - 53, up to vertex 19)
    for(int i = 0; i < 20; i++){
        con_idx[(i*3)] = i;
        con_idx[(i*3)+1] = i+1;
        con_idx[(i*3)+2] = 40;
    }

    for(int i = 0; i < 20; i++){
        con_idx[(i+20) * 3] = 0;
        con_idx[(i+20) * 3 + 1] = i+1;
        con_idx[(i+20) * 3 + 2] = i+2;
    }
}

void Cone::create()
{
    GLuint con_idx[CON_IDX_COUNT];
    glm::vec4 con_vert_pos[CON_VERT_COUNT];
    glm::vec4 con_vert_nor[CON_VERT_COUNT];

    createConeVertexPositions(con_vert_pos);
    createConeVertexNormals(con_vert_nor);
    createConeIndices(con_idx);

    count = CON_IDX_COUNT;

    // Create a VBO on our GPU and store its handle in bufIdx
    generateIdx();

    // Tell OpenGL that we want to perform subsequent operations on the VBO referred to by bufIdx
    // and that it will be treated as an element array buffer (since it will contain triangle indices)
    context->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufIdx);
    // Pass the data stored in cyl_idx into the bound buffer, reading a number of bytes equal to
    // CYL_IDX_COUNT multiplied by the size of a GLuint. This data is sent to the GPU to be read by shader programs.
    context->glBufferData(GL_ELEMENT_ARRAY_BUFFER, CON_IDX_COUNT * sizeof(GLuint), con_idx, GL_STATIC_DRAW);

    // The next few sets of function calls are basically the same as above, except bufPos and bufNor are
    // array buffers rather than element array buffers, as they store vertex attributes like position.
    generatePos();
    context->glBindBuffer(GL_ARRAY_BUFFER, bufPos);
    context->glBufferData(GL_ARRAY_BUFFER, CON_VERT_COUNT * sizeof(glm::vec4), con_vert_pos, GL_STATIC_DRAW);

    generateNor();
    context->glBindBuffer(GL_ARRAY_BUFFER, bufNor);
    context->glBufferData(GL_ARRAY_BUFFER, CON_VERT_COUNT * sizeof(glm::vec4), con_vert_nor, GL_STATIC_DRAW);
}
