#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPixmap>
#include <QFileDialog>
#include <QTextStream>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <iostream>
#include <QApplication>
#include <QKeyEvent>
#include <QImageWriter>
#include <QDebug>
#include <tiny_obj_loader.h>

//Poke around in this file if you want, but it's virtually uncommented!
//You won't need to modify anything in here to complete the assignment.


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    rasterizer(std::vector<Polygon>())
{
    ui->setupUi(this);
    setFocusPolicy(Qt::StrongFocus);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::DisplayQImage(QImage &i)
{
    QPixmap pixmap(QPixmap::fromImage(i));
    graphics_scene.addPixmap(pixmap);
    graphics_scene.setSceneRect(pixmap.rect());
    ui->scene_display->setScene(&graphics_scene);
}

void MainWindow::on_actionLoad_Scene_triggered()
{
    std::vector<Polygon> polygons;

    QString filename = QFileDialog::getOpenFileName(0, QString("Load Scene File"), QDir::currentPath().append(QString("../..")), QString("*.json"));
    int i = filename.length() - 1;
    while(QString::compare(filename.at(i), QChar('/')) != 0)
    {
        i--;
    }
    QStringRef local_path = filename.leftRef(i+1);

    QFile file(filename);
    if(!file.open(QIODevice::ReadOnly)){
        qWarning("Could not open the JSON file.");
        return;
    }
    QByteArray file_data = file.readAll();

    QJsonDocument jdoc(QJsonDocument::fromJson(file_data));
    //Read the mesh data in the file
    QJsonArray objects = jdoc.object()["objects"].toArray();
    for(int i = 0; i < objects.size(); i++)
    {
        std::vector<vec4> vert_pos, vert_col;
        QJsonObject obj = objects[i].toObject();
        QString type = obj["type"].toString();
        //Custom Polygon case
        if(QString::compare(type, QString("custom")) == 0)
        {
            QString name = obj["name"].toString();
            QJsonArray pos = obj["vertexPos"].toArray();
            for(int j = 0; j < pos.size(); j++)
            {
                QJsonArray arr = pos[j].toArray();
                vec4 p(arr[0].toDouble(), arr[1].toDouble(), arr[2].toDouble(), 1);
                vert_pos.push_back(p);
            }
            QJsonArray col = obj["vertexCol"].toArray();
            for(int j = 0; j < col.size(); j++)
            {
                QJsonArray arr = col[j].toArray();
                vec4 c(arr[0].toDouble(), arr[1].toDouble(), arr[2].toDouble(), 1);
                vert_col.push_back(c);
            }
            Polygon p(name, vert_pos, vert_col);
            polygons.push_back(p);
        }
        //Regular Polygon case
        else if(QString::compare(type, QString("regular")) == 0)
        {
            QString name = obj["name"].toString();
            int sides = obj["sides"].toInt();
            QJsonArray colorA = obj["color"].toArray();
            vec4 color(colorA[0].toDouble(), colorA[1].toDouble(), colorA[2].toDouble(),1);
            QJsonArray posA = obj["pos"].toArray();
            vec4 pos(posA[0].toDouble(), posA[1].toDouble(), posA[2].toDouble(),1);
            float rot = obj["rot"].toDouble();
            QJsonArray scaleA = obj["scale"].toArray();
            vec4 scale(scaleA[0].toDouble(), scaleA[1].toDouble(), scaleA[2].toDouble(),1);
            Polygon p(name, sides, color, pos, rot, scale);
            polygons.push_back(p);
        }
        //OBJ file case
        else if(QString::compare(type, QString("obj")) == 0)
        {
            QString name = obj["name"].toString();
            QString filename = local_path.toString().append(obj["filename"].toString());
            Polygon p = LoadOBJ(filename, name);
            p.SetTexture(new QImage(local_path.toString().append(obj["texture"].toString())));
            // read normal map information
            p.SetNormal(new QImage(local_path.toString().append(obj["normal"].toString())));
            polygons.push_back(p);
        }
    }

    rasterizer = Rasterizer(polygons);
    rendered_image = rasterizer.RenderScene(camera);
    DisplayQImage(rendered_image);
}


Polygon MainWindow::LoadOBJ(const QString &file, const QString &polyName)
{
    Polygon p(polyName);
    QString filepath = file;
    std::vector<tinyobj::shape_t> shapes; std::vector<tinyobj::material_t> materials;
    std::string errors = tinyobj::LoadObj(shapes, materials, filepath.toStdString().c_str());
    std::cout << errors << std::endl;
    if(errors.size() == 0)
    {
        int min_idx = 0;
        //Read the information from the vector of shape_ts
        for(unsigned int i = 0; i < shapes.size(); i++)
        {
            std::vector<vec4> pos, nor, uv;
            std::vector<float> &positions = shapes[i].mesh.positions;
            std::vector<float> &normals = shapes[i].mesh.normals;
            std::vector<float> &uvs = shapes[i].mesh.texcoords;
            for(unsigned int j = 0; j < positions.size()/3; j++)
            {
                pos.push_back(vec4(positions[j*3], positions[j*3+1], positions[j*3+2],1));
            }
            for(unsigned int j = 0; j < normals.size()/3; j++)
            {
                nor.push_back(vec4(normals[j*3], normals[j*3+1], normals[j*3+2],0));
            }
            for(unsigned int j = 0; j < uvs.size()/2; j++)
            {
                uv.push_back(vec4(uvs[j*2], uvs[j*2+1],0,0));
            }
            for(unsigned int j = 0; j < pos.size(); j++)
            {
                p.AddVertex(Vertex(pos[j], vec4(255,255,255,1), nor[j], uv[j]));
            }

            std::vector<unsigned int> indices = shapes[i].mesh.indices;
            for(unsigned int j = 0; j < indices.size(); j += 3)
            {
                Triangle t;
                t.indices[0] = indices[j] + min_idx;
                t.indices[1] = indices[j+1] + min_idx;
                t.indices[2] = indices[j+2] + min_idx;
                p.AddTriangle(t);
            }

            min_idx += pos.size();
        }
    }
    else
    {
        //An error loading the OBJ occurred!
        std::cout << errors << std::endl;
    }
    return p;
}


void MainWindow::on_actionSave_Image_triggered()
{
    QString filename = QFileDialog::getSaveFileName(0, QString("Save Image"), QString("../.."), QString("*.bmp"));
    QString ext = filename.right(4);
    if(QString::compare(ext, QString(".bmp")) != 0)
    {
        filename.append(QString(".bmp"));
    }
    QImageWriter writer(filename);
    writer.setFormat("bmp");
    if(!writer.write(rendered_image))
    {
        qDebug() << writer.errorString();
    }
}

void MainWindow::on_actionEquilateral_Triangle_triggered()
{
    std::vector<vec4> pos;
    pos.push_back(vec4(384,382,0,1));
    pos.push_back(vec4(256,160,0,1));
    pos.push_back(vec4(128,382,0,1));

    std::vector<vec4> col;
    col.push_back(vec4(0,0,255,1));
    col.push_back(vec4(0,255,0,1));
    col.push_back(vec4(255,0,0,1));

    Polygon p(QString("Equilateral"), pos, col);

    p.ClearTriangles();

    Triangle t;
    for(unsigned int i = 0; i < 3; i++)
    {
        t.indices[i] = i;
    }

    p.AddTriangle(t);
    std::vector<Polygon> vec; vec.push_back(p);

    rasterizer = Rasterizer(vec);
    rendered_image = rasterizer.RenderScene(camera);
    DisplayQImage(rendered_image);
}

void MainWindow::keyPressEvent(QKeyEvent *e)
{
    switch(e->key())
    {
    //The key shortcuts for the other menu commands were set in Qt's GUI
    //editor. This one was implemented as a key press event for illustration purposes.
    case Qt::Key_Escape : on_actionQuit_Esc_triggered();
    case Qt::Key_Q:
        camera.x_translate(0.5);
        rendered_image = rasterizer.RenderScene(camera);
        DisplayQImage(rendered_image);
        break;
    case Qt::Key_W:
        camera.x_translate(-0.5);
        rendered_image = rasterizer.RenderScene(camera);
        DisplayQImage(rendered_image);
        break;
    case Qt::Key_E:
        camera.y_translate(0.5);
        rendered_image = rasterizer.RenderScene(camera);
        DisplayQImage(rendered_image);
        break;
    case Qt::Key_R:
        camera.y_translate(-0.5);
        rendered_image = rasterizer.RenderScene(camera);
        DisplayQImage(rendered_image);
        break;
    case Qt::Key_T:
        camera.z_translate(0.5);
        rendered_image = rasterizer.RenderScene(camera);
        DisplayQImage(rendered_image);
        break;
    case Qt::Key_Y:
        camera.z_translate(-0.5);
        rendered_image = rasterizer.RenderScene(camera);
        DisplayQImage(rendered_image);
        break;
    case Qt::Key_A:
        camera.x_rotate(5);
        rendered_image = rasterizer.RenderScene(camera);
        DisplayQImage(rendered_image);
        break;
    case Qt::Key_S:
        camera.x_rotate(-5);
        rendered_image = rasterizer.RenderScene(camera);
        DisplayQImage(rendered_image);
        break;
    case Qt::Key_D:
        camera.y_rotate(5);
        rendered_image = rasterizer.RenderScene(camera);
        DisplayQImage(rendered_image);
        break;
    case Qt::Key_F:
        camera.y_rotate(-5);
        rendered_image = rasterizer.RenderScene(camera);
        DisplayQImage(rendered_image);
        break;
    case Qt::Key_G:
        camera.z_rotate(5);
        rendered_image = rasterizer.RenderScene(camera);
        DisplayQImage(rendered_image);
        break;
    case Qt::Key_H:
        camera.z_rotate(-5);
        rendered_image = rasterizer.RenderScene(camera);
        DisplayQImage(rendered_image);
        break;
    }
}

void MainWindow::on_actionQuit_Esc_triggered()
{
    QApplication::exit();
}
