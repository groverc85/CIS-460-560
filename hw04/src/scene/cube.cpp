#include "cube.h"
#include <la.h>
#include <iostream>

static const int CUB_IDX_COUNT = 36;
static const int CUB_VERT_COUNT = 24;

Cube::Cube(GLWidget277 *context) : Drawable(context)
{}

//These are functions that are only defined in this cpp file. They're used for organizational purposes
//when filling the arrays used to hold the vertex and index data.
void createCubeVertexPositions(glm::vec4 (&cub_vert_pos)[CUB_VERT_COUNT]) {
    for (int i = 0; i < 4; i++) {
        glm::vec4 v = glm::rotate(glm::mat4(1.0f), glm::radians(i*90.0f), glm::vec3(0, 1, 0)) * glm::vec4(0.5f, 0.5f, 0, 1);
        cub_vert_pos[i] = v;
    }
    for (int i = 4; i < 8; i++) {
        glm::vec4 v = glm::rotate(glm::mat4(1.0f), glm::radians((i-4)*90.0f), glm::vec3(0, 1, 0)) * glm::vec4(0.5f, -0.5f, 0, 1);
        cub_vert_pos[i] = v;
    }

    // GL only supports one normal per vertex so we need some more vertices
    for (int i = 0; i < 4; i++) {
        glm::vec4 v = glm::rotate(glm::mat4(1.0f), glm::radians(i*90.0f), glm::vec3(0, 1, 0)) * glm::vec4(0.5f, 0.5f, 0, 1);
        cub_vert_pos[i + 8] = v;
    }
    for (int i = 4; i < 8; i++) {
        glm::vec4 v = glm::rotate(glm::mat4(1.0f), glm::radians((i-4)*90.0f), glm::vec3(0, 1, 0)) * glm::vec4(0.5f, -0.5f, 0, 1);
        cub_vert_pos[i + 8] = v;
    }

    for (int i = 0; i < 4; i++) {
        glm::vec4 v = glm::rotate(glm::mat4(1.0f), glm::radians(i*90.0f), glm::vec3(0, 1, 0)) * glm::vec4(0.5f, 0.5f, 0, 1);
        cub_vert_pos[i + 16] = v;
    }
    for (int i = 4; i < 8; i++) {
        glm::vec4 v = glm::rotate(glm::mat4(1.0f), glm::radians((i-4)*90.0f), glm::vec3(0, 1, 0)) * glm::vec4(0.5f, -0.5f, 0, 1);
        cub_vert_pos[i + 16] = v;
    }
}

void createCubeVertexNormals(glm::vec4 (&cub_vert_nor)[CUB_VERT_COUNT]) {
    // y direction normal
    for (int i = 0; i < 4; i++) {
        cub_vert_nor[i] = glm::vec4(0,1,0,0);
    }
    for (int i = 4; i < 8; i++) {
        cub_vert_nor[i] = glm::vec4(0,-1,0,0);
    }

    // x direction normal
    for (int i = 0; i < 4; i++) {
        glm::vec4 v = glm::rotate(glm::mat4(1.0f), glm::radians(i*90.0f), glm::vec3(0, 1, 0)) * glm::vec4(1,0,0,0);
        cub_vert_nor[i + 8] = v;
    }
    for (int i = 4; i < 8; i++) {
        glm::vec4 v = glm::rotate(glm::mat4(1.0f), glm::radians((i-4)*90.0f), glm::vec3(0, 1, 0)) * glm::vec4(1,0,0,0);
        cub_vert_nor[i + 8] = v;
    }

    // z direction normal
    // TO DO: need consideration
    for (int i = 0; i < 4; i++) {
        glm::vec4 v = glm::rotate(glm::mat4(1.0f), glm::radians(i*90.0f), glm::vec3(0, 1, 0)) * glm::vec4(0,0,1,0);
        cub_vert_nor[i + 16] = v;
    }
    for (int i = 4; i < 8; i++) {
        glm::vec4 v = glm::rotate(glm::mat4(1.0f), glm::radians((i-4)*90.0f), glm::vec3(0, 1, 0)) * glm::vec4(0,0,1,0);
        cub_vert_nor[i + 16] = v;
    }

}

void createCubeIndices(GLuint (&cub_idx)[CUB_IDX_COUNT]) {
    // y direction
    cub_idx[0] = 0; cub_idx[1] = 1; cub_idx[2] = 2;
    cub_idx[3] = 0; cub_idx[4] = 2; cub_idx[5] = 3;
    cub_idx[6] = 4; cub_idx[7] = 5; cub_idx[8] = 6;
    cub_idx[9] = 4; cub_idx[10] = 6; cub_idx[11] = 7;
    // x direction
    cub_idx[12] = 8; cub_idx[13] = 11; cub_idx[14] = 15;
    cub_idx[15] = 8; cub_idx[16] = 12; cub_idx[17] = 15;
    cub_idx[18] = 9; cub_idx[19] = 10; cub_idx[20] = 14;
    cub_idx[21] = 9; cub_idx[22] = 13; cub_idx[23] = 14;
    // z direction
    cub_idx[24] = 16; cub_idx[25] = 17; cub_idx[26] = 21;
    cub_idx[27] = 16; cub_idx[28] = 20; cub_idx[29] = 21;
    cub_idx[30] = 18; cub_idx[31] = 19; cub_idx[32] = 23;
    cub_idx[33] = 18; cub_idx[34] = 22; cub_idx[35] = 23;
}

void Cube::create() {
    GLuint cub_idx[CUB_IDX_COUNT];
    glm::vec4 cub_vert_pos[CUB_VERT_COUNT];
    glm::vec4 cub_vert_nor[CUB_VERT_COUNT];

    createCubeVertexPositions(cub_vert_pos);
    createCubeVertexNormals(cub_vert_nor);
    createCubeIndices(cub_idx);

    count = CUB_IDX_COUNT;

    // Create a VBO on our GPU and store its handle in bufIdx
    generateIdx();
    // Tell OpenGL that we want to perform subsequent operations on the VBO referred to by bufIdx
    // and that it will be treated as an element array buffer (since it will contain triangle indices)
    context->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufIdx);
    // Pass the data stored in cyl_idx into the bound buffer, reading a number of bytes equal to
    // CYL_IDX_COUNT multiplied by the size of a GLuint. This data is sent to the GPU to be read by shader programs.
    context->glBufferData(GL_ELEMENT_ARRAY_BUFFER, CUB_IDX_COUNT * sizeof(GLuint), cub_idx, GL_STATIC_DRAW);

    // The next few sets of function calls are basically the same as above, except bufPos and bufNor are
    // array buffers rather than element array buffers, as they store vertex attributes like position.
    generatePos();
    context->glBindBuffer(GL_ARRAY_BUFFER, bufPos);
    context->glBufferData(GL_ARRAY_BUFFER, CUB_VERT_COUNT * sizeof(glm::vec4), cub_vert_pos, GL_STATIC_DRAW);

    generateNor();
    context->glBindBuffer(GL_ARRAY_BUFFER, bufNor);
    context->glBufferData(GL_ARRAY_BUFFER, CUB_VERT_COUNT * sizeof(glm::vec4), cub_vert_nor, GL_STATIC_DRAW);

}


