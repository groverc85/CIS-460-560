#ifndef JOINT_H
#define JOINT_H

#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <drawable.h>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <shaderprogram.h>
#include <QFile>


class Joint : public QTreeWidgetItem, public Drawable
{
public:
    int id;
    QString name;
    Joint* parent;
    std::vector<Joint*> children;
    glm::vec3 pos;
    glm::quat rot;
    glm::mat4 bind;

    glm::mat4 GetLocalTransformation();
    glm::mat4 GetOverallTransformation();
    void addChild(Joint* c);    
    void setBindMatrix();
    void getTransformationMatrix(glm::mat4 matArray[100]);
    void getBindMatrix(glm::mat4 matArray[100]);


    Joint(GLWidget277 *context);
    Joint();
    ~Joint();
    Joint(Joint* j);
    Joint(int id, QString name, Joint* parent, glm::vec3 pos, glm::quat rot);
    void draw(GLWidget277 &f, ShaderProgram &prog_flat);
    void create();
    void create(bool selected);
};


#endif // JOINT_H
