#include "mygl.h"
#include <la.h>
#include <iostream>
#include <QApplication>
#include <QKeyEvent>
#include <QListWidgetItem>
#include <QListWidget>
#include <random>

MyGL::MyGL(QWidget *parent)
    : GLWidget277(parent),
      geom_cylinder(this), geom_sphere(this), geom_mesh(this),
      prog_lambert(this), prog_flat(this),
      gl_camera(), vertexSelection(nullptr), edgeSelection(nullptr), faceSelection(nullptr), face_draw(this), \
      point(this), line(this)
{
    // Connect the timer to a function so that when the timer ticks the function is executed
    connect(&timer, SIGNAL(timeout()), this, SLOT(timerUpdate()));
    // Tell the timer to redraw 60 times per second
    timer.start(16);
    timeCount = 0;
    renderMode = 0;
}

MyGL::~MyGL()
{
    makeCurrent();
    glDeleteVertexArrays(1, &vao);
    geom_cylinder.destroy();
    geom_sphere.destroy();
    geom_mesh.destroy();
    delete edgeSelection;
    delete vertexSelection;
    delete faceSelection;
}

void MyGL::initializeGL()
{
    // Create an OpenGL context using Qt's QOpenGLFunctions_3_2_Core class
    // If you were programming in a non-Qt context you might use GLEW (GL Extension Wrangler)instead
    initializeOpenGLFunctions();
    // Print out some information about the current OpenGL context
    debugContextVersion();

    // Set a few settings/modes in OpenGL rendering
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_POLYGON_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
    // Set the size with which points should be rendered
    glPointSize(5);
    // Set the color with which the screen is filled at the start of each render call.
    glClearColor(0.5, 0.5, 0.5, 1);

    printGLErrorLog();

    // Create a Vertex Attribute Object
    glGenVertexArrays(1, &vao);

    //Create the instances of Cylinder and Sphere.
    geom_cylinder.create();
    geom_sphere.create();

    point.create();
    line.create();
    face_draw.create();

    //Create the cube in Mesh
    if (cube_display == 1)
        geom_mesh.LoadCube();
    geom_mesh.create();

    // Create and set up the diffuse shader
    prog_lambert.create(":/glsl/lambert.vert.glsl", ":/glsl/lambert.frag.glsl");
    // Create and set up the flat lighting shader
    prog_flat.create(":/glsl/flat.vert.glsl", ":/glsl/flat.frag.glsl");

    // Set a color with which to draw geometry since you won't have one
    // defined until you implement the Node classes.
    // This makes your geometry render green.
//    prog_lambert.setGeometryColor(glm::vec4(0,1,0,1));

    // We have to have a VAO bound in OpenGL 3.2 Core. But if we're not
    // using multiple VAOs, we can just bind one once.
//    vao.bind();
    glBindVertexArray(vao);


}

void MyGL::resizeGL(int w, int h)
{
    //This code sets the concatenated view and perspective projection matrices used for
    //our scene's camera view.
    gl_camera = Camera(w, h);
    glm::mat4 viewproj = gl_camera.getViewProj();

    // Upload the view-projection matrix to our shaders (i.e. onto the graphics card)

    prog_lambert.setViewProjMatrix(viewproj);
    prog_flat.setViewProjMatrix(viewproj);

    printGLErrorLog();
}

//This function is called by Qt any time your GL window is supposed to update
//For example, when the function updateGL is called, paintGL is called implicitly.
//DO NOT CONSTRUCT YOUR SCENE GRAPH IN THIS FUNCTION!
void MyGL::paintGL()
{
    // Clear the screen so that we only see newly drawn images
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    prog_flat.setViewProjMatrix(gl_camera.getViewProj());
    prog_lambert.setViewProjMatrix(gl_camera.getViewProj());

#define NOPE
#ifdef NOPE

    for (int i = 0; i < geom_mesh.Vertices.size(); i++)
    {
        emit sig_sendVertex(geom_mesh.Vertices[i]);
    }

    for (int i = 0; i < geom_mesh.Faces.size(); i++)
    {
        emit sig_sendFace(geom_mesh.Faces[i]);
    }

    for (int i = 0; i < geom_mesh.HalfEdges.size(); i++)
    {
        emit sig_sendHalfEdge(geom_mesh.HalfEdges[i]);
    }


    //Create a model matrix. This one scales the sphere uniformly by 3, then translates it by <-2,0,0>.
    //Note that we have to transpose the model matrix before passing it to the shader
    //This is because OpenGL expects column-major matrices, but you've
    //implemented row-major matrices.

//    glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(-2,0,0)) * glm::scale(glm::mat4(1.0f), glm::vec3(3,3,3));
    //Send the geometry's transformation matrix to the shader
//    prog_lambert.setModelMatrix(model);
    //Draw the example sphere using our lambert shader
//    prog_lambert.draw(geom_sphere);

    //Now do the same to render the cylinder
    //We've rotated it -45 degrees on the Z axis, then translated it to the point <2,2,0>
    glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(1.0f,1.0f,1.0f)) * glm::rotate(glm::mat4(1.0f), glm::radians(-45.0f), glm::vec3(0,0,1));

    prog_lambert.setModelMatrix(model);

    prog_lambert.context->glUniform1i(prog_lambert.unifTime, timeCount);
    prog_lambert.context->glUniform1i(prog_lambert.unifRenderMode, renderMode);

//    prog_lambert.draw(geom_cylinder);

    //    Draw mesh
//    model = glm::translate(glm::mat4(1.0f), glm::vec3(0,1,0)) * glm::rotate(glm::mat4(1.0f), glm::radians(-45.0f), glm::vec3(0,0,1));
//    prog_lambert.setModelMatrix(model);
    prog_lambert.draw(geom_mesh);

    glDisable(GL_DEPTH_TEST);

    prog_flat.setModelMatrix(model);

    prog_flat.draw(point);
    prog_flat.draw(line);
    prog_flat.draw(face_draw);

    glEnable(GL_DEPTH_TEST);


#endif
}


void MyGL::keyPressEvent(QKeyEvent *e)
{

    float amount = 2.0f;
    if(e->modifiers() & Qt::ShiftModifier){
        amount = 10.0f;
    }
    // http://doc.qt.io/qt-5/qt.html#Key-enum
    // This could all be much more efficient if a switch
    // statement were used, but I really dislike their
    // syntax so I chose to be lazy and use a long
    // chain of if statements instead
    if (e->key() == Qt::Key_Escape) {
        QApplication::quit();
    } else if (e->key() == Qt::Key_Right) {
        gl_camera.RotateAboutUp(-amount);
    } else if (e->key() == Qt::Key_Left) {
        gl_camera.RotateAboutUp(amount);
    } else if (e->key() == Qt::Key_Up) {
        gl_camera.RotateAboutRight(-amount);
    } else if (e->key() == Qt::Key_Down) {
        gl_camera.RotateAboutRight(amount);
    } else if (e->key() == Qt::Key_1) {
        gl_camera.fovy += amount;
    } else if (e->key() == Qt::Key_2) {
        gl_camera.fovy -= amount;
    } else if (e->key() == Qt::Key_W) {
        gl_camera.TranslateAlongLook(amount);
    } else if (e->key() == Qt::Key_S) {
        gl_camera.TranslateAlongLook(-amount);
    } else if (e->key() == Qt::Key_D) {
        gl_camera.TranslateAlongRight(amount);
    } else if (e->key() == Qt::Key_A) {
        gl_camera.TranslateAlongRight(-amount);
    } else if (e->key() == Qt::Key_Q) {
        gl_camera.TranslateAlongUp(-amount);
    } else if (e->key() == Qt::Key_E) {
        gl_camera.TranslateAlongUp(amount);
    } else if (e->key() == Qt::Key_P) {
        cube_display = 1 - cube_display;
    }
    else if (e->key() == Qt::Key_R) {
//        gl_camera = Camera(this->width(), this->height());
        renderMode = 1 - renderMode;
    } else if(e->key() == Qt::Key_N) {
        line.lne_vert_pos.clear();
        line.lne_vert_pos.push_back(glm::vec4(edgeSelection->vert->pos, 1));
        line.lne_vert_pos.push_back(glm::vec4(edgeSelection->next->vert->pos, 1));

        edgeSelection = edgeSelection->next;

        line.destroy();
        line.create();
    } else if(e->key() == Qt::Key_M) {
        line.lne_vert_pos.clear();
        line.lne_vert_pos.push_back(glm::vec4(edgeSelection->vert->pos, 1));
        line.lne_vert_pos.push_back(glm::vec4(edgeSelection->sym->vert->pos, 1));

        edgeSelection = edgeSelection->sym;
    } else if(e->key() == Qt::Key_F) {
        faceSelection = edgeSelection->face;
        face_draw.fce_vert_col.clear();
        face_draw.fce_vert_pos.clear();

        //start point
        face_draw.fce_vert_pos.push_back(glm::vec4(faceSelection->start_edge->vert->pos,1));
        face_draw.fce_vert_col.push_back(glm::vec4(glm::vec3(1,1,1) - faceSelection->color, 1));

        HalfEdge* lastEdge = faceSelection->start_edge->next;

        while (lastEdge != faceSelection->start_edge)
        {
            face_draw.fce_vert_pos.push_back(glm::vec4(lastEdge->vert->pos,1));
            face_draw.fce_vert_pos.push_back(glm::vec4(lastEdge->vert->pos,1));
            face_draw.fce_vert_col.push_back(glm::vec4(glm::vec3(1,1,1) - faceSelection->color, 1));
            face_draw.fce_vert_col.push_back(glm::vec4(glm::vec3(1,1,1) - faceSelection->color, 1));

            lastEdge = lastEdge->next;
        }
        face_draw.fce_vert_pos.push_back(glm::vec4(faceSelection->start_edge->vert->pos,1));
        face_draw.fce_vert_col.push_back(glm::vec4(glm::vec3(1,1,1) - faceSelection->color, 1));

        face_draw.destroy();
        face_draw.create();
    } else if(e->key() == Qt::Key_V) {
        point.pnt_vert_pos.clear();
        point.pnt_vert_pos.push_back(glm::vec4(edgeSelection->vert->pos,1));

        point.destroy();
        point.create();
    } else if ((e->key() == Qt::Key_H) && QApplication::keyboardModifiers() && Qt::ShiftModifier) {
        line.lne_vert_pos.clear();

        line.lne_vert_pos.push_back(glm::vec4(faceSelection->start_edge->vert->pos,1));
        line.lne_vert_pos.push_back(glm::vec4(faceSelection->start_edge->sym->vert->pos, 1));

        line.destroy();
        line.create();
    } else if(e->key() == Qt::Key_H) {
        line.lne_vert_pos.clear();

        line.lne_vert_pos.push_back(glm::vec4(vertexSelection->edge->vert->pos,1));
        line.lne_vert_pos.push_back(glm::vec4(vertexSelection->edge->sym->vert->pos, 1));

        line.destroy();
        line.create();
    }
    gl_camera.RecomputeAttributes();
    update();  // Calls paintGL, among other things
}

void MyGL::timerUpdate()
{
//     This function is called roughly 60 times per second.
//     Use it to perform any repeated actions you want to do,
//     such as
    timeCount += 1;
    update();
    setFocus();
}

void MyGL::slot_vertexSelectChanged(QListWidgetItem* v)
{
    Vertex* vertex = dynamic_cast<Vertex *> (v);

    point.pnt_vert_pos.clear();

    if (vertex)
//    if (Vertex* vertex = dynamic_cast<Vertex *> (v))
    {
//        point.destroy();

        this->vertexSelection = vertex;
//        printf("%f, %f, %f\n", vertex->pos[0], vertex->pos[1], vertex->pos[2]);
        emit sig_updateVertexX(vertex->pos[0]);
        emit sig_updateVertexY(vertex->pos[1]);
        emit sig_updateVertexZ(vertex->pos[2]);

        point.pnt_vert_pos.push_back(glm::vec4(vertex->pos,1));

        point.destroy();
        point.create();
        this->update();

    }
}

void MyGL::slot_faceSelectChanged(QListWidgetItem *f)
{
    face_draw.fce_vert_col.clear();
    face_draw.fce_vert_pos.clear();

    if (Face* face = dynamic_cast<Face *>(f)) {
        this->faceSelection = face;

        emit sig_updateFaceR(face->color[0]);
        emit sig_updateFaceG(face->color[1]);
        emit sig_updateFaceB(face->color[2]);

        //start point
        face_draw.fce_vert_pos.push_back(glm::vec4(face->start_edge->vert->pos,1));
        face_draw.fce_vert_col.push_back(glm::vec4(glm::vec3(1,1,1) - face->color, 1));

        HalfEdge* lastEdge = face->start_edge->next;

        while (lastEdge != face->start_edge)
        {
            face_draw.fce_vert_pos.push_back(glm::vec4(lastEdge->vert->pos,1));
            face_draw.fce_vert_pos.push_back(glm::vec4(lastEdge->vert->pos,1));
            face_draw.fce_vert_col.push_back(glm::vec4(glm::vec3(1,1,1) - face->color, 1));
            face_draw.fce_vert_col.push_back(glm::vec4(glm::vec3(1,1,1) - face->color, 1));

            lastEdge = lastEdge->next;
        }
        face_draw.fce_vert_pos.push_back(glm::vec4(face->start_edge->vert->pos,1));
        face_draw.fce_vert_col.push_back(glm::vec4(glm::vec3(1,1,1) - face->color, 1));

        face_draw.destroy();
        face_draw.create();
        this->update();

    }
}

void MyGL::slot_halfEdgeSelectChanged(QListWidgetItem *l)
{
    if (HalfEdge* edge = dynamic_cast<HalfEdge *>(l)) {
        line.lne_vert_pos.clear();
        this->edgeSelection = edge;

        line.lne_vert_pos.push_back(glm::vec4(edge->vert->pos,1));

        HalfEdge* last_edge = edge;

        while (edge->next != last_edge)
        {
            edge = edge->next;
        }

        line.lne_vert_pos.push_back(glm::vec4(edge->vert->pos, 1));

        line.destroy();
        line.create();
        this->update();
    }
}

void MyGL::slot_receiveVertexTranslateX(double x)
{
    if (vertexSelection != nullptr) {
        vertexSelection->pos = glm::vec3(float(x), vertexSelection->pos[1], vertexSelection->pos[2]);

        // this id need to be updated
        int id = vertexSelection->id;
        int size = geom_mesh.Vertices.size();

        geom_mesh.mesh_vert_pos[id] = glm::vec4(vertexSelection->pos,1);
        geom_mesh.mesh_vert_pos[id+size] = glm::vec4(vertexSelection->pos,1);
        geom_mesh.mesh_vert_pos[id+size*2] = glm::vec4(vertexSelection->pos,1);

        geom_mesh.create();

        point.pnt_vert_pos.clear();
        point.pnt_vert_pos.push_back(glm::vec4(vertexSelection->pos,1));
        point.create();

        this->update();
    }
}

void MyGL::slot_receiveVertexTranslateY(double y) {
    if (vertexSelection != nullptr) {
        vertexSelection->pos = glm::vec3(vertexSelection->pos[0],float(y),vertexSelection->pos[2]);

        int id = vertexSelection->id;
        int size = geom_mesh.Vertices.size();

        geom_mesh.mesh_vert_pos[id] = glm::vec4(vertexSelection->pos,1);
        geom_mesh.mesh_vert_pos[id+size] = glm::vec4(vertexSelection->pos,1);
        geom_mesh.mesh_vert_pos[id+size*2] = glm::vec4(vertexSelection->pos,1);

        geom_mesh.create();

        point.pnt_vert_pos.clear();
        point.pnt_vert_pos.push_back(glm::vec4(vertexSelection->pos,1));
        point.create();

        this->update();
    }
}
void MyGL::slot_receiveVertexTranslateZ(double z) {
    if (vertexSelection != nullptr) {
        vertexSelection->pos = glm::vec3(vertexSelection->pos[0],vertexSelection->pos[1],float(z));

        int id = vertexSelection->id;
        int size = geom_mesh.Vertices.size();

        geom_mesh.mesh_vert_pos[id] = glm::vec4(vertexSelection->pos,1);
        geom_mesh.mesh_vert_pos[id+size] = glm::vec4(vertexSelection->pos,1);
        geom_mesh.mesh_vert_pos[id+size*2] = glm::vec4(vertexSelection->pos,1);

        geom_mesh.create();

        point.pnt_vert_pos.clear();
        point.pnt_vert_pos.push_back(glm::vec4(vertexSelection->pos,1));
        point.create();

        this->update();
    }
}


void MyGL::slot_receiveFaceColorR(double x)
{
//    printf("slot_recieveFaceColorR entered!\n");

    if (faceSelection != nullptr) {
        faceSelection->color = glm::vec3(float(x), faceSelection->color[1], faceSelection->color[2]);

        // start vertex
        int id = faceSelection->start_edge->id;
    //    printf("%d\n", id);
        int size = geom_mesh.Vertices.size();

        geom_mesh.mesh_vert_col[id] = glm::vec4(faceSelection->color,1);
        geom_mesh.mesh_vert_col[id+size] = glm::vec4(faceSelection->color,1);
        geom_mesh.mesh_vert_col[id+size*2] = glm::vec4(faceSelection->color,1);

        HalfEdge* lastEdge = faceSelection->start_edge->next;

        while (lastEdge != faceSelection->start_edge)
        {
            id = lastEdge->id;
            geom_mesh.mesh_vert_col[id] = glm::vec4(faceSelection->color,1);
            geom_mesh.mesh_vert_col[id+size] = glm::vec4(faceSelection->color,1);
            geom_mesh.mesh_vert_col[id+size*2] = glm::vec4(faceSelection->color,1);

            lastEdge = lastEdge->next;
        }

        geom_mesh.create();
        this->update();
    }
}

void MyGL::slot_receiveFaceColorG(double y)
{
    if (faceSelection != nullptr) {
        faceSelection->color = glm::vec3(faceSelection->color[0], float(y), faceSelection->color[2]);
        // start vertex
        int id = faceSelection->start_edge->id;
    //    printf("%d\n", id);
        int size = geom_mesh.Vertices.size();

        geom_mesh.mesh_vert_col[id] = glm::vec4(faceSelection->color,1);
        geom_mesh.mesh_vert_col[id+size] = glm::vec4(faceSelection->color,1);
        geom_mesh.mesh_vert_col[id+size*2] = glm::vec4(faceSelection->color,1);

        HalfEdge* lastEdge = faceSelection->start_edge->next;

        while (lastEdge != faceSelection->start_edge)
        {
            id = lastEdge->id;

            geom_mesh.mesh_vert_col[id] = glm::vec4(faceSelection->color,1);
            geom_mesh.mesh_vert_col[id+size] = glm::vec4(faceSelection->color,1);
            geom_mesh.mesh_vert_col[id+size*2] = glm::vec4(faceSelection->color,1);

            lastEdge = lastEdge->next;
        }

        geom_mesh.create();
        this->update();
    }
}

void MyGL::slot_receiveFaceColorB(double z)
{
    if (faceSelection != nullptr) {
        faceSelection->color = glm::vec3(faceSelection->color[0], faceSelection->color[1], float(z));

        // start vertex
        int id = faceSelection->start_edge->id;
    //    printf("%d\n", id);
        int size = geom_mesh.Vertices.size();

        geom_mesh.mesh_vert_col[id] = glm::vec4(faceSelection->color,1);
        geom_mesh.mesh_vert_col[id+size] = glm::vec4(faceSelection->color,1);
        geom_mesh.mesh_vert_col[id+size*2] = glm::vec4(faceSelection->color,1);

        HalfEdge* lastEdge = faceSelection->start_edge->next;

        while (lastEdge != faceSelection->start_edge)
        {
            id = lastEdge->id;
    //        printf("%d\n", id);
            geom_mesh.mesh_vert_col[id] = glm::vec4(faceSelection->color,1);
            geom_mesh.mesh_vert_col[id+size] = glm::vec4(faceSelection->color,1);
            geom_mesh.mesh_vert_col[id+size*2] = glm::vec4(faceSelection->color,1);

            lastEdge = lastEdge->next;
        }
        geom_mesh.create();
        this->update();
    }
}

void MyGL::addVertex(HalfEdge* edge, int mode)
{
    HalfEdge* HE1 = edge;
    HalfEdge* HE2 = edge->sym;

    Vertex* v1 = HE1->vert;
    Vertex* v2 = HE2->vert;

    HalfEdge* HE1B = new HalfEdge(geom_mesh.HalfEdges.size());
    HalfEdge* HE2B = new HalfEdge(geom_mesh.HalfEdges.size() + 1);
    Vertex* v3 = new Vertex(geom_mesh.Vertices.size());

    if (mode == 0)
        v3->pos = glm::vec3((v1->pos[0]+v2->pos[0])/2, (v1->pos[1]+v2->pos[1])/2, (v1->pos[2]+v2->pos[2])/2);
    else
        v3->pos = glm::vec3((edge->vert->pos[0]+edge->vert_before->pos[0]+centroid[edge->face->id]->pos[0]+\
                centroid[edge->sym->face->id]->pos[0])/4, (edge->vert->pos[1]+edge->vert_before->pos[1]+\
                centroid[edge->face->id]->pos[1]+centroid[edge->sym->face->id]->pos[1])/4, \
                (edge->vert->pos[2]+edge->vert_before->pos[2]+centroid[edge->face->id]->pos[2]+\
                centroid[edge->sym->face->id]->pos[2])/4);

    v3->edge = HE1;

    HE1B->vert = v1;
    HE2B->vert = v2;
    HE1B->vert_before = v3;
    HE2B->vert_before = v3;
    HE1->vert = v3;
    HE2->vert = v3;

    HE1B->face = HE1->face;
    HE2B->face = HE2->face;

    HE1B->next = HE1->next;
    HE2B->next = HE2->next;
    HE1->next = HE1B;
    HE2->next = HE2B;

    HE1->sym = HE2B;
    HE2B->sym = HE1;
    HE2->sym = HE1B;
    HE1B->sym = HE2;

    geom_mesh.Vertices.push_back(v3);
    geom_mesh.HalfEdges.push_back(HE1B);
    geom_mesh.HalfEdges.push_back(HE2B);

    if (mode == 1)
    {
        // mid stores information face -> all midpoints
        mid[edge->face->id].push_back(v3);
        mid[edge->sym->face->id].push_back(v3);
        // midpoint store information edge -> midpoint
        midpoint[edge->id] = v3;
        midpoint[edge->sym->id] = v3;
    }
}

void MyGL::slot_addVertex()
{
    addVertex(edgeSelection, 0);

    point.pnt_vert_pos.clear();

    Vertex* v3 = geom_mesh.Vertices[geom_mesh.Vertices.size()-1];

    emit sig_updateVertexX(v3->pos[0]);
    emit sig_updateVertexY(v3->pos[1]);
    emit sig_updateVertexZ(v3->pos[2]);

    point.pnt_vert_pos.push_back(glm::vec4(v3->pos,1));

    point.create();
    geom_mesh.create();
    this->update();
}

// count how many edges do face have
int count_edge(Face* face)
{
   int num = 1;

   HalfEdge* tmp = face->start_edge->next;

   while (tmp->id != face->start_edge->id)
   {
       num += 1;
       tmp = tmp->next;
   }

   return num;
}


void MyGL::slot_Triangulate()
{
    Face* FACE1 = faceSelection; // face to start with

    line.lne_vert_pos.clear();

    while (count_edge(FACE1) > 3)
    {
        Face* FACE2 = new Face(geom_mesh.Faces.size());
        HalfEdge* HE_0 = FACE1->start_edge;

        HalfEdge* HE_A = new HalfEdge(geom_mesh.Vertices.size());
        HalfEdge* HE_B = new HalfEdge(geom_mesh.Vertices.size()+1);
        //step 1
        HE_A->vert = HE_0->vert;
        HalfEdge* lastlastEdge = HE_0->next;
        while (lastlastEdge->next->next != HE_0)
            lastlastEdge = lastlastEdge->next;
//        HE_B->vert = HE_0->next->next->vert;
        HE_B->vert = lastlastEdge->vert;
        HE_A->sym = HE_B;
        HE_B->sym = HE_A;

        // step 2
        HE_A->face = FACE2;
        HE_0->next->face = FACE2;
        HE_0->next->next->face = FACE2;
        HE_B->face = FACE1;

        FACE2->start_edge = HE_A;
        FACE2->color = glm::vec3(1-FACE1->color[0],1-FACE1->color[1],1-FACE1->color[2]);
        FACE2->id = geom_mesh.Faces.size();

        //step 3
//        HE_B->next = HE_0->next->next->next;
        HE_B->next = lastlastEdge->next;
//        HE_0->next->next->next = HE_A;
        lastlastEdge->next = HE_A;
        HE_A->next = HE_0->next;
        HE_0->next = HE_B;

        // step 4: add extra vert_before infromation & pushback
        HE_A->vert_before = HE_B->vert;
        HE_B->vert_before = HE_A->vert;
        geom_mesh.HalfEdges.push_back(HE_A);
        geom_mesh.HalfEdges.push_back(HE_B);
        geom_mesh.Faces.push_back(FACE2);

        // step 5: draw added hafedges
        line.lne_vert_pos.push_back(glm::vec4(HE_A->vert->pos, 1));
        line.lne_vert_pos.push_back(glm::vec4(HE_B->vert->pos, 1));

        FACE1 = FACE2;
    }
    line.create();
}

void MyGL::Quadrangle(Face* face, Vertex* centroid, std::vector<HalfEdge*> mid)
{
   std::vector<HalfEdge> tmp_edge;
   HalfEdge* startEdge = face->start_edge;
   tmp_edge.push_back(*(startEdge));
   startEdge = startEdge->next;
   int n = 1;

   while (startEdge != face->start_edge)
   {
       tmp_edge.push_back(*(startEdge));
       startEdge = startEdge->next;
       n++;
   }
   //tmp_edge stores all original halfedge information locally

    for (int i = 0; i < mid.size(); i++)
    {
        geom_mesh.HalfEdges.resize(geom_mesh.HalfEdges.size());

        if (i == mid.size()-1)
        {
            HalfEdge* new_1 = new HalfEdge(geom_mesh.HalfEdges.size());
            HalfEdge* new_2 = new HalfEdge(geom_mesh.HalfEdges.size()+1);

            new_1->face = face;
            new_2->face = face;
            new_1->vert = centroid;
            new_2->vert = mid[i]->vert; //right side midpoint

            HalfEdge* tmp = new HalfEdge(-1);

            for (int j = 0; j < n; j++)
            {
                if (tmp_edge[j].vert->id == mid[i]->vert->id)
                {
                    int id = tmp_edge[j].next->id;
                    tmp = geom_mesh.HalfEdges[id];
                }
            }

            new_1->next = new_2;
            new_2->next = tmp;
            tmp->next->next = new_1;

            new_1->vert_before = tmp->next->vert;
            new_2->vert_before = centroid;
            face->start_edge = new_1;
            face->color = glm::vec3(1, 0.0, 0.0);

            centroid->edge = new_1;
            mid[i]->vert->edge = new_2;

            geom_mesh.HalfEdges.push_back(new_1);
            geom_mesh.HalfEdges.push_back(new_2);
        }
        else
        {
            Face* face_new = new Face(geom_mesh.Faces.size());

            HalfEdge* new_1 = new HalfEdge(geom_mesh.HalfEdges.size());
            HalfEdge* new_2 = new HalfEdge(geom_mesh.HalfEdges.size()+1);

            new_1->face = face_new;
            new_2->face = face_new;
            new_1->vert = centroid;
            new_2->vert = mid[i]->vert; //right side midpoint

            HalfEdge* tmp = new HalfEdge(-1);

            for (int j = 0; j < n; j++)
            {
                if (tmp_edge[j].vert->id == mid[i]->vert->id)
                {
                    int id = tmp_edge[j].next->id;
                    tmp = geom_mesh.HalfEdges[id];
                }
            }

            new_1->next = new_2;
            new_2->next = tmp;
            tmp->next->next = new_1;

            new_1->vert_before = tmp->next->vert;
            new_2->vert_before = centroid;
            face_new->start_edge = new_1;
            face_new->color = glm::vec3(1, 0.0, 0.0);

            centroid->edge = new_1;
            mid[i]->vert->edge = new_2;

            geom_mesh.Faces.push_back(face_new);
            geom_mesh.HalfEdges.push_back(new_1);
            geom_mesh.HalfEdges.push_back(new_2);
        }

    }
}

void MyGL::slot_subdivision()
{
    centroid.clear();
    mid.clear();
    midpoint.clear();

    int size_HalfEdge = geom_mesh.HalfEdges.size();
    int size_Vertex = geom_mesh.Vertices.size();
    int size_Face = geom_mesh.Faces.size();

    // centroid construction
    for (int i = 0; i < geom_mesh.Faces.size(); i++)
    {
        centroid[geom_mesh.Faces[i]->id] = new Vertex(-1);

        HalfEdge* tmp = geom_mesh.Faces[i]->start_edge->next;
        int num = count_edge(geom_mesh.Faces[i]);

        glm::vec3 tmp_pos = glm::vec3(geom_mesh.Faces[i]->start_edge->vert->pos[0],\
                geom_mesh.Faces[i]->start_edge->vert->pos[1], geom_mesh.Faces[i]->start_edge->vert->pos[2]);

        while (tmp != geom_mesh.Faces[i]->start_edge)
        {
            tmp_pos += tmp->vert->pos;
            tmp = tmp->next;
        }
        centroid[geom_mesh.Faces[i]->id]->pos = glm::vec3(tmp_pos[0]/num, tmp_pos[1]/num, tmp_pos[2]/num);

//        point.pnt_vert_pos.push_back(glm::vec4(centroid[geom_mesh.Faces[i]]->pos,1));
    }

    // midpoint construction
    bool duplicate = false;

    std::vector<Vertex> tmp_vertices;
    std::vector<HalfEdge> tmp_halfedges;
    std::vector<Face> tmp_faces;

    // store original information
    for (int i = 0; i < geom_mesh.Vertices.size(); i++)
        tmp_vertices.push_back(*(geom_mesh.Vertices[i]));

    for (int i = 0; i < geom_mesh.HalfEdges.size(); i++)
        tmp_halfedges.push_back(*(geom_mesh.HalfEdges[i]));

    for (int i = 0; i < geom_mesh.Faces.size(); i++)
        tmp_faces.push_back(*(geom_mesh.Faces[i]));

    for (int i = 0; i < size_HalfEdge; i++)
    {
        duplicate = false;

        for (int j = 0; j < i; j++)
        {
            if (tmp_halfedges[i].sym->id == tmp_halfedges[j].id)
            {
                duplicate = true;
                break;
            }
        }
        if (duplicate == false)
            addVertex(geom_mesh.HalfEdges[i], 1);
    }

    int test_1 = geom_mesh.HalfEdges.size();
    int test_2 = geom_mesh.Vertices.size();

    // halfedge size here should be 48 for cube case.
    // vertex size here should be 8(original)+12(midpoint)=20

    // smooth the original vertices
    for (int i = 0; i < size_Vertex; i++)
    {

        glm::vec3 sum_e = glm::vec3(0,0,0);
        glm::vec3 sum_f = glm::vec3(0,0,0);
        int n = 0;

        // loop 3 times
        for (int j = 0; j < geom_mesh.HalfEdges.size(); j++)
        {
            if (geom_mesh.HalfEdges[j]->vert->id == geom_mesh.Vertices[i]->id)
            {
                sum_e += geom_mesh.HalfEdges[j]->vert_before->pos;
                sum_f += centroid[geom_mesh.HalfEdges[j]->face->id]->pos;
                n += 1;
            }
        }

        printf("%d", n);

        geom_mesh.Vertices[i]->pos = glm::vec3((n-2)*1.0/n * geom_mesh.Vertices[i]->pos[0], \
                (n-2)*1.0/n * geom_mesh.Vertices[i]->pos[1], (n-2)*1.0/n * geom_mesh.Vertices[i]->pos[2]) +
                glm::vec3(1.0/(n*n) * sum_e[0], 1.0/(n*n) * sum_e[1], 1.0/(n*n) * sum_e[2]) + \
                glm::vec3(1.0/(n*n) * sum_f[0], 1.0/(n*n) * sum_f[1], 1.0/(n*n) * sum_f[2]);

        n = 0;
    }

    // midpoint should be bind to halfedge instead of face
    std::vector<Vertex*> test;
    for (int i = 0; i < 6; i++)
        for (int j = 0; j < mid[geom_mesh.Faces[i]->id].size(); j++)
            test.push_back(mid[geom_mesh.Faces[i]->id][j]);


    std::vector<HalfEdge*> mid_edge;

    // quadrangle face
    for (int i = 0; i < size_Face; i++) // each face
    {
        mid_edge.clear();
        for (int j = 0; j < mid[geom_mesh.Faces[i]->id].size(); j++) // each midpoint on face
        {

            if (mid[geom_mesh.Faces[i]->id][j]->edge->face->id != geom_mesh.Faces[i]->id)
                mid_edge.push_back(mid[geom_mesh.Faces[i]->id][j]->edge->next->sym);
            else
                mid_edge.push_back(mid[geom_mesh.Faces[i]->id][j]->edge);
        }

        Quadrangle(geom_mesh.Faces[i], centroid[geom_mesh.Faces[i]->id], mid_edge);
    }

    for(int m = 0; m < geom_mesh.HalfEdges.size(); m++){
        for(int n = m+1; n < geom_mesh.HalfEdges.size(); n++){
            glm::vec3 start_pos1, start_pos2;
            HalfEdge *H1 = geom_mesh.HalfEdges[m], *H2 = geom_mesh.HalfEdges[n];
            while(H1->next!=geom_mesh.HalfEdges[m]){
                H1 = H1->next;
            }
            start_pos1 = H1->vert->pos;
            while(H2->next!=geom_mesh.HalfEdges[n]){
                H2 = H2->next;
            }
            start_pos2 = H2->vert->pos;
            if(start_pos1 == geom_mesh.HalfEdges[n]->vert->pos && start_pos2 == geom_mesh.HalfEdges[m]->vert->pos){
                geom_mesh.HalfEdges[m]->sym = geom_mesh.HalfEdges[n];
                geom_mesh.HalfEdges[n]->sym = geom_mesh.HalfEdges[m];
                break;
            }
        }
    }


    geom_mesh.destroy();
    geom_mesh.create();
}

void MyGL::slot_extrude()
{
    HalfEdge *HE = faceSelection->start_edge;
    glm::vec3 normal = glm::normalize(glm::cross(HE->next->vert->pos - HE->vert->pos, HE->next->next->vert->pos - HE->next->vert->pos));
    int edge_count = geom_mesh.HalfEdges.size();

    do{
        if(HE == faceSelection->start_edge){

            Vertex *V3 = new Vertex(geom_mesh.Vertices.size());
            Vertex *V4 = new Vertex(geom_mesh.Vertices.size()+1);

            HalfEdge *HE1 = HE;
            HalfEdge *HE2 = HE->sym;

            Vertex *V1 = HE1->vert;
            HE1->vert = V3;
            HalfEdge *HE1_prev = HE1;
            Vertex *V2 = HE2->vert;
            V3->pos = V1->pos + 0.1f * normal;
            V4->pos = V2->pos + 0.1f * normal;
            while(HE1_prev->next != HE1){
                HE1_prev = HE1_prev->next;
            }
            HE1_prev->vert = V4;

            HalfEdge *HE1B = new HalfEdge, *HE2B = new HalfEdge;
            HE1->sym = HE1B;
            HE2->sym = HE2B;
            HE1B->sym = HE1;
            HE2B->sym = HE2;
            HE1B->vert = V4;
            HE2B->vert = V1;
            HE1B->id = geom_mesh.HalfEdges.size();
            HE2B->id = geom_mesh.HalfEdges.size()+1;

            Face *new_F = new Face;
            HalfEdge *HE3 = new HalfEdge, *HE4 = new HalfEdge;
            faceSelection->id = geom_mesh.Faces.size();
            HE3->id = geom_mesh.HalfEdges.size()+2;
            HE4->id = geom_mesh.HalfEdges.size()+3;

            HE1B->face = new_F;
            HE2B->face = new_F;
            HE3->face = new_F;
            HE4->face = new_F;
            HE3->vert = V3;
            HE4->vert = V2;
            HE1B->next = HE4;
            HE4->next = HE2B;
            HE2B->next = HE3;
            HE3->next = HE1B;

            V1->edge = HE2B;
            V2->edge = HE2;
            V3->edge = HE1;
            V4->edge = HE1B;

            new_F->color = glm::vec3((double)rand()/(RAND_MAX),(double)rand()/(RAND_MAX),(double)rand()/(RAND_MAX));
            new_F->start_edge = HE1B;

            geom_mesh.Faces.push_back(new_F);
            geom_mesh.HalfEdges.push_back(HE1B);
            geom_mesh.HalfEdges.push_back(HE2B);
            geom_mesh.HalfEdges.push_back(HE3);
            geom_mesh.HalfEdges.push_back(HE4);
            geom_mesh.Vertices.push_back(V3);
            geom_mesh.Vertices.push_back(V4);
        }

        else if(HE->next == faceSelection->start_edge){
            HalfEdge *HE1 = HE;
            HalfEdge *HE2 = HE->sym;
            while(HE2->next != HE->sym)
                HE2 = HE2->next;
            Vertex *V1 = HE2->vert;
            HE2 = HE2->next;
            Vertex *V2 = HE2->vert;
            Vertex *V3 = HE1->next->sym->vert;
            HalfEdge *HE1_prev = HE1;
            while(HE1_prev->next != HE1){
                HE1_prev = HE1_prev->next;
            }
            Vertex *V4 = HE1_prev->vert;
            HE1->vert = V3;

            HalfEdge *HE1B = new HalfEdge(geom_mesh.HalfEdges.size());
            HalfEdge *HE2B = new HalfEdge(geom_mesh.HalfEdges.size() + 1);
            HE1->sym = HE1B;
            HE1B->sym = HE1;
            HE2->sym = HE2B;
            HE2B->sym = HE2;
            HE1B->vert = V4;
            HE2B->vert = V1;

            Face *new_F = new Face(geom_mesh.Faces.size());
            HalfEdge *HE3 = new HalfEdge(geom_mesh.HalfEdges.size()+2);
            HalfEdge *HE4 = new HalfEdge(geom_mesh.HalfEdges.size()+3);

            HE1B->face = faceSelection;
            HE2B->face = faceSelection;
            HE3->face = faceSelection;
            HE4->face = faceSelection;
            HE3->vert = V3;
            HE4->vert = V2;
            HE1B->next = HE4;
            HE4->next = HE2B;
            HE2B->next = HE3;
            HE3->next = HE1B;

            V1->edge = HE2B;
            V2->edge = HE2;
            V3->edge = HE1;
            V4->edge = HE1B;

            new_F->color = glm::vec3((double)rand()/(RAND_MAX),(double)rand()/(RAND_MAX),(double)rand()/(RAND_MAX));
            new_F->start_edge = HE1B;

            geom_mesh.Faces.push_back(new_F);
            geom_mesh.HalfEdges.push_back(HE1B);
            geom_mesh.HalfEdges.push_back(HE2B);
            geom_mesh.HalfEdges.push_back(HE3);
            geom_mesh.HalfEdges.push_back(HE4);
        }
        else{
            Vertex *V3 = new Vertex(geom_mesh.Vertices.size());
            HalfEdge *HE1 = HE;
            HalfEdge *HE2 = HE->sym;
            while(HE2->next != HE->sym)
                HE2 = HE2->next;
            Vertex *V1 = HE2->vert;
            HE2 = HE2->next;
            Vertex *V2 = HE2->vert;
            HE1->vert = V3;
            HalfEdge *HE1_prev = HE1;
            V3->pos = V1->pos + 0.1f * normal;
            while(HE1_prev->next != HE1){
                HE1_prev = HE1_prev->next;
            }
            Vertex *V4 = HE1_prev->vert;

            HalfEdge *HE1B = new HalfEdge(geom_mesh.HalfEdges.size());
            HalfEdge *HE2B = new HalfEdge(geom_mesh.HalfEdges.size()+1);
            HE1->sym = HE1B;
            HE1B->sym = HE1;
            HE2->sym = HE2B;
            HE2B->sym = HE2;
            HE1B->vert = V4;
            HE2B->vert = V1;

            Face *new_F = new Face(geom_mesh.Faces.size());
            HalfEdge *HE3 = new HalfEdge(geom_mesh.HalfEdges.size()+2);
            HalfEdge *HE4 = new HalfEdge(geom_mesh.HalfEdges.size()+3);
            HE1B->face = faceSelection;
            HE2B->face = faceSelection;
            HE3->face = faceSelection;
            HE4->face = faceSelection;
            HE3->vert = V3;
            HE4->vert = V2;
            HE1B->next = HE4;
            HE4->next = HE2B;
            HE2B->next = HE3;
            HE3->next = HE1B;

            V1->edge = HE2B;
            V2->edge = HE2;
            V3->edge = HE1;
            V4->edge = HE1B;

            new_F->color = glm::vec3((double)rand()/(RAND_MAX),(double)rand()/(RAND_MAX),(double)rand()/(RAND_MAX));
            new_F->start_edge = HE1B;

            geom_mesh.Faces.push_back(new_F);
            geom_mesh.HalfEdges.push_back(HE1B);
            geom_mesh.HalfEdges.push_back(HE2B);
            geom_mesh.HalfEdges.push_back(HE3);
            geom_mesh.HalfEdges.push_back(HE4);
            geom_mesh.Vertices.push_back(V3);
        }

        HE = HE->next;

    } while(HE!=faceSelection->start_edge);

    int edge_count_new = geom_mesh.HalfEdges.size();

    for(int m = edge_count; m < edge_count_new; m++){
        for(int n = m+1; n < edge_count_new; n++){
            glm::vec3 start_pos1, start_pos2;
            HalfEdge *H1 = geom_mesh.HalfEdges[m], *H2 = geom_mesh.HalfEdges[n];
            while(H1->next!=geom_mesh.HalfEdges[m]){
                H1 = H1->next;
            }
            start_pos1 = H1->vert->pos;
            while(H2->next!=geom_mesh.HalfEdges[n]){
                H2 = H2->next;
            }
            start_pos2 = H2->vert->pos;
            if(start_pos1 == geom_mesh.HalfEdges[n]->vert->pos && start_pos2 == geom_mesh.HalfEdges[m]->vert->pos){
                geom_mesh.HalfEdges[m]->sym = geom_mesh.HalfEdges[n];
                geom_mesh.HalfEdges[n]->sym = geom_mesh.HalfEdges[m];
                break;
            }
        }
    }

    geom_mesh.destroy();
    geom_mesh.create();
}
