#include "mygl.h"
#include <la.h>

#include <iostream>
#include <QApplication>
#include <QKeyEvent>


MyGL::MyGL(QWidget *parent)
    : GLWidget277(parent),
      geom_cylinder(this), geom_sphere(this), geom_cube(this), geom_cone(this), geom_pipe(this),
      //TODO: When you make your Cube instance, add it to this init list
      prog_lambert(this), prog_flat(this)
{
}

MyGL::~MyGL()
{
    makeCurrent();
    glDeleteVertexArrays(1, &vao);
    geom_cylinder.destroy();
    geom_sphere.destroy();
    geom_cube.destroy();
    geom_cone.destroy();
    geom_pipe.destroy();
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

    //Create the instances of Cylinder Sphere and cube.
    geom_cylinder.create();
    geom_sphere.create();
    geom_cube.create();
    geom_cone.create();
    geom_pipe.create();

    Root = new Node();

    // lower limb should rotate around its joint
    // this step is to move the rotation pivot from its geometry center to its edge center.
    TranslateNode LowerLimbTranslate_1(0,0.5f,0);
    // We need to rotate 180 degrees so that the vertex part should be facing downwards.
    // FOR TAs: make changes to the degree and you can rotate this part, e.c 150 degrees.
    RotateNode LowerLimbRotate(180,1,0,0);
    ScaleNode LowerLimbScale(1.0f,2.0f,1.0f);
    // move this geometry back to the position it should be, which connects with upper limb.
    TranslateNode LowerLimbTranslate_2(0.0f,1.0f,0.0f);
    LowerLimb = new Node(LowerLimbTranslate_1.MatrixStored() * LowerLimbRotate.MatrixStored() * \
                               LowerLimbScale.MatrixStored() * LowerLimbTranslate_2.MatrixStored(), \
                               QString("LowerLimb"));
    LowerLimb->setGeomColor(glm::vec4(0,1,0,1));
    LowerLimb->setGeometry(&geom_cone);

    TranslateNode UpperLimbTranslate(0,-1.0f,0);
    RotateNode UpperLimbRotate(0,0,0,0);
    ScaleNode UpperLimbScale(1.0f,1.0f,0.5f);
    UpperLimb = new Node(UpperLimbTranslate.MatrixStored() * UpperLimbRotate.MatrixStored() * UpperLimbScale.MatrixStored(), \
                   QString("UpperLimb"));
    UpperLimb->setGeomColor(glm::vec4(1,1,0,1));
    UpperLimb->setGeometry(&geom_cylinder);
    UpperLimb->addChild(LowerLimb);

    TranslateNode HeadTranslate(0,1.0f,0);
    RotateNode HeadRotate(0,0,0,0);
    ScaleNode HeadScale(1,1,0.5f);
    Head = new Node(HeadTranslate.MatrixStored() * HeadRotate.MatrixStored() * HeadScale.MatrixStored(), \
              QString("Head"));
    Head->setGeomColor(glm::vec4(0,0,1,1));
    Head->setGeometry(&geom_sphere);

    TranslateNode BodyTranslate(0,0,0);
    RotateNode BodyRotate(0,0,0,0);
    ScaleNode BodyScale(1,1,2);
    Body = new Node(BodyTranslate.MatrixStored() * BodyRotate.MatrixStored() * BodyScale.MatrixStored(), \
              QString("Body"));
    Body->setGeometry(&geom_cube);
    Body->setGeomColor(glm::vec4(0.5,0.5,0.5,1));
    Body->addChild(Head);
    Body->addChild(UpperLimb);

    TranslateNode RootTranslate(0,0,0);
    RotateNode RootRotate(0,0,0,0);
    ScaleNode RootScale(1,1,1);
    Root = new Node(RootTranslate.MatrixStored() * RootRotate.MatrixStored() * RootScale.MatrixStored(), \
              QString("Root"));
    Root->addChild(Body);

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

    emit sig_scenegraph(this->Root);
}

void MyGL::resizeGL(int w, int h)
{
    //This code sets the concatenated view and perspective projection matrices used for
    //our scene's camera view.
    glm::vec4 c1(1.1933f, 0, 1.1933f, 0);
    glm::vec4 c2(0.9856f, 1.9712f, -0.9856f, 0);
    glm::vec4 c3(0.5785f, -0.5785f, -0.5785f, 11.9484f);
    glm::vec4 c4(0.5774f, -0.5774f, -0.5774f, 12.1244f);
    glm::mat4 viewproj(c1, c2, c3, c4);
    //Transpose since GLM is column major and I wrote out the rows of the matrix
    viewproj = glm::transpose(viewproj);

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

    //VVV CLEAR THIS CODE WHEN YOU IMPLEMENT SCENE GRAPH TRAVERSAL VVV///////////////////
#define NOPE
#ifdef NOPE

    //Create a model matrix. This one scales the sphere uniformly by 3, then translates it by <-2,0,0>.
    //Note that we have to transpose the model matrix before passing it to the shader
    //This is because OpenGL expects column-major matrices, but you've
    //implemented row-major matrices.

//    glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(-2,0,0)) * glm::scale(glm::mat4(1.0f), glm::vec3(3,3,3));
//    //Send the geometry's transformation matrix to the shader
//    prog_lambert.setModelMatrix(model);
//    //Draw the example sphere using our lambert shader
//    prog_lambert.draw(geom_sphere);

    //Now do the same to render the cylinder
    //We've rotated it -45 degrees on the Z axis, then translated it to the point <2,2,0>
//    model = glm::translate(glm::mat4(1.0f), glm::vec3(2,2,0)) * glm::rotate(glm::mat4(1.0f), glm::radians(-45.0f), glm::vec3(0,0,1));
//    prog_lambert.setModelMatrix(model);
//    prog_lambert.draw(geom_cylinder);

//    model = glm::translate(glm::mat4(1.0f), glm::vec3(0,0,0)) * glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(0,0,1)) \
//             * glm::scale(glm::mat4(1.0f), glm::vec3(2,2,2));;
//    prog_lambert.setModelMatrix(model);
//    prog_lambert.draw(geom_cube);

//    glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0,0,0)) * glm::scale(glm::mat4(1.0f), glm::vec3(2,2,2));
//    prog_lambert.setModelMatrix(model);
//    prog_lambert.draw(geom_cone);

//    glm::mat4 model = glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(0,0,1)) * glm::scale(glm::mat4(1.0f), glm::vec3(2,2,2));
//    prog_lambert.setModelMatrix(model);
//    prog_lambert.draw(geom_pipe);

#endif
    //^^^ CLEAR THIS CODE WHEN YOU IMPLEMENT SCENE GRAPH TRAVERSAL ^^^/////////////////

    //Here is a good spot to call your scene graph traversal function.
    traverse(Root, Root->MatrixStored(), prog_lambert);
}

void MyGL::keyPressEvent(QKeyEvent *e)
{
    // http://doc.qt.io/qt-5/qt.html#Key-enum
    if (e->key() == Qt::Key_Escape) {
        QApplication::quit();
    }
}

void MyGL::traverse(Node* N, glm::mat4 T, ShaderProgram p) {
    T = T * N->MatrixStored();
    for (int i = 0; i < (int)N->children.size(); i++) {
        traverse(N->children[i], T, p);
    }

    if (N->geometry != NULL) {
        p.setModelMatrix(T);
        prog_lambert.setGeometryColor(N->color);
        p.draw(*(N->geometry));
    }
}

// ItemClicked's slot
void MyGL::slot_changeGeometry(QTreeWidgetItem* p, int n) {
    Head->setGeomColor(Head_color);
    Body->setGeomColor(Body_color);
    UpperLimb->setGeomColor(UpperLimb_color);
    LowerLimb->setGeomColor(LowerLimb_color);

    if (p->text(n) == "Head")
        Head->setGeomColor(glm::vec4(1,1,1,1));
    else if (p->text(n) == "Body")
        Body->setGeomColor(glm::vec4(1,1,1,1));
    else if (p->text(n) == "UpperLimb")
        UpperLimb->setGeomColor(glm::vec4(1,1,1,1));
    else if (p->text(n) == "LowerLimb")
        LowerLimb->setGeomColor(glm::vec4(1,1,1,1));
    else;
}
