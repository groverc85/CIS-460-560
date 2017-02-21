#include "plusmark.h"
#include <la.h>
#include <iostream>
///------------------------------------------
/// -----------added by Xi Yang--------------
/// ---Draw plus mark in the screen center---
/// -----------------------------------------
///
PlusMark::PlusMark(GLWidget277 *context):Drawable(context),pluslength(0.3)
{}


GLenum PlusMark::drawMode()
{
    return GL_LINES;
}
void PlusMark::SetPlusLength(float l)
{
    pluslength=l;
//    destroy();
//    create();
}
void PlusMark::create()
{
    GLuint pls_idx[4];
    glm::vec4 pls_vert_pos[4];
    glm::vec4 pls_vert_col[4];
    glm::vec4 pls_vert_data[12];
    for(unsigned int i=0;i<4;i++)
    {
        pls_idx[i]=i;
        pls_vert_col[i]=glm::vec4(1.0f);
    }
//    pls_vert_pos[0]=pos+glm::vec4(pluslength*0.5,0,0,1);
//    pls_vert_pos[1]=pos-glm::vec4(pluslength*0.5,0,0,1);
//    pls_vert_pos[2]=pos+glm::vec4(0,pluslength*0.5,0,1);
//    pls_vert_pos[3]=pos-glm::vec4(0,pluslength*0.5,0,1);
    pls_vert_pos[0]=glm::vec4(pluslength*0.5,0,0,1);
    pls_vert_pos[1]=glm::vec4(-pluslength*0.5,0,0,1);
    pls_vert_pos[2]=glm::vec4(0,pluslength*0.5,0,1);
    pls_vert_pos[3]=glm::vec4(0,-pluslength*0.5,0,1);
    pls_vert_data[0]=glm::vec4(pluslength*0.5,0,0,1);
    pls_vert_data[1]=glm::vec4(0,0,0,0);
    pls_vert_data[2]=glm::vec4(1.0f);
    pls_vert_data[3]=glm::vec4(-pluslength*0.5,0,0,1);
    pls_vert_data[4]=glm::vec4(0,0,0,0);
    pls_vert_data[5]=glm::vec4(1.0f);
    pls_vert_data[6]=glm::vec4(0,pluslength*0.5,0,1);
    pls_vert_data[7]=glm::vec4(0,0,0,0);
    pls_vert_data[8]=glm::vec4(1.0f);
    pls_vert_data[9]=glm::vec4(0,-pluslength*0.5,0,1);
    pls_vert_data[10]=glm::vec4(0,0,0,0);
    pls_vert_data[11]=glm::vec4(1.0f);
    count=4;

    generateIdx();
    context->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufIdx);
    context->glBufferData(GL_ELEMENT_ARRAY_BUFFER, 4 * sizeof(GLuint), pls_idx, GL_STATIC_DRAW);

   // generatePos();
   // context->glBindBuffer(GL_ARRAY_BUFFER, bufPos);
   // context->glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(glm::vec4), pls_vert_pos, GL_STATIC_DRAW);

   // generateCol();
   // context->glBindBuffer(GL_ARRAY_BUFFER, bufCol);
    //context->glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(glm::vec4), pls_vert_col, GL_STATIC_DRAW);


    generateVData();
    context->glBindBuffer(GL_ARRAY_BUFFER,bufVData);
    context->glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(glm::vec4), pls_vert_data, GL_STATIC_DRAW);


}
