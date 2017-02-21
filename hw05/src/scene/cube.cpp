#include "cube.h"
#include <la.h>
#include <iostream>

static const int CUB_IDX_COUNT = 36;
static const int CUB_VERT_COUNT = 24;

Cube::Cube(GLWidget277 *context) : Drawable(context)
{}



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


void Cube::create()
{
    HalfEdge h1, h2, h3, h4, h5, h6, h7, h8, h9, h10, h11, h12, h13, h14, h15, h16, h17, h18, h19, h20, h21, h22, h23, h24;

    Vertex v1(glm::vec3(0.5, 0.5, 0.5), &h1, 1);
    Vertex v2(glm::vec3(-0.5, 0.5, 0.5), &h2, 2);
    Vertex v3(glm::vec3(-0.5, -0.5, 0.5), &h3, 3);
    Vertex v4(glm::vec3(0.5, -0.5, 0.5), &h4, 4);
    Vertex v5(glm::vec3(0.5, 0.5, -0.5), &h5, 5);
    Vertex v6(glm::vec3(-0.5, 0.5, -0.5), &h6, 6);
    Vertex v7(glm::vec3(-0.5, -0.5, -0.5), &h7, 7);
    Vertex v8(glm::vec3(0.5, -0.5, -0.5), &h8, 8);

    Face f1(&h1, glm::vec3(0,0,0), 1); //upper
    Face f2(&h5, glm::vec3(0,0,0), 2); //lower
    Face f3(&h9, glm::vec3(0,0,0), 3); //left
    Face f4(&h13, glm::vec3(0,0,0), 4); //right
    Face f5(&h17, glm::vec3(0,0,0), 5); //front
    Face f6(&h21, glm::vec3(0,0,0), 6); //back

    h1 = HalfEdge(&f1, &v1, &h2, &h16, 1);
    h2 = HalfEdge(&f1, &v2, &h3, &h23, 2);
    h3 = HalfEdge(&f1, &v3, &h4, &h12, 3);
    h4 = HalfEdge(&f1, &v4, &h1, &h19, 4);

    h5 = HalfEdge(&f2, &v5, &h6, &h14, 5);
    h6 = HalfEdge(&f2, &v6, &h7, &h21, 6);
    h7 = HalfEdge(&f2, &v7, &h8, &h10, 7);
    h8 = HalfEdge(&f2, &v8, &h5, &h17, 8);

    h9 = HalfEdge(&f3, &v7, &h10, &h20, 9);
    h10 = HalfEdge(&f3, &v6, &h11, &h7, 10);
    h11 = HalfEdge(&f3, &v2, &h12, &h24, 11);
    h12 = HalfEdge(&f3, &v3, &h9, &h3, 12);

    h13 = HalfEdge(&f4, &v8, &h14, &h18, 13);
    h14 = HalfEdge(&f4, &v5, &h15, &h5, 14);
    h15 = HalfEdge(&f4, &v1, &h16, &h22, 15);
    h16 = HalfEdge(&f4, &v4, &h13, &h1, 16);

    h17 = HalfEdge(&f5, &v8, &h18, &h8, 17);
    h18 = HalfEdge(&f5, &v4, &h19, &h13, 18);
    h19 = HalfEdge(&f5, &v3, &h20, &h4, 19);
    h20 = HalfEdge(&f5, &v7, &h17, &h9, 20);

    h21 = HalfEdge(&f6, &v5, &h22, &h6, 21);
    h22 = HalfEdge(&f6, &v1, &h23, &h15, 22);
    h23 = HalfEdge(&f6, &v2, &h24, &h2, 23);
    h24 = HalfEdge(&f6, &v6, &h21, &h11, 24);

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
