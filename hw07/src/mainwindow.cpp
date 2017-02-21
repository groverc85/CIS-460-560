#include "mainwindow.h"
#include <ui_mainwindow.h>
#include "cameracontrolshelp.h"
#include <QFileDialog>
#include <QFile>
#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>
#include <iostream>
#include <random>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <math.h>
#include <scene/joint.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->mygl->setFocus();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionQuit_triggered()
{
    QApplication::exit();
}

void MainWindow::on_actionCamera_Controls_triggered()
{
    CameraControlsHelp* c = new CameraControlsHelp();
    c->show();
}

void MainWindow::on_actionLoadObj_triggered()
{

//    ui->Vertex->clear();
//    ui->Vertex->children().clear();

    QString filename = QFileDialog::getOpenFileName(0, QString("Load obj"), QDir::currentPath().append(QString("../..")), QString("*.obj"));

    std::ifstream infile(filename.toStdString());

    std::string identifier;
    int i = 0,j = 0, k = 0; //index
    float x,y,z; //vertex pos/normal x,y,z value
    int pos_size;

    std::vector<Vertex*> inputVertex;
    std::vector<Face*> inputFace;
    std::vector<HalfEdge*> inputHalfEdge;
    std::vector<glm::vec4> normal;

    ui->mygl->geom_mesh.mesh_vert_col.clear();
    ui->mygl->geom_mesh.mesh_vert_nor.clear();
    ui->mygl->geom_mesh.mesh_vert_pos.clear();
    ui->mygl->geom_mesh.Vertices.clear();
    ui->mygl->geom_mesh.Faces.clear();
    ui->mygl->geom_mesh.HalfEdges.clear();

    for (std::string line; std::getline(infile, line);)
    {
        std::stringstream line_seg;
        line_seg << line;
        line_seg >> identifier;

        if (identifier == "v")
        {
            line_seg >> x >> y >> z;
            Vertex* vertex = new Vertex(i);
            inputVertex.push_back(vertex);

            inputVertex[i]->pos = glm::vec3(x,y,z);
            i += 1;
        }
        else if (identifier == "vn")
        {
            line_seg >> x >> y >> z;
            normal.push_back(glm::vec4(x,y,z,1));
            j += 1;
        }
        else if (identifier == "f")
        {
            std::vector<int> pos;
            std::vector<std::string> seg;
            std::string seg_each;
            int a;

            while (line_seg >> seg_each)
            {
                seg.push_back(seg_each);
            }

            for (a = 0; a < seg.size(); a++)
            {
                int beginIdx = seg[a].find('/');
                std::string tmp = seg[a].substr(0, beginIdx);

                int beginIdx2 = seg[a].rfind('/');
                std::string tmp2 = seg[a].substr(beginIdx2+1, seg[a].length()-beginIdx-1);
                pos.push_back(std::stoi(tmp.c_str()));
                ui->mygl->geom_mesh.mesh_vert_nor.push_back(normal[std::stoi(tmp2)-1]);
            }

            Face* face = new Face(k); // set up id
            inputFace.push_back(face);
            inputFace[k]->color = glm::vec3((double)rand()/(RAND_MAX),(double)rand()/(RAND_MAX),(double)rand()/(RAND_MAX)); //random number
            pos_size = pos.size();

            for (a = 0; a < pos.size(); a++)
            {
                HalfEdge* halfedge = new HalfEdge(k*pos.size()+a); // set up id
                inputHalfEdge.push_back(halfedge);
            }

            for (a = 0; a < pos.size(); a++)
            {
                inputHalfEdge[k*pos.size()+a]->face = inputFace[k];
                inputHalfEdge[k*pos.size()+a]->vert = inputVertex[pos[a]-1];
                inputVertex[pos[a]-1]->edge = inputHalfEdge[k*pos.size()+a];
            }
            inputFace[k]->start_edge = inputHalfEdge[k*pos.size()+0];

            inputHalfEdge[k*pos.size()+0]->vert_before = inputVertex[pos[pos.size()-1]-1];
            for (a = 1; a < pos.size(); a++)
            {
                inputHalfEdge[k*pos.size()+a]->vert_before = inputVertex[pos[a-1]-1];
            }

            for (a = 0; a < pos.size()-1; a++)
            {
                inputHalfEdge[k*pos.size()+a]->next = inputHalfEdge[k*pos.size()+a+1];
            }
            inputHalfEdge[k*pos.size()+pos.size()-1]->next = inputHalfEdge[k*pos.size()+0];

            k += 1;
        }
    }
    int size_vertex = i;
    int size_normal = j;
    int size_halfedge = k * pos_size;
    int size_face = k;

    for (i = 0; i < size_halfedge; i++)
        for (j = i+1; j < size_halfedge; j++)
        {
            if (inputHalfEdge[i]->vert->id == inputHalfEdge[j]->vert->id &&\
                    inputHalfEdge[i]->vert_before->id == inputHalfEdge[j]->vert_before->id)
            {
                inputHalfEdge[i]->sym = inputHalfEdge[j];
                inputHalfEdge[j]->sym = inputHalfEdge[i];
            }
        }

    for (i = 0; i < size_vertex; i++)
        ui->mygl->geom_mesh.Vertices.push_back(inputVertex[i]);
    for (j = 0; j < size_halfedge; j++)
        ui->mygl->geom_mesh.HalfEdges.push_back(inputHalfEdge[j]);
    for (k = 0; k < size_face; k++)
        ui->mygl->geom_mesh.Faces.push_back(inputFace[k]);

    ui->mygl->geom_mesh.destroy();
    ui->mygl->geom_mesh.create();
    ui->mygl->cube_display = 0;
}
