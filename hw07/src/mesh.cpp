#include "mesh.h"
#include "mainwindow.h"

Mesh::Mesh(GLWidget277 *context) : Drawable(context)
{
}

int Mesh::getIDX(){
    int num = 0;

    for(int i = 0; i < Faces.size(); i++){
       HalfEdge* startEdge = Faces[i]->start_edge;
       HalfEdge* next = startEdge->next;
       while(next != startEdge){
           next = next->next;
           num ++;
       }
    }
    return 3*(num-Faces.size());
}

void getDistances(Vertex* v, Joint* j, Joint* (&joints)[2], float (&distances)[2]) {

    j->setBindMatrix();

    // compute the distance between vertex and joint
    float distance = glm::distance(glm::vec4(v->pos,1), j->GetOverallTransformation() * glm::vec4(0,0,0,1));
    float max = std::max(distances[0],distances[1]);
    if (distance < max) {
        if (distances[0] == max) {
            distances[0] = distance;
            joints[0] = j;
        } else {
            distances[1] = distance;
            joints[1] = j;
        }
    }
    if (j->children.size() > 0) {
        for (int i = 0; i < j->children.size();i++) {
            getDistances(v,j->children[i],joints,distances);
        }
    }
}


void Mesh::bindVertex(Joint* root){
    std::vector<glm::vec2> influences;
    std::vector<glm::ivec2> ids;

    for (int i = 0; i < Vertices.size();i++) {
        Joint* joints[2];
        float distances[2]{FLT_MAX,FLT_MAX};

        getDistances(Vertices[i], root, joints, distances);

        float total    = distances[0]+distances[1];
        distances[0]  /= total;
        distances[1]  /= total;

        Vertices[i]->bindPosition = Vertices[i]->pos;
        Vertices[i]->influences[0] = std::pair <Joint*,float> (joints[0],distances[0]);
        Vertices[i]->influences[1] = std::pair <Joint*,float> (joints[1],distances[1]);
    }

    for(Face* f : this->Faces)
    {
        HalfEdge* startEdge = f->start_edge;

        influences.push_back(glm::vec2(startEdge->vert->influences[0].second, startEdge->vert->influences[1].second));
        ids.push_back(glm::ivec2(startEdge->vert->influences[0].first->id, startEdge->vert->influences[1].first->id));

        startEdge = startEdge->next;

        while (startEdge != f->start_edge)
        {
            influences.push_back(glm::vec2(startEdge->vert->influences[0].second, startEdge->vert->influences[1].second));
            ids.push_back(glm::ivec2(startEdge->vert->influences[0].first->id, startEdge->vert->influences[1].first->id));
            startEdge = startEdge->next;
        }
    }

    generateJointIDs();
    context->glBindBuffer(GL_ARRAY_BUFFER, bufJointIDs);
    context->glBufferData(GL_ARRAY_BUFFER, ids.size() * sizeof(glm::ivec2), &ids[0], GL_STATIC_DRAW);

    generateInfluences();
    context->glBindBuffer(GL_ARRAY_BUFFER, bufInfluences);
    context->glBufferData(GL_ARRAY_BUFFER, influences.size() * sizeof(glm::vec2), &influences[0], GL_STATIC_DRAW);
}


void Mesh::updateVertex(Joint* root, ShaderProgram &prog_skeleton)
{
    glm::mat4 transformationMatrix[100];
    root->getTransformationMatrix(transformationMatrix);

    prog_skeleton.setTransformationMatrix(transformationMatrix);
}

void Mesh::create()
{
    // for cube
    // input value vertex_size = 8, face_size = 6, halfedge_size = 24
    // we need to populate vertex_size to 24 in vbo part

    int vertex_size = Vertices.size();
    int face_size = Faces.size();
    int halfedge_size = HalfEdges.size();

    GLuint mesh_idx[getIDX()];

    mesh_vert_pos.resize(halfedge_size);
    mesh_vert_nor.resize(halfedge_size);
    mesh_vert_col.resize(halfedge_size);

    int n=0;
    for(Face* face:Faces){
       HalfEdge* startEdge = face->start_edge;
       mesh_vert_pos[n] = glm::vec4(startEdge->vert->pos,1);
       mesh_vert_col[n] = glm::vec4(face->color,1);

       glm::vec3 v1 = startEdge->vert->pos;
       glm::vec3 v2 = startEdge->next->vert->pos;
       glm::vec3 v3 = startEdge->next->next->vert->pos;

       glm::vec3 nor = glm::normalize(glm::cross(v2-v1, v3-v2));
       n++;

       HalfEdge* next = startEdge->next;
       while(next != startEdge){
           mesh_vert_pos[n] = glm::vec4(next->vert->pos,1);
           mesh_vert_col[n] = glm::vec4(face->color, 1);
           mesh_vert_nor[n] = glm::vec4(nor, 1);
           next=next->next;
           n++;
       }
    }

    //creat indices
    int num = 0;
    int vertcount = 0;
    for(Face* face:Faces){
        int edgenum = 1;
        HalfEdge* startEdge = face->start_edge->next;
        while (startEdge != face->start_edge)
        {
            edgenum += 1;
            startEdge = startEdge->next;
        }
        // edgenum-> # of edges in a face

        for (int j = 2; j < edgenum; j++)
        {
            mesh_idx[num*3] = vertcount;
            mesh_idx[num*3+1] = vertcount+j-1;
            mesh_idx[num*3+2] = vertcount+j;
            num++;
        }
        vertcount = vertcount + edgenum;
    }

    count = getIDX();
    int test = getIDX();


    // Create a VBO on our GPU and store its handle in bufIdx
    generateIdx();
    // Tell OpenGL that we want to perform subsequent operations on the VBO referred to by bufIdx
    // and that it will be treated as an element array buffer (since it will contain triangle indices)
    context->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufIdx);
    // Pass the data stored in cyl_idx into the bound buffer, reading a number of bytes equal to
    // SPH_IDX_COUNT multiplied by the size of a GLuint. This data is sent to the GPU to be read by shader programs.
    context->glBufferData(GL_ELEMENT_ARRAY_BUFFER, getIDX() * sizeof(GLuint), mesh_idx, GL_STATIC_DRAW);

    // The next few sets of function calls are basically the same as above, except bufPos and bufNor are
    // array buffers rather than element array buffers, as they store vertex attributes like position.
    generatePos();
    context->glBindBuffer(GL_ARRAY_BUFFER, bufPos);
    context->glBufferData(GL_ARRAY_BUFFER, halfedge_size * sizeof(glm::vec4), &mesh_vert_pos[0], GL_STATIC_DRAW);

    generateNor();
    context->glBindBuffer(GL_ARRAY_BUFFER, bufNor);
    context->glBufferData(GL_ARRAY_BUFFER, halfedge_size * sizeof(glm::vec4), &mesh_vert_nor[0], GL_STATIC_DRAW);

    generateCol();
    context->glBindBuffer(GL_ARRAY_BUFFER, bufCol);
    context->glBufferData(GL_ARRAY_BUFFER, halfedge_size * sizeof(glm::vec4), &mesh_vert_col[0], GL_STATIC_DRAW);
}


void Mesh::LoadCube()
{
    Vertex *V0 = new Vertex(0), *V1 = new Vertex(1), *V2 = new Vertex(2), *V3 = new Vertex(3),
            *V4 = new Vertex(4), *V5 = new Vertex(5),*V6 = new Vertex(6), *V7 = new Vertex(7);

    int i = 0;
    glm::vec4 v = glm::rotate(glm::mat4(1.0f), glm::radians(i*90.0f), glm::vec3(0, 1, 0)) * glm::vec4(0.5f,0.5f,0.5f,1);
    i = 1;
    v = glm::rotate(glm::mat4(1.0f), glm::radians(i*90.0f), glm::vec3(0, 1, 0)) * glm::vec4(0.5f,0.5f,0.5f,1);
    i = 2;
    v = glm::rotate(glm::mat4(1.0f), glm::radians(i*90.0f), glm::vec3(0, 1, 0)) * glm::vec4(0.5f,0.5f,0.5f,1);
    i = 3;
    v = glm::rotate(glm::mat4(1.0f), glm::radians(i*90.0f), glm::vec3(0, 1, 0)) * glm::vec4(0.5f,0.5f,0.5f,1);

    //set pos
    V0->pos = glm::vec3( 0.5, 0.5, 0.5);
    V1->pos = glm::vec3( 0.5, 0.5, -0.5);
    V2->pos = glm::vec3(-0.5, 0.5, -0.5);
    V3->pos = glm::vec3(-0.5, 0.5, 0.5);
    V4->pos = glm::vec3( 0.5, -0.5, 0.5);
    V5->pos = glm::vec3( 0.5, -0.5,-0.5);
    V6->pos = glm::vec3(-0.5, -0.5, -0.5);
    V7->pos = glm::vec3(-0.5, -0.5, 0.5);

    //top face:
    Face *F0 = new Face(0);
    F0->color = glm::vec3(0,0,1);
    HalfEdge *HE0 = new HalfEdge(0), *HE1 = new HalfEdge(1), *HE2 = new HalfEdge(2), *HE3 = new HalfEdge(3);
    F0->start_edge = HE0;
    //set next
    HE0->next = HE1;
    HE1->next = HE2;
    HE2->next = HE3;
    HE3->next = HE0;
    //set face
    HE0->face = F0;
    HE1->face = F0;
    HE2->face = F0;
    HE3->face = F0;
    //set Vertex
    HE0->vert = V0;
    HE1->vert = V1;
    HE2->vert = V2;
    HE3->vert = V3;    
    HE0->vert_before = V3;
    HE1->vert_before = V0;
    HE2->vert_before = V1;
    HE3->vert_before = V2;
    //set normal
//    V0->nor = glm::vec3(0,1,0);
//    V1->nor = glm::vec3(0,1,0);
//    V2->nor = glm::vec3(0,1,0);
//    V3->nor = glm::vec3(0,1,0);

    mesh_vert_nor.push_back(glm::vec4(0,1,0,1));
    mesh_vert_nor.push_back(glm::vec4(0,1,0,1));
    mesh_vert_nor.push_back(glm::vec4(0,1,0,1));
    mesh_vert_nor.push_back(glm::vec4(0,1,0,1));


    //buttom face:
    Face *F1 = new Face(1);
    F1->color = glm::vec3(0,1,0);
    HalfEdge *HE4 = new HalfEdge(4), *HE5 = new HalfEdge(5), *HE6 = new HalfEdge(6), *HE7 = new HalfEdge(7);
    F1->start_edge = HE4;
    //set next
    HE4->next = HE7;
    HE5->next = HE4;
    HE6->next = HE5;
    HE7->next = HE6;
    //set face
    HE4->face = F1;
    HE5->face = F1;
    HE6->face = F1;
    HE7->face = F1;
    //set Vertex
    HE4->vert = V4;
    HE5->vert = V5;
    HE6->vert = V6;
    HE7->vert = V7;

    HE4->vert_before = V5;
    HE5->vert_before = V6;
    HE6->vert_before = V7;
    HE7->vert_before = V4;

//    HE4->vert_before = V7;
//    HE5->vert_before = V4;
//    HE6->vert_before = V5;
//    HE7->vert_before = V6;

    //set normal
//    V3->nor = glm::vec3(0,-1,0);
//    V5->nor = glm::vec3(0,-1,0);
//    V4->nor = glm::vec3(0,-1,0);
//    V0->nor = glm::vec3(0,-1,0);

    mesh_vert_nor.push_back(glm::vec4(0,-1,0,1));
    mesh_vert_nor.push_back(glm::vec4(0,-1,0,1));
    mesh_vert_nor.push_back(glm::vec4(0,-1,0,1));
    mesh_vert_nor.push_back(glm::vec4(0,-1,0,1));

    //left face:
    Face *F2 = new Face(2);
    HalfEdge *HE8 = new HalfEdge(8), *HE9 = new HalfEdge(9), *HE10 = new HalfEdge(10), *HE11 = new HalfEdge(11);
    F2->color = glm::vec3(1,0,0);
    F2->start_edge = HE8;
    //set next
    HE8->next = HE11;
    HE9->next = HE8;
    HE10->next = HE9;
    HE11->next = HE10;
    //set face
    HE8->face = F2;
    HE9->face = F2;
    HE10->face = F2;
    HE11->face = F2;
    //set Vertex
    HE8->vert = V3;
    HE9->vert = V7;
    HE10->vert = V6;
    HE11->vert = V2;

    HE8->vert_before = V7;
    HE9->vert_before = V6;
    HE10->vert_before = V2;
    HE11->vert_before = V3;

    //set normal
//    V2->nor = glm::vec3(-1,0,0);
//    V6->nor = glm::vec3(-1,0,0);
//    V5->nor = glm::vec3(-1,0,0);
//    V3->nor = glm::vec3(-1,0,0);

    mesh_vert_nor.push_back(glm::vec4(-1,0,0,1));
    mesh_vert_nor.push_back(glm::vec4(-1,0,0,1));
    mesh_vert_nor.push_back(glm::vec4(-1,0,0,1));
    mesh_vert_nor.push_back(glm::vec4(-1,0,0,1));

    //right face:
    Face *F3 = new Face(3);
    HalfEdge *HE12 = new HalfEdge(12), *HE13 = new HalfEdge(13), *HE14 = new HalfEdge(14), *HE15 = new HalfEdge(15);
    F3->color = glm::vec3(0,1,1);
    F3->start_edge = HE12;
    //set next
    HE12->next = HE13;
    HE13->next = HE14;
    HE14->next = HE15;
    HE15->next = HE12;
    //set face
    HE12->face = F3;
    HE13->face = F3;
    HE14->face = F3;
    HE15->face = F3;
    //set Vertex
    HE12->vert = V0;
    HE13->vert = V4;
    HE14->vert = V5;
    HE15->vert = V1;
    HE12->vert_before = V1;
    HE13->vert_before = V0;
    HE14->vert_before = V4;
    HE15->vert_before = V5;
    //set normal
//    V5->nor = glm::vec3(1,0,0);
//    V6->nor = glm::vec3(1,0,0);
//    V7->nor = glm::vec3(1,0,0);
//    V4->nor = glm::vec3(1,0,0);

    mesh_vert_nor.push_back(glm::vec4(1,0,0,1));
    mesh_vert_nor.push_back(glm::vec4(1,0,0,1));
    mesh_vert_nor.push_back(glm::vec4(1,0,0,1));
    mesh_vert_nor.push_back(glm::vec4(1,0,0,1));

    //front face:
    Face *F4 = new Face(4);
    HalfEdge *HE16 = new HalfEdge(16), *HE17 = new HalfEdge(17), *HE18 = new HalfEdge(18), *HE19 = new HalfEdge(19);
    F4->color = glm::vec3(1,0,1);
    F4->start_edge = HE16;
    //set next
    HE16->next = HE17;
    HE17->next = HE18;
    HE18->next = HE19;
    HE19->next = HE16;
    //set face
    HE16->face = F4;
    HE17->face = F4;
    HE18->face = F4;
    HE19->face = F4;
    //set Vertex
    HE16->vert = V0;
    HE17->vert = V3;
    HE18->vert = V7;
    HE19->vert = V4;
    HE16->vert_before = V4;
    HE17->vert_before = V0;
    HE18->vert_before = V3;
    HE19->vert_before = V7;
    //set normal
//    V1->nor = glm::vec3(0,0,1);
//    V7->nor = glm::vec3(0,0,1);
//    V6->nor = glm::vec3(0,0,1);
//    V2->nor = glm::vec3(0,0,1);

    mesh_vert_nor.push_back(glm::vec4(0,0,1,1));
    mesh_vert_nor.push_back(glm::vec4(0,0,1,1));
    mesh_vert_nor.push_back(glm::vec4(0,0,1,1));
    mesh_vert_nor.push_back(glm::vec4(0,0,1,1));

    //back face:
    Face *F5 = new Face(5);
    HalfEdge *HE20 = new HalfEdge(20), *HE21 = new HalfEdge(21), *HE22 = new HalfEdge(22), *HE23 = new HalfEdge(23);
    F5->color = glm::vec3(1,1,0);
    F5->start_edge = HE20;
    //set next
    HE20->next = HE23;
    HE21->next = HE20;
    HE22->next = HE21;
    HE23->next = HE22;
    //set face
    HE20->face = F5;
    HE21->face = F5;
    HE22->face = F5;
    HE23->face = F5;
    //set Vertex
    HE20->vert = V1;
    HE21->vert = V2;
    HE22->vert = V6;
    HE23->vert = V5;
    HE20->vert_before = V2;
    HE21->vert_before = V6;
    HE22->vert_before = V5;
    HE23->vert_before = V1;
    //set normal
//    V0->nor = glm::vec3(0,0,-1);
//    V4->nor = glm::vec3(0,0,-1);
//    V7->nor = glm::vec3(0,0,-1);
//    V1->nor = glm::vec3(0,0,-1);

    mesh_vert_nor.push_back(glm::vec4(0,0,-1,1));
    mesh_vert_nor.push_back(glm::vec4(0,0,-1,1));
    mesh_vert_nor.push_back(glm::vec4(0,0,-1,1));
    mesh_vert_nor.push_back(glm::vec4(0,0,-1,1));

    //set SYM:
    HE0->sym = HE17;
    HE1->sym = HE12;
    HE2->sym = HE20;
    HE3->sym = HE11;
    HE4->sym = HE14;
    HE5->sym = HE22;
    HE6->sym = HE9;
    HE7->sym = HE19;
    HE8->sym = HE18;
    HE9->sym = HE6;
    HE10->sym = HE21;
    HE11->sym = HE3;
    HE12->sym = HE1;
    HE13->sym = HE16;
    HE14->sym = HE4;
    HE15->sym = HE23;
    HE16->sym = HE13;
    HE17->sym = HE0;
    HE18->sym = HE8;
    HE19->sym = HE7;
    HE20->sym = HE2;
    HE21->sym = HE10;
    HE22->sym = HE5;
    HE23->sym = HE15;

    V0->edge = HE0;
    V1->edge = HE1;
    V2->edge = HE2;
    V3->edge = HE3;
    V4->edge = HE4;
    V5->edge = HE5;
    V6->edge = HE6;
    V7->edge = HE7;

    //push_back:
    //Faces
    Faces.push_back(F0);
    Faces.push_back(F1);
    Faces.push_back(F2);
    Faces.push_back(F3);
    Faces.push_back(F4);
    Faces.push_back(F5);
    //Vertices
    Vertices.push_back(V0);
    Vertices.push_back(V1);
    Vertices.push_back(V2);
    Vertices.push_back(V3);
    Vertices.push_back(V4);
    Vertices.push_back(V5);
    Vertices.push_back(V6);
    Vertices.push_back(V7);
    //HalfEdges
    HalfEdges.push_back(HE0);
    HalfEdges.push_back(HE1);
    HalfEdges.push_back(HE2);
    HalfEdges.push_back(HE3);
    HalfEdges.push_back(HE4);
    HalfEdges.push_back(HE5);
    HalfEdges.push_back(HE6);
    HalfEdges.push_back(HE7);
    HalfEdges.push_back(HE8);
    HalfEdges.push_back(HE9);
    HalfEdges.push_back(HE10);
    HalfEdges.push_back(HE11);
    HalfEdges.push_back(HE12);
    HalfEdges.push_back(HE13);
    HalfEdges.push_back(HE14);
    HalfEdges.push_back(HE15);
    HalfEdges.push_back(HE16);
    HalfEdges.push_back(HE17);
    HalfEdges.push_back(HE18);
    HalfEdges.push_back(HE19);
    HalfEdges.push_back(HE20);
    HalfEdges.push_back(HE21);
    HalfEdges.push_back(HE22);
    HalfEdges.push_back(HE23);
}
