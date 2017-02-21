#pragma once
#include <vec4.h>
#include <mat4.h>
#include <vector>
#include "math.h"
#include <QString>
#include <QImage>

struct Vertex
{
    vec4 pos;    // The position of the vertex. In hw02, this is in pixel space.
    vec4 color;  // The color of the vertex. X corresponds to Red, Y corresponds to Green, and Z corresponds to Blue.
    vec4 normal; // The surface normal of the vertex (not yet used)
    vec4 uv;     // The texture coordinates of the vertex (not yet used)

    Vertex(vec4 p, vec4 c, vec4 n, vec4 u)
        : pos(p), color(c), normal(n), uv(u)
    {}
};

struct Triangle
{
    //The indices of the Vertices that make up this triangle
    //The indices correspond to the std::vector of Vertices stored in the Polygon
    //which stores this Triangle
    unsigned int indices[3];
};

class Line
{
private:
    vec4 endpoint_1;
    vec4 endpoint_2;
    int dy;
    int dx;
public:
    int intersect_pnt;
    int intersect_pnt_left = INT_MAX;
    int intersect_pnt_right = INT_MIN;
    Line(vec4 x_1, vec4 x_2);
    bool intersection(int y); // whether there exsits two intersection
    bool horizontal(int y); // whether there're 2 intersections
};

class Polygon
{
private:
    //TODO: Populate this list of triangles in Triangulate()
    std::vector<Triangle> tris;
    //The list of Vertices that define this polygon. This is already filled by the Polygon constructor.
    std::vector<Vertex> verts;
    //The name of this polygon, primarily to help you debug
    QString name;
    //The image that can be read to determine pixel color when used in conjunction with UV coordinates
    QImage* texture;
    QImage* normal;

public:
    Polygon(const QString &name, const std::vector<vec4> &pos, const std::vector<vec4> &col);
    Polygon(const QString &name, int sides, vec4 color, vec4 pos, float rot, vec4 scale);
    Polygon(const QString &name);
    Polygon(const Polygon& p);
    Polygon();

    ~Polygon();

    //TODO: Complete the body of Triangulate() in polygon.cpp
    void Triangulate();

    //Copies the input QImage into this Polygon's texture
    void SetTexture(QImage*);
    //Copies the input QImage into this Polygon's normal map
    void SetNormal(QImage*);

    //Various getter, setter, and adder functions
    void AddVertex(const Vertex&);
    void AddTriangle(const Triangle&);
    void ClearTriangles();

    Triangle& TriAt(unsigned int);
    Triangle TriAt(unsigned int) const;

    Vertex& VertAt(unsigned int);
    Vertex VertAt(unsigned int) const;

    int Triangle_number();
    int vertex_number();
    QString getName();
    QImage* getImage();
    QImage* getNormal();
};
