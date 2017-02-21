#pragma once
#include <polygon.h>
#include <QImage>
#include "math.h"

class Camera
{
private:
    vec4 forward = vec4(0, 0, -1, 0);
    vec4 right = vec4(1, 0, 0, 0);
    vec4 up = vec4(0, 1, 0, 0);
    int field_view = 45;
    vec4 eye = vec4(0, 0, 10, 1);
    float near_plane = 0.01;
    float far_plane = 100.0;
    float aspect_ratio = 1.0;
    float tan_angle = tan(field_view * M_PI * 1.0 / (180.0 * 2.0));
public:
    mat4 ViewMatrix();
    mat4 ProjectionMatrix();
    void x_translate(float displacement);
    void y_translate(float displacement);
    void z_translate(float displacement);
    void x_rotate(int angle);
    void y_rotate(int angle);
    void z_rotate(int angle);
};


class Rasterizer
{
private:
    //This is the set of Polygons loaded from a JSON scene file
    std::vector<Polygon> polygons;
public:
    Rasterizer(const std::vector<Polygon> &polygons);
    QImage RenderScene(Camera camera);
    void ClearScene();
    vec4 Barycentric_Interpolation(Vertex v1, Vertex v2, Vertex v3, vec4 point, int requirement);
//    vec4 Barycentric_Interpolation_uv(Vertex v1, Vertex v2, Vertex v3, vec4 point);
};

