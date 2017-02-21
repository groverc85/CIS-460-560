#include "joint.h"
#include "mygl.h"

static const int WF_IDX_COUNT = 72+2;
static const int WF_VERT_COUNT = 36+2;
static const int WF_COL_COUNT = 36+2;

Joint::Joint(GLWidget277 *context) : Drawable(context)
{
    mode = GL_LINES;
    id = -1;
    name = QString("Joint");
    parent = nullptr;
    pos = glm::vec3(0,0,0);
    rot = glm::quat(1,0,0,0);
    setText(0, name);
    bind = glm::mat4(0);
}

Joint::Joint() : Drawable(context)
{
    mode = GL_LINES;
    id = -1;
    name = QString("Joint");
    parent = nullptr;
    pos = glm::vec3(0,0,0);
    rot = glm::quat(1,0,0,0);
    setText(0, name);
    bind = glm::mat4(0);
}

Joint::Joint(Joint* j) : Drawable(context)
{
    this->id = j->id;
    this->name = j->name;
    this->parent = j->parent;
    for (int i = 0; i < children.size(); i++)
    {
        this->children[i] = j->children[i];
    }
    this->pos = j->pos;
    this->rot = j->rot;
    this->bind = j->bind;
}

Joint::Joint(int id, QString name, Joint* parent, glm::vec3 pos, glm::quat rot) : Drawable(context)
{
    this->id = id;
    this->name = name;
    this->parent = parent;

    this->pos = pos;
    this->rot = rot;
    this->bind = this->GetOverallTransformation();
}

Joint::~Joint()
{
    while (children.size() > 0)
    {
        children.erase(children.begin());
        delete children[0];
    }
    children.clear();

    if (parent != nullptr)
    {
        for (int i = 0; i < parent->children.size(); i++)
        {
            if (this == parent->children[i])
                parent->children.erase(parent->children.begin()+i);
        }
    }
}

glm::mat4 Joint::GetLocalTransformation()
{
    return glm::translate(glm::mat4(1.0f), pos) * glm::mat4_cast(rot) * glm::scale(glm::mat4(1.0f), glm::vec3(1, 1, 1));
}

glm::mat4 Joint::GetOverallTransformation()
{
    if (parent != nullptr)
    {
        return parent->GetOverallTransformation() * GetLocalTransformation();
    }
    else
    {
        return GetLocalTransformation();
    }
}

void Joint::addChild(Joint* c)
{
    children.push_back(c);
    c->parent = this;
    QTreeWidgetItem::addChild(c);
    c->create();
}


void Joint::draw(GLWidget277 &f, ShaderProgram &prog_flat) {
    prog_flat.setModelMatrix(GetOverallTransformation());
    prog_flat.draw(*this);

    if (children.size()>0) {
        for (int i = 0; i < children.size(); i++) {
            children[i]->draw(f,prog_flat);
        }
    }
}

//WIREFRAME PART
void createWireframeVertexPositions(glm::vec4 (&wf_vert_pos)[WF_VERT_COUNT]){
    for (int i= 0; i<12; i++) {
        glm::vec4 v = glm::rotate(glm::mat4(1.0f), glm::radians(i*30.0f), glm::vec3(0, 0, 1)) * glm::vec4(0,0.5f,0,1);
        wf_vert_pos[i] = v;
    }
    for (int i= 12; i<24; i++) {
        glm::vec4 v = glm::rotate(glm::mat4(1.0f), glm::radians(i*30.0f), glm::vec3(1, 0, 0)) * glm::vec4(0,0.5f,0,1);
        wf_vert_pos[i] = v;
    }
    for (int i= 24; i<36; i++) {
        glm::vec4 v = glm::rotate(glm::mat4(1.0f), glm::radians(i*30.0f), glm::vec3(0, 1, 0)) * glm::vec4(0.5f,0,0,1);
        wf_vert_pos[i] = v;
    }
    //If no parent
    wf_vert_pos[36] = glm::vec4(0,0,0,1);
    wf_vert_pos[37] = glm::vec4(0,0,0,1);
}


void createWireframeVertexPositions(glm::vec4 (&wf_vert_pos)[WF_VERT_COUNT], Joint* parent, Joint* self){
    for (int i= 0; i<12; i++) {
        glm::vec4 v = glm::rotate(glm::mat4(1.0f), glm::radians(i*30.0f), glm::vec3(0, 0, 1)) * glm::vec4(0,0.5f,0,1);
        wf_vert_pos[i] = v;
    }
    for (int i= 12; i<24; i++) {
        glm::vec4 v = glm::rotate(glm::mat4(1.0f), glm::radians(i*30.0f), glm::vec3(1, 0, 0)) * glm::vec4(0,0.5f,0,1);;
        wf_vert_pos[i] = v;
    }
    for (int i= 24; i<36; i++) {
        glm::vec4 v = glm::rotate(glm::mat4(1.0f), glm::radians(i*30.0f), glm::vec3(0, 1, 0)) * glm::vec4(0.5f,0,0,1);;
        wf_vert_pos[i] = v;
    }

    wf_vert_pos[36] = glm::vec4(0,0,0,1);
    wf_vert_pos[37] = glm::inverse(self->GetLocalTransformation()) * glm::vec4(0,0,0,1);
}

void createWireframeIndices(GLuint (&wf_idx)[WF_IDX_COUNT]) {
    for (int i= 0; i<22; i+=2) {
        wf_idx[i] = i/2;
        wf_idx[i+1] = (i/2) + 1;
    }
    wf_idx[22] = 11;
    wf_idx[23] = 0;

    for (int i= 24; i<46; i+=2) {
        wf_idx[i] = i/2;
        wf_idx[i+1] = (i/2) + 1;
    }
    wf_idx[46] = 23;
    wf_idx[47] = 12;

    for (int i= 48; i<70; i+=2) {
        wf_idx[i] = i/2;
        wf_idx[i+1] = (i/2) + 1;
    }
    wf_idx[70] = 35;
    wf_idx[71] = 24;

    //Last two
    wf_idx[72] = 36;
    wf_idx[73] = 37;

}


void createWireframeColors(glm::vec4 (&wf_col)[WF_COL_COUNT]) {
    for (int i= 0; i<12; i++) {
        wf_col[i] = glm::vec4(0,0,0.8,1);
    }
    for (int i= 12; i<24; i++) {
        wf_col[i] = glm::vec4(0.8,0,0,1);
    }
    for (int i= 24; i<WF_COL_COUNT; i++) {
        wf_col[i] = glm::vec4(0,0.8,0,1);
    }
    wf_col[36] = glm::vec4(1,1,0,1);
    wf_col[37] = glm::vec4(1,0,1,1);
}


void createWireframeColors(glm::vec4 (&wf_col)[WF_COL_COUNT], bool selected) {
    if (!selected) {
        for (int i= 0; i<12; i++) {
            wf_col[i] = glm::vec4(0,0,0.8,1);
        }
        for (int i= 12; i<24; i++) {
            wf_col[i] = glm::vec4(0.8,0,0,1);
        }
        for (int i= 24; i<WF_COL_COUNT; i++) {
            wf_col[i] = glm::vec4(0,0.8,0,1);
        }
        wf_col[36] = glm::vec4(0.5f,0.5f,0,1);
        wf_col[37] = glm::vec4(1,1,1,1);

    } else {
        for (int i = 0; i<WF_COL_COUNT;i++) {
            wf_col[i]=glm::vec4(0.25,1,0.5,1);
        }
    }
}


void Joint::create() {
    GLuint wf_idx[WF_IDX_COUNT];
    glm::vec4 wf_vert_pos[WF_VERT_COUNT];
    glm::vec4 wf_col[WF_COL_COUNT];

    if (parent != nullptr) {
        createWireframeVertexPositions(wf_vert_pos, parent,this);
    } else {
        createWireframeVertexPositions(wf_vert_pos);
    }

    createWireframeIndices(wf_idx);
    createWireframeColors(wf_col);

    count = WF_IDX_COUNT;

    generateIdx();
    // Tell OpenGL that we want to perform subsequent operations on the VBO referred to by bufIdx
    // and that it will be treated as an element array buffer (since it will contain triangle indices)
    context->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufIdx);
    // Pass the data stored in cyl_idx into the bound buffer, reading a number of bytes equal to
    // CYL_IDX_COUNT multiplied by the size of a GLuint. This data is sent to the GPU to be read by shader programs.
    context->glBufferData(GL_ELEMENT_ARRAY_BUFFER, WF_IDX_COUNT * sizeof(GLuint), wf_idx, GL_STATIC_DRAW);

    // The next few sets of function calls are basically the same as above, except bufPos and bufNor are
    // array buffers rather than element array buffers, as they store vertex attributes like position.
    generatePos();
    context->glBindBuffer(GL_ARRAY_BUFFER, bufPos);
    context->glBufferData(GL_ARRAY_BUFFER, WF_VERT_COUNT * sizeof(glm::vec4), wf_vert_pos, GL_STATIC_DRAW);

    generateCol();
    context->glBindBuffer(GL_ARRAY_BUFFER, bufCol);
    context->glBufferData(GL_ARRAY_BUFFER, WF_COL_COUNT * sizeof(glm::vec4), wf_col, GL_STATIC_DRAW);
}

void Joint::create(bool selected) {
    GLuint wf_idx[WF_IDX_COUNT];
    glm::vec4 wf_vert_pos[WF_VERT_COUNT];
    glm::vec4 wf_col[WF_COL_COUNT];

    if (parent != nullptr) {
        createWireframeVertexPositions(wf_vert_pos, parent,this);
    } else {
        createWireframeVertexPositions(wf_vert_pos);
    }

    createWireframeIndices(wf_idx);
    createWireframeColors(wf_col,selected);

    count = WF_IDX_COUNT;

    generateIdx();
    // Tell OpenGL that we want to perform subsequent operations on the VBO referred to by bufIdx
    // and that it will be treated as an element array buffer (since it will contain triangle indices)
    context->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufIdx);
    // Pass the data stored in cyl_idx into the bound buffer, reading a number of bytes equal to
    // CYL_IDX_COUNT multiplied by the size of a GLuint. This data is sent to the GPU to be read by shader programs.
    context->glBufferData(GL_ELEMENT_ARRAY_BUFFER, WF_IDX_COUNT * sizeof(GLuint), wf_idx, GL_STATIC_DRAW);

    // The next few sets of function calls are basically the same as above, except bufPos and bufNor are
    // array buffers rather than element array buffers, as they store vertex attributes like position.
    generatePos();
    context->glBindBuffer(GL_ARRAY_BUFFER, bufPos);
    context->glBufferData(GL_ARRAY_BUFFER, WF_VERT_COUNT * sizeof(glm::vec4), wf_vert_pos, GL_STATIC_DRAW);

    generateCol();
    context->glBindBuffer(GL_ARRAY_BUFFER, bufCol);
    context->glBufferData(GL_ARRAY_BUFFER, WF_COL_COUNT * sizeof(glm::vec4), wf_col, GL_STATIC_DRAW);
}

//GLenum Joint::drawMode() {
//    return GL_LINES;
//}

void Joint::setBindMatrix()
{
    bind = glm::mat4(glm::inverse(GetOverallTransformation()));
}

void Joint::getTransformationMatrix(glm::mat4 matArray[100]) {
    matArray[id] = GetOverallTransformation();
    for (int i = 0; i < childCount(); i++) {
        matArray[children[i]->id] = children[i]->GetOverallTransformation();
        if (children[i]->childCount() > 0) {
            children[i]->getTransformationMatrix(matArray);
        }
    }
}

void Joint::getBindMatrix(glm::mat4 matArray[100]) {
    matArray[id] = bind;
    for (int i = 0; i < childCount(); i++) {
        matArray[children[i]->id] = children[i]->bind;
        if (children[i]->childCount() > 0) {
            children[i]->getBindMatrix(matArray);
        }
    }
}
