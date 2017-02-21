#include "polygon.h"

Line::Line(vec4 x_1, vec4 x_2) : endpoint_1{x_1}, endpoint_2{x_2}, dy{(int)(x_1[1] - x_2[1])}, dx{(int)(x_1[0] - x_2[0])} {}

// whether line segment intersects with screen row
bool Line::intersection(int y) {
    if ((endpoint_1[1]-y) * (endpoint_2[1]-y) > 0)
        return false;
    else if (dx == 0 && (endpoint_1[1]-y) * (endpoint_2[1]-y) <= 0) {
        intersect_pnt = endpoint_1[0];
        return true;
    }
    else if (dy == 0 && endpoint_1[1] != y)
        return false; // not intersect
    else if (dy == 0 && endpoint_1[1] == y) {
        intersect_pnt_left = fmin(endpoint_1[0], endpoint_2[0]);
        intersect_pnt_right = fmax(endpoint_1[0], endpoint_2[0]);
        return true; // but has two intersection points
    }
    else {
        float slope = dy / dx;
        intersect_pnt = endpoint_1[0] - (endpoint_1[1] - y) / slope;
        return true;
    }
}

// two intersection points
bool Line::horizontal(int y) {
    if (dy == 0 && endpoint_1[1] == y) {
        return true;
    }
    return false;
}

void Polygon::Triangulate()
{
    //TODO: Populate list of triangles
    // Use Fan method here for convex polygons
    for (int i = 1; i< (int)verts.size() - 1; i++) {
        Triangle t;
        t.indices[0] = 0;
        t.indices[1] = i;
        t.indices[2] = i+1;
        tris.push_back(t);
    }
}

Polygon::Polygon()
    : tris(), verts(), name("Polygon"), texture(nullptr)
{}

Polygon::Polygon(const Polygon& p)
    : tris(p.tris), verts(p.verts), name(p.name), texture(new QImage(*p.texture))
{}

//Creates a polygon from the input list of vertex positions and colors
Polygon::Polygon(const QString &name, const std::vector<vec4> &pos, const std::vector<vec4> &col)
    : tris(), name(name), texture(nullptr)
{
    for(unsigned int i = 0; i < pos.size(); i++)
    {
        verts.push_back(Vertex(pos[i], col[i], vec4(), vec4()));
    }
    Triangulate();
}

//Creates a regular polygon with a number of sides indicated by the "sides" input integer.
//All of its vertices are of color "color", and the polygon is centered at "pos".
//It is rotated about its center by "rot" degrees, and is scaled from its center by "scale" units
Polygon::Polygon(const QString &name, int sides, vec4 color, vec4 pos, float rot, vec4 scale)
    :tris(), name(name), texture(nullptr)
{
    vec4 v(0,1,0,1);
    float angle = 360.f / sides;
    for(int i = 0; i < sides; i++)
    {
        mat4 translate_matrix = mat4::translate(pos[0], pos[1], pos[2]);
        mat4 rotation_matrix_1 = mat4::rotate(rot, 0, 0, 1);
        mat4 scale_matrix = mat4::scale(scale[0], scale[1], scale[2]);
        mat4 rotation_matrix_2 = mat4::rotate(i * angle, 0,0,1);
        mat4 tmp1 = translate_matrix * rotation_matrix_1;
        mat4 tmp2 = tmp1 * scale_matrix;
        mat4 tmp3 = tmp2 * rotation_matrix_2;
        vec4 tmp4 = tmp3 * v;
        verts.push_back(Vertex(tmp4, color, vec4(), vec4()));
    }
    Triangulate();
}

Polygon::Polygon(const QString &name)
    : tris(), name(name), texture(nullptr)
{}

Polygon::~Polygon()
{
    delete texture;
}

void Polygon::SetTexture(QImage *i)
{
    texture = i;
}

void Polygon::SetNormal(QImage *i)
{
    normal = i;
}

void Polygon::AddTriangle(const Triangle &t)
{
    tris.push_back(t);
}

void Polygon::AddVertex(const Vertex &v)
{
    verts.push_back(v);
}

void Polygon::ClearTriangles()
{
    tris.clear();
}

Triangle& Polygon::TriAt(unsigned int i)
{
    return tris[i];
}

Triangle Polygon::TriAt(unsigned int i) const
{
    return tris[i];
}

Vertex &Polygon::VertAt(unsigned int i)
{
    return verts[i];
}

Vertex Polygon::VertAt(unsigned int i) const
{
    return verts[i];
}

int Polygon::Triangle_number() {
    return tris.size();
}

int Polygon::vertex_number() {
    return verts.size();
}

QString Polygon::getName() {
    return name;
}

QImage* Polygon::getImage() {
    return texture;
}

QImage* Polygon::getNormal() {
    return normal;
}

