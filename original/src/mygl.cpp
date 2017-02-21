#include "mygl.h"
#include <la.h>

#include <iostream>
#include <QApplication>
#include <QKeyEvent>
#include <tuple>
#include <iostream>


MyGL::MyGL(QWidget *parent)
    : GLWidget277(parent),
      geom_cube(this),
      prog_lambert(this), prog_flat(this),
      gl_camera(),chunk(this,glm::vec3(0,0,0)),
      chunkManager(),geom_plus(this)
{
    // Connect the timer to a function so that when the timer ticks the function is executed
    connect(&timer, SIGNAL(timeout()), this, SLOT(timerUpdate()));
    // Tell the timer to redraw 60 times per second
    timer.start(16);
    setFocusPolicy(Qt::ClickFocus);
}

MyGL::~MyGL()
{
    makeCurrent();
    glDeleteVertexArrays(1, &vao);
    geom_cube.destroy();
    chunk.destroy();
    geom_plus.destroy();

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
    glClearColor(0.37f, 0.74f, 1.0f, 1);

    printGLErrorLog();

    // Create a Vertex Attribute Object
    glGenVertexArrays(1, &vao);

    //Create the instance of Cube
    geom_cube.create();
    geom_plus.create();
    // Create and set up the diffuse shader
    prog_lambert.create(":/glsl/lambert.vert.glsl", ":/glsl/lambert.frag.glsl");
    // Create and set up the flat lighting shader
    prog_flat.create(":/glsl/flat.vert.glsl", ":/glsl/flat.frag.glsl");

    // Set a color with which to draw geometry since you won't have one
    // defined until you implement the Node classes.
    // This makes your geometry render green.
    prog_lambert.setGeometryColor(glm::vec4(0,1,0,1));

    /// by Xi Yang
    setMouseTracking(true);
    QPoint mousePos(width()/2,height()/2);
    mousePos=mapToGlobal(mousePos);
    QCursor::setPos(mousePos);
    setCursor(Qt::BlankCursor);

    // We have to have a VAO bound in OpenGL 3.2 Core. But if we're not
    // using multiple VAOs, we can just bind one once.
//    vao.bind();
    glBindVertexArray(vao);

   scene.CreateTestScene();
   //updateChunk();

}

void MyGL::resizeGL(int w, int h)
{
    //This code sets the concatenated view and perspective projection matrices used for
    //our scene's camera view.
//    gl_camera = Camera(w, h);
    gl_camera = Camera(w, h, glm::vec3(scene.dimensions.x/2, scene.dimensions.y/2 + 2, scene.dimensions.z/2),
                           glm::vec3(scene.dimensions.x/2, scene.dimensions.y/2+2, scene.dimensions.z/2+1), glm::vec3(0,1,0));
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
  //  prog_lambert.setModelMatrix(glm::translate(glm::mat4(), glm::vec3(0, 0, 0)));

    //prog_lambert.draw(chunk);
    updateChunk();
    GLDrawScene();
    ///-------------added by Xi Yang-------------------
    ///----------------Draw plus mark------------------
    glDisable(GL_DEPTH_TEST);
    prog_flat.setViewProjMatrix(glm::mat4(1.0f));
    prog_flat.setModelMatrix(glm::mat4(1.0f));
    prog_flat.draw(geom_plus);
    glEnable(GL_DEPTH_TEST);
    ///----------------Draw plus mark------------------
}

void MyGL::GLDrawScene()
{
    if(chunkManager.renderList.size()!=0){
        for(Chunk* chunk:chunkManager.renderList){
            if(chunk->isActive()==true){
             prog_lambert.setModelMatrix(glm::translate(glm::mat4(), glm::vec3(0, 0, 0)));

             prog_lambert.draw(*chunk);
            }
        }
    }

//chunkManager.renderList.clear();
}

void MyGL::keyPressEvent(QKeyEvent *e)
{
    Perlin* perlin_generator = new Perlin();

    float amount = 1.0f;
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
        //gl_camera.TranslateAlongLook(amount);
        TranslateAlongLook_new(amount);

//         z direction higher boundary
        scene.lowerBoundZ += 1;
        scene.higherBoundZ += 1;

        for (int x = scene.lowerBoundX; x < scene.higherBoundX; x++)
        {
            int height = perlin_generator->OctavePerlin(x/64.0, 0, (scene.higherBoundZ-1)/64.0, 6, 1) * 64;
            for (int i = 0; i < height; i++)
            {
                scene.containBlock[std::make_tuple(x, i, scene.higherBoundZ-1)] = true;
                scene.scene[std::make_tuple(x, i, scene.higherBoundZ-1)] = Default;
            }
            for (int y = scene.lowerBoundY; y < scene.higherBoundY; y++)
            {
                scene.containBlock[std::make_tuple(x, y, scene.lowerBoundZ-1)] = false;
            }

        }

    } else if (e->key() == Qt::Key_S) {

        //gl_camera.TranslateAlongLook(-amount);
        TranslateAlongLook_new(-amount);

        // z direction lower boundary
//        scene.lowerBoundZ -= 1;
//        scene.higherBoundZ -= 1;

//        for (int x = scene.lowerBoundX; x < scene.higherBoundX; x++)
//        {
//            int height = perlin_generator->OctavePerlin(x/64.0, 0, scene.lowerBoundZ/64.0, 6, 1) * 64;
//            for (int i = 0; i < height; i++)
//            {
//                scene.containBlock[std::make_tuple(x, i, scene.lowerBoundZ)] = true;
//                scene.scene[std::make_tuple(x, i, scene.lowerBoundZ)] = Default;
//            }
//            for (int y = scene.lowerBoundY; y < scene.higherBoundY; y++)
//            {
//                scene.containBlock[std::make_tuple(x, y, scene.higherBoundZ)] = false;
//            }

//        }

        //gl_camera.TranslateAlongLook(-amount);
    } else if (e->key() == Qt::Key_D) {
        //gl_camera.TranslateAlongRight(amount);
        TranslateAlongRight_new(amount);

        // x direction lower boundary
//        scene.lowerBoundX -= 1;
//        scene.higherBoundX -= 1;

//        for(int z = scene.lowerBoundZ; z < scene.higherBoundZ; z++)
//        {
//            int height = perlin_generator->OctavePerlin(scene.lowerBoundX/64.0, 0, z/64.0, 6, 1) * 64;
//            for (int i = 0; i < height; i++)
//            {
//                scene.containBlock[std::make_tuple(scene.lowerBoundX, i, z)] = true;
//                scene.scene[std::make_tuple(scene.lowerBoundX, i, z)] = Default;
//            }
//            for (int y = scene.lowerBoundY; y < scene.higherBoundY; y++)
//            {
//                scene.containBlock[std::make_tuple(scene.higherBoundX, y, z)] = false;
//            }
//        }

    } else if (e->key() == Qt::Key_A) {
        //gl_camera.TranslateAlongRight(-amount);
        TranslateAlongRight_new(-amount);

        // x direction higher boundary
//        scene.lowerBoundX += 1;
//        scene.higherBoundX += 1;

//        for(int z = scene.lowerBoundZ; z < scene.higherBoundZ; z++)
//        {
//            int height = perlin_generator->OctavePerlin((scene.higherBoundX-1)/64.0, 0, z/64.0, 6, 1) * 64;
//            for (int i = 0; i < height; i++)
//            {
//                scene.containBlock[std::make_tuple(scene.higherBoundX-1, i, z)] = true;
//                scene.scene[std::make_tuple(scene.higherBoundX-1, i, z)] = Default;
//            }
//            for (int y = scene.lowerBoundY; y < scene.higherBoundY; y++)
//            {
//                scene.containBlock[std::make_tuple(scene.lowerBoundX-1, y, z)] = false;
//            }
//        }
    } else if (e->key() == Qt::Key_Q) {
        gl_camera.TranslateAlongUp(-amount);

        // y direction lower

//        scene.lowerBoundX += 1;
//        scene.higherBoundX -= 1;
//        scene.lowerBoundZ += 1;
//        scene.higherBoundZ -= 1;

//        for (int z = scene.lowerBoundZ; z < scene.higherBoundX; z++)
//            for (int y = scene.lowerBoundY; y < scene.higherBoundY; y++)
//            {
//                scene.containBlock[std::make_tuple(scene.higherBoundX, y, z)] = false;
//                scene.containBlock[std::make_tuple(scene.lowerBoundX-1, y, z)] = false;
//            }

//        for (int x = scene.lowerBoundX; x < scene.higherBoundX; x++)
//            for (int y = scene.lowerBoundY; y < scene.higherBoundY; y++)
//            {
//                scene.containBlock[std::make_tuple(x, y, scene.higherBoundZ)] = false;
//                scene.containBlock[std::make_tuple(x, y, scene.lowerBoundZ-1)] = false;
//            }

    } else if (e->key() == Qt::Key_E) {
        gl_camera.TranslateAlongUp(amount);

        // y direction higher

//        scene.lowerBoundX -= 1;
//        scene.higherBoundX += 1;
//        scene.lowerBoundZ -= 1;
//        scene.higherBoundZ += 1;

//        for (int z = scene.lowerBoundZ; z < scene.higherBoundX; z++)
//        {
//            int height_1 = perlin_generator->OctavePerlin((scene.higherBoundX-1)/64.0, 0, z/64.0, 6, 1) * 64;
//            int height_2 = perlin_generator->OctavePerlin((scene.lowerBoundX)/64.0, 0, z/64.0, 6, 1) * 64;
//            for (int i = 0; i < height_1; i++)
//            {
//                scene.containBlock[std::make_tuple(scene.higherBoundX-1, i, z)] = true;
//                scene.scene[std::make_tuple(scene.higherBoundX-1, i, z)] = Default;
//            }
//            for (int i = 0; i < height_2; i++)
//            {
//                scene.containBlock[std::make_tuple(scene.lowerBoundX, i, z)] = true;
//                scene.scene[std::make_tuple(scene.lowerBoundX, i, z)] = Default;
//            }
//        }
//        for (int x = scene.lowerBoundX; x < scene.higherBoundX; x++)
//        {
//            int height_1 = perlin_generator->OctavePerlin(x/64.0, 0, (scene.higherBoundZ-1)/64.0, 6, 1) * 64;
//            int height_2 = perlin_generator->OctavePerlin(x/64.0, 0, (scene.lowerBoundZ)/64.0, 6, 1) * 64;
//            for (int i = 0; i < height_1; i++)
//            {
//                scene.containBlock[std::make_tuple(x, i, scene.higherBoundZ-1)] = true;
//                scene.scene[std::make_tuple(x, i, scene.higherBoundZ-1)] = Default;
//            }
//            for (int i = 0; i < height_2; i++)
//            {
//                scene.containBlock[std::make_tuple(x, i, scene.lowerBoundZ)] = true;
//                scene.scene[std::make_tuple(x, i, scene.lowerBoundZ)] = Default;
//            }
//        }
    } else if (e->key() == Qt::Key_R) {
        gl_camera = Camera(this->width(), this->height());
    }else if (e->key() == Qt::Key_Space) {
        PlayerJump();
    }
    gl_camera.RecomputeAttributes();
    update();  // Calls paintGL, among other things
}


void MyGL::timerUpdate()
{
}
//---------------------------------------------------------------
//-----------------------------ADD BY YI HUANG--------------------
void MyGL::pushBlock(Chunk *chunk){
    std::map<std::tuple<int,int,int>, BlockType> s=scene.scene;
    std::map<std::tuple<int,int,int>, bool> active=scene.containBlock;
   // active=scene.containBlock;
    bool is_active;
    glm::vec3 pos=chunk->getPos();
    int width=chunk->CHUNK_SIZE;
    int lowerx=pos.x;
    int lowery=pos.y;
    int lowerz=pos.z;
    int highx=pos.x+width;
    int highy=pos.y+width;
    int highz=pos.z+width;
   for(int x=lowerx;x<highx;x++)
       for(int y=lowery;y<highy;y++)
           for(int z=lowerz;z<highz;z++){
            // loop every block in the map
        is_active=active[std::make_tuple(x,y,z)];
              //find the block belongs to which
       if(is_active){
            glm::vec3 C_Pos=chunk->getPos();
            if(x>=C_Pos.x &&x<C_Pos.x+chunk->CHUNK_SIZE)
                if(y>=C_Pos.y &&y<C_Pos.y+chunk->CHUNK_SIZE)
                    if(z>=C_Pos.z &&z<C_Pos.z+chunk->CHUNK_SIZE){

                        int cx=x-C_Pos.x;
                        int cy=y-C_Pos.y;
                        int cz=z-C_Pos.z;
                        chunk->active[cx][cy][cz]=true;
                        chunk->type[cx][cy][cz]=s[std::make_tuple(x,y,z)];

                    }
        }
    }


}
//-----------------------------------------------------------------
//-------------------------ADD BY YI HUANG--------------------------
void MyGL::updateChunk(){
    Perlin* perlin_generator=new Perlin();
    glm::vec3 eye=gl_camera.eye;
    if(first_load==true ||abs(scene.higherBoundX-oldhigherBoundX)>15 ||abs(scene.higherBoundY-oldhigherBoundY)>15 ||
         abs(scene.higherBoundZ-oldhigherBoundZ)>15){

    //  if(first_load==true||oldhigherBoundX-eye.x<16 ||oldhigherBoundY-eye.y<16 ||
      //      oldhigherBoundZ-eye.z<16 || eye.x-oldlowerBounderX<16||
      //      eye.y-oldlowerBounderY<16 || eye.z-oldlowerBounderZ<16){
          first_load = false;

          glm::ivec3 higherBound=glm::ivec3(scene.higherBoundX,scene.higherBoundY,scene.higherBoundZ);
          glm::ivec3 lowerBound=glm::ivec3(scene.lowerBoundX,scene.lowerBoundY,scene.lowerBoundZ);

          int gapsize=16;
          int chunkNumByRow=(higherBound.x-lowerBound.x)/16;

          for(int i=0;i<chunkNumByRow;i++){
              for(int j=0;j<chunkNumByRow;j++){
                  for(int k=0;k<chunkNumByRow;k++){
                      bool chunkExist=false;
                     int x=lowerBound.x+gapsize*i;
                     int y=lowerBound.y+gapsize*j;
                     int z=lowerBound.z+gapsize*k;
                     glm::vec3 Chunkpos= glm::vec3(x,y,z);
                     for(Chunk* chunk:chunkManager.allChunks){
                         if(chunk->getPos()==Chunkpos){
                             //this chunk is not new, need to be update
                             chunkManager.renderList.push_back(chunk);
                             chunkExist=true;
                             break;
                         }
                     }
                     if(chunkExist==false){               //this chunk is new and need to be creat first
                         Chunk* chunk=new Chunk(this,Chunkpos);
                         chunkManager.newLoadList.push_back(chunk);
                         chunkManager.updateList.push_back(chunk);
                         pushBlock(chunk);
                     }
                  }
              }
          }
          chunkManager.update(gl_camera);

          oldhigherBoundX=scene.higherBoundX;
          oldhigherBoundY=scene.higherBoundY;
          oldhigherBoundZ=scene.higherBoundZ;
          oldlowerBounderX=scene.lowerBoundX;
          oldlowerBounderY=scene.lowerBoundY;
          oldlowerBounderZ=scene.lowerBoundZ;

           }
}


void MyGL::updateBlock(Block block){
std::tuple<int,int,int> tempPos =block.pos;
int x=std::get<0>(tempPos);
int y=std::get<1>(tempPos);
int z=std::get<2>(tempPos);
int gapx=0,gapy=0,gapz=0;
while(x-16>=0){
    gapx++;
    x=x-16;
}
while(y-16>=0){
    gapy++;
    y=y-16;
}
while(z-16>=0){
    gapz++;
    z=z-16;
}
int posx=gapx*16;
int posy=gapy*16;
int posz=gapz*16;
glm::vec3 pos(posx,posy,posz);
for(Chunk* chunk:chunkManager.renderList){
    if(chunk->getPos()==pos){
        //chunk->active[x-posx][y-posy][z-posz]=block.isActive();
        chunk->active[x][y][z]=block.isActive();
        chunk->create();
        update();
    }
}
}
///---------------------added by Xi Yang------------------------
/// ------------------------------------------------------------
void MyGL::mousePressEvent(QMouseEvent *e)
{
    if(e->button()==Qt::LeftButton)
    {
        BreakBlock();
    }else if(e->button()==Qt::RightButton)
    {
        PlaceBlock();
    }
}
void MyGL::BreakBlock()
{
    glm::vec3 forward=gl_camera.look;
    glm::vec3 rayPos=gl_camera.eye;
    float t=gl_camera.near_clip;
    bool Intersect=false;
    while((t<gl_camera.far_clip)&& (Intersect==false))
    {
        rayPos=rayPos+t*forward;
        for(int x = scene.lowerBoundX; x < scene.higherBoundX; x++)
        {   if(Intersect)
                break;
            if((x-0.5-FLT_EPSILON<rayPos[0])&&(rayPos[0]<x+0.5+FLT_EPSILON))
            {
                for (int y = scene.lowerBoundY; y < scene.higherBoundY; y++)
                {
                    if(Intersect)
                                    break;
                    if((y-0.5-FLT_EPSILON<rayPos[1])&&(rayPos[1]<y+0.5+FLT_EPSILON))
                    {
                        for (int z = scene.lowerBoundZ; z < scene.higherBoundZ; z++)
                        {
                            if(Intersect)
                                            break;
                            if((scene.containBlock[std::make_tuple(x,y,z)] == true))
                            {
                                if((z-0.5-FLT_EPSILON<rayPos[2])&&(rayPos[2]<z+0.5+FLT_EPSILON))
                                {
                                    Intersect=true;
                                    //std::cout<<x<<" "<<y<<" "<<z<<std::endl;
                                    scene.containBlock[std::make_tuple(x,y,z)] = false;
                                    Block block;
                                    block.pos=std::make_tuple(x,y,z);
                                    block.SetActive(false);
                                    updateBlock(block);
                                    update();

                                }

                            }
                        }
                    }
                }
            }
        }

        t=t+0.01;
    }
}
void MyGL::PlaceBlock()
{
    glm::vec3 forward=gl_camera.look;
    glm::vec3 rayPos=gl_camera.eye;
    float t=gl_camera.near_clip;
    bool Intersect=false;
    while((t<gl_camera.far_clip)&& (Intersect==false))
    {
        rayPos=rayPos+t*forward;
        for(int x = scene.lowerBoundX; x < scene.higherBoundX; x++)
        {   if(Intersect)
                break;
            if((x-0.5<rayPos[0]-FLT_EPSILON)&&(rayPos[0]<x+0.5+FLT_EPSILON))
            {
                for (int y = scene.lowerBoundY; y < scene.higherBoundY; y++)
                {
                    if(Intersect)
                                    break;
                    if((y-0.5-FLT_EPSILON<rayPos[1])&&(rayPos[1]<y+0.5+FLT_EPSILON))
                    {
                        for (int z = scene.lowerBoundZ; z < scene.higherBoundZ; z++)
                        {
                            if(Intersect)
                                            break;
                            if((scene.containBlock[std::make_tuple(x,y,z)] == true))
                            {
                                if((z-0.5-FLT_EPSILON<rayPos[2])&&(rayPos[2]<z+0.5+FLT_EPSILON))
                                {
                                    Intersect=true;
                                    //std::cout<<x<<" "<<y<<" "<<z<<std::endl;
                                    int nx=x;
                                    int ny=y;
                                    int nz=z;
                                    for(int i=0;i<200;i++)
                                    {
                                        float s=0.001*i;
                                        rayPos=rayPos-s*forward;
                                        if(fabs(x+0.5-rayPos[0])<0.01)
                                        {
                                            nx=x+1;
                                            break;
                                        }
                                        if(fabs(x-0.5-rayPos[0])<0.01)
                                        {
                                            nx=x-1;
                                            break;
                                        }
                                        if(fabs(y+0.5-rayPos[1])<0.01)
                                        {
                                            ny=y+1;
                                            break;
                                        }
                                        if(fabs(y-0.5-rayPos[1])<0.01)
                                        {
                                            ny=y-1;
                                            break;
                                        }
                                        if(fabs(z+0.5-rayPos[2])<0.01)
                                        {
                                            nz=z+1;
                                            break;
                                        }
                                        if(fabs(z-0.5-rayPos[2])<0.01)
                                        {
                                            nz=z-1;
                                            break;
                                        }


                                    }

                                    scene.containBlock[std::make_tuple(nx,ny,nz)] = true;
                                    scene.scene[std::make_tuple(nx,ny,nz)]=Grass;
                                    Block block;
                                    block.pos=std::make_tuple(nx,ny,nz);
                                    block.SetActive(true);
                                    updateBlock(block);
                                    update();

                                }

                            }
                        }
                    }
                }
            }
        }

        t=t+0.01;
    }
}

void MyGL::TranslateAlongLook_new(float amt)
{
    glm::vec3 translation = gl_camera.look * amt;
    translation[1]=0;
    gl_camera.eye += translation;
    gl_camera.ref += translation;
    gl_camera.RecomputeAttributes();
    update();
    CollisionCheckLook(amt);
    GroundCheck();

}
void MyGL::TranslateAlongRight_new(float amt)
{
    glm::vec3 translation = gl_camera.right * amt;
    translation[1]=0;
    gl_camera.eye += translation;
    gl_camera.ref += translation;
    gl_camera.RecomputeAttributes();
    update();
    CollisionCheckRight(amt);
    GroundCheck();
}
void MyGL::CollisionCheckLook(float amt)
{
    glm::vec3 head=gl_camera.eye;
    bool collision=false;
    for(int y = scene.lowerBoundX; y < scene.higherBoundX; y++)
    {
        if(collision)
            break;
        if((y<head[1]+1)&&(y>head[1]-2))
        {
            for (int x = scene.lowerBoundY; x < scene.higherBoundY; x++)
            {
                if(collision)
                    break;
                if((head[0]-1<x)&&(head[0]+1>x))
                {
                    for (int z = scene.lowerBoundZ; z < scene.higherBoundZ; z++)
                    {
                        if((head[2]-1<z+FLT_EPSILON)&&(head[2]+1>z+FLT_EPSILON))
                          {
                            if((scene.containBlock[std::make_tuple(x,y,z)] == true))
                            {
                                //std::cout<<"collision!!"<<std::endl;
                             //std::cout<<"camera "<<head[0]<<" "<<head[1]<<" "<<head[2]<<std::endl;
                            //std::cout<<"block "<<x<<" "<<y<<" "<<z<<std::endl;
                            collision=true;
                            TranslateAlongLook_new(-amt/amt*0.1);
                            break;}

                        }
                    }

                }
            }

        }
    }
}
void MyGL::CollisionCheckRight(float amt)
{
    glm::vec3 head=gl_camera.eye;
    bool collision=false;
    for(int y = scene.lowerBoundX; y < scene.higherBoundX; y++)
    {
        if(collision)
            break;
        if((y<head[1]+1)&&(y>head[1]-2))
        {
            for (int x = scene.lowerBoundY; x < scene.higherBoundY; x++)
            {
                if(collision)
                    break;
                if((head[0]-1<x)&&(head[0]+1>x))
                {
                    for (int z = scene.lowerBoundZ; z < scene.higherBoundZ; z++)
                    {
                        if((scene.containBlock[std::make_tuple(x,y,z)] == true))
                          {
                            if((head[2]-1<z+FLT_EPSILON)&&(head[2]+1>z+FLT_EPSILON))
                            {
                                //std::cout<<"collision!!"<<std::endl;
                             //std::cout<<"camera "<<head[0]<<" "<<head[1]<<" "<<head[2]<<std::endl;
                           //std::cout<<"block "<<x<<" "<<y<<" "<<z<<std::endl;
                            collision=true;
                            TranslateAlongRight_new(-amt/amt*0.1);
                            break;}

                        }
                    }

                }
            }

        }
    }
}
void MyGL::mouseMoveEvent(QMouseEvent *e)
{
    int ht=height();
    int wd=width();
    float x=e->x();
    float  y=e->y();
    float sx=(2 * x/wd) - 1;
    float sy=1-(2*y/ht);
    glm::vec3 p=gl_camera.ref+sx*gl_camera.H+sy*gl_camera.V;
         glm::vec3 p2eye=p-gl_camera.eye;
         if(glm::dot(p2eye,glm::vec3(p2eye[0],0.0,p2eye[2]))<0)
             return;
         else
         {
             gl_camera.ref=p;
             gl_camera.RecomputeAttributes();
             update();
             QPoint mousePos(width()/2,height()/2);
             mousePos=mapToGlobal(mousePos);
             QCursor::setPos(mousePos);
         }


}
void MyGL::GroundCheck()
{
   glm::vec3 head=gl_camera.eye;
   //std::cout<<"camera "<<head[0]<<" "<<head[1]<<" "<<head[2]<<std::endl;
   bool FindGround=false;
   std::vector<int> underY;
   int maxY=0;
   for(int x = scene.lowerBoundX; x < scene.higherBoundX; x++)
    {
       if(FindGround)
           break;
       if((x+FLT_EPSILON>head[0]-1)&&(x-FLT_EPSILON<head[0]+1))
       {
           for (int z = scene.lowerBoundZ; z < scene.higherBoundZ; z++)
           {
               if(FindGround)
                   break;
               if((z+FLT_EPSILON>head[2]-1)&&(z-FLT_EPSILON<head[2]+1))
               {
                   for (int y = scene.lowerBoundY; y < scene.higherBoundY; y++)
                   {
                       if(FindGround)
                           break;
                       if((scene.containBlock[std::make_tuple(x,y,z)] == true))
                       {
                           if(fabs(head[1]-2-y)<0.01)
                           {
                               //std::cout<<"find "<<x<<" "<<y<<" "<<z<<std::endl;
                               FindGround=true;
                               return;
                           }else if(y-FLT_EPSILON<head[1]-2)
                           {
                               //std::cout<<"under "<<x<<" "<<y<<" "<<z<<std::endl;
                               underY.push_back(y);
                           }

                       }

                   }
               }
           }

       }

    }

    for(unsigned int i=0;i<underY.size();i++)
       {
           if(maxY<underY[i])
               maxY=underY[i];
       }
    //std::cout<<"maxY "<<maxY<<std::endl;
    int t=0;
    float s=head[1]-2-maxY;
    while(s>0.01)
    {
        s=s-PlayerFalling(t);
        t++;
    }

}
float MyGL::PlayerFalling(int t)
{
    float a=2;
    float ss;
    if(a*t<20)
    {
    ss=0.5*a*pow(t,2);
    gl_camera.eye[1]-=ss;
    gl_camera.ref[1] -= ss;
    gl_camera.RecomputeAttributes();
    update();
    }
    if(a*t==20)
    {
         ss=100+20*t;
        gl_camera.eye[1]-=ss;
        gl_camera.ref[1] -= ss;
        gl_camera.RecomputeAttributes();
        update();

    }
    return ss;
}
void MyGL::PlayerJump()
{
    glm::vec3 translation = gl_camera.look;
    translation[1]=1;
    gl_camera.eye += translation;
    gl_camera.ref += translation;
    gl_camera.RecomputeAttributes();
    GroundCheck();
    update();

}
///---------------------------------------------------------------
