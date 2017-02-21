#include <meshcomponent.h>

Vertex::Vertex(){
    pos = glm::vec3(0,0,0);
    edge = nullptr;
    id = -1;
}

Vertex::Vertex(int id){
    pos = glm::vec3(0,0,0);
    edge = nullptr;
    this->id = id;
    QString Qvert = QString("Vert")+QString().number(id);
    setText(Qvert);
}

Vertex::Vertex(glm::vec3 pos, HalfEdge* edge, int id) {
    this->pos = pos;
    this->edge = edge;
    this->id = id;
}

void Vertex::setPos(glm::vec3 pos) {
    this->pos = pos;
}

void Vertex::setHalfEdge(HalfEdge* edge) {
    this->edge = edge;
//    edge->vert = this;
}

void Vertex::setID(int id) {
    this->id = id;
}

void Vertex::operator=(const Vertex &v) {
    this->pos = v.pos;
    this->edge = v.edge;
    this->id = v.id;
}

void Vertex::addItem(QListWidgetItem* c)
{
    c->setText(QString::number(((Vertex*)c)->id));
}

Face::Face(){
    start_edge = nullptr;
    color = glm::vec3(0,0,0);
    id = -1;
}

Face::Face(int id){
    start_edge = nullptr;
    color = glm::vec3(0,0,0);
    this->id = id;
    QString Qface = QString("Face")+QString().number(id);
    setText(Qface);
}

Face::Face(HalfEdge *start_edge, int id){
    this->start_edge = start_edge;
    color = glm::vec3(0,0,0);
    this->id = id;
}

Face::Face(HalfEdge* start_edge, glm::vec3 color, int id) {
    this->start_edge = start_edge;
    this->color = color;
    this->id = id;
}

void Face::operator=(const Face &f) {
    this->start_edge = f.start_edge;
    this->color = f.color;
    this->id = f.id;
}

void Face::setHalfEdge(HalfEdge* edge) {
    this->start_edge = edge;
}

void Face::setColor(glm::vec3 color) {
    this->color = color;
}

void Face::setID(int id) {
    this->id = id;
}

HalfEdge::HalfEdge(){
    face = nullptr;
    vert = nullptr;
    vert_before = nullptr;
    next = nullptr;
    sym = nullptr;
    id = -1;
}

HalfEdge::HalfEdge(int id){
    face = nullptr;
    vert = nullptr;
    vert_before = nullptr;
    next = nullptr;
    sym = nullptr;
    this->id = id;
    QString Qedge = QString("HalfEdge")+QString().number(id);
    setText(Qedge);
}


HalfEdge::HalfEdge(Face* face, Vertex* vert, Vertex* vert_before, HalfEdge* next, HalfEdge* sym, int id) {
    this->face = face;
    this->vert = vert;
    this->vert_before = vert_before;
    this->next = next;
    this->sym = sym;
    this->id = id;
}

void HalfEdge::operator=(const HalfEdge &h) {
    this->face = h.face;
    this->vert = h.vert;
    this->vert_before = h.vert_before;
    this->next = h.next;
    this->sym = h.sym;
    this->id = h.id;
}

void HalfEdge::setFace(Face* face) {
    this->face = face;
}

void HalfEdge::setVert(Vertex* vert) {
    this->vert = vert;
}

void HalfEdge::setNext(HalfEdge* next) {
    this->next = next;
}

void HalfEdge::setVertexBefore(Vertex* vert_before) {
    this->vert_before = vert_before;
}

void HalfEdge::setSym(HalfEdge* sym) {
    this->sym = sym;
}

void HalfEdge::setID(int id) {
    this->id = id;
}
