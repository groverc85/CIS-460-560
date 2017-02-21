#ifndef MESHCOMPONENT_H
#define MESHCOMPONENT_H

#include <meshcomponent.h>
#include <glwidget277.h>
#include <utils.h>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLShaderProgram>
#include <la.h>
#include <iostream>
#include <QListWidget>
#include <QListWidgetItem>

class Vertex;
class Face;
class HalfEdge;

class Vertex : public QListWidgetItem
{
public:
    glm::vec3 pos;
    HalfEdge* edge;
    int id;
    glm::vec3 nor;

    Vertex();
    Vertex(int id);
    Vertex(glm::vec3 pos, HalfEdge* edge, int id);
    void operator=(const Vertex &v);
    void setPos(glm::vec3 pos);
    void setHalfEdge(HalfEdge* edge);
    void setID(int id);

    virtual void addItem(QListWidgetItem*);
};

class Face  : public QListWidgetItem
{
public:
    HalfEdge* start_edge;
    glm::vec3 color;
    int id;

    Face();
    Face(int id);
    Face(HalfEdge *start_edge, int id);
    Face(HalfEdge* start_edge, glm::vec3 color, int id);
    void operator=(const Face &f);
    void setHalfEdge(HalfEdge* edge);
    void setColor(glm::vec3 color);
    void setID(int id);
};

class HalfEdge  : public QListWidgetItem
{
public:
    Face* face;
    Vertex* vert;
    Vertex* vert_before;
    HalfEdge* next;
    HalfEdge* sym;
    int id;

    HalfEdge();
    HalfEdge(int id);
    HalfEdge(Face* face, Vertex* vert, Vertex* vert_before, HalfEdge* next, HalfEdge* sym, int id);
    void operator=(const HalfEdge &h);
    void setFace(Face* face);
    void setVertexBefore(Vertex* vert_before);
    void setVert(Vertex* vert);
    void setNext(HalfEdge* next);
    void setSym(HalfEdge* sym);
    void setID(int id);
};


#endif // MESHCOMPONENT_H
