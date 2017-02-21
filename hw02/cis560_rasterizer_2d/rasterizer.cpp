#include "rasterizer.h"
#include "math.h"
using namespace std;

Rasterizer::Rasterizer(const std::vector<Polygon> &polygons)
    : polygons(polygons)
{}

bool compare(Polygon p1, Polygon p2) {
    return p1.VertAt(0).pos[2] < p2.VertAt(0).pos[2];
}

QImage Rasterizer::RenderScene(Camera camera)
{
    //TODO: Complete the various components of code that make up this function
    //It should return the rasterized image of the current scene

    //Notice that even though it should return a QImage this function compiles.
    //Remember, C++'s compiler is dumb (though it will at least warn you that
    //the function doesn't return anything).
    //BEWARE! If you use a function that is missing its return statement,
    //it will return garbage memory!

    // rendering prepare
    QImage image(512, 512, QImage::Format_RGB32);
    QRgb value = qRgb(0, 0, 0);
    for (int i = 0; i < 512; i++)
        for (int j = 0; j < 512; j++)
            image.setPixel(i, j, value);

    mat4 transform_matrix = camera.ProjectionMatrix() * camera.ViewMatrix();

  // sort function for 2d image rasterizer
    sort(this->polygons.begin(), this->polygons.end(), compare);

    // code segment for z-buffering
    float depth[512][512];
    for (int i = 0; i < 512; i++)
        for (int j = 0; j < 512; j++)
            depth[i][j] = INT_MIN;

    for (int i = 0; i < (int)this->polygons.size(); i++) {
        for (int j = 0; j < this->polygons[i].Triangle_number(); j++ ){
            // original vertex
            Vertex v_1 = this->polygons[i].VertAt(this->polygons[i].TriAt(j).indices[0]);
            Vertex v_2 = this->polygons[i].VertAt(this->polygons[i].TriAt(j).indices[1]);
            Vertex v_3 = this->polygons[i].VertAt(this->polygons[i].TriAt(j).indices[2]);

            vec4 v1_pos = transform_matrix * v_1.pos;
            vec4 v2_pos = transform_matrix * v_2.pos;
            vec4 v3_pos = transform_matrix * v_3.pos;
            \
            v1_pos /= v1_pos[3];
            v2_pos /= v2_pos[3];
            v3_pos /= v3_pos[3];

            vec4 v1_pixel = vec4((v1_pos[0] + 1) * 512 / 2, (1 - v1_pos[1]) * 512 / 2, v1_pos[3], 1);
            vec4 v2_pixel = vec4((v2_pos[0] + 1) * 512 / 2, (1 - v2_pos[1]) * 512 / 2, v1_pos[3], 1);
            vec4 v3_pixel = vec4((v3_pos[0] + 1) * 512 / 2, (1 - v3_pos[1]) * 512 / 2, v1_pos[3], 1);

            // vertex is pixel space, value between 0-512, rasterizer using v1, v2, v3
            Vertex v1 = Vertex(v1_pixel, v_1.color, v_1.normal, v_1.uv);
            Vertex v2 = Vertex(v2_pixel, v_2.color, v_2.normal, v_2.uv);
            Vertex v3 = Vertex(v3_pixel, v_3.color, v_3.normal, v_3.uv);

            // bounding box
            int y_min = min(v1.pos[1], min(v2.pos[1], v3.pos[1]));
            int y_max = max(v1.pos[1], max(v2.pos[1], v3.pos[1]));

            float slope_1, slope_2, slope_3;
            // computer slopes
            if ((v3.pos[0]-v2.pos[0]) != 0)
                slope_1 = (v3.pos[1]-v2.pos[1]) / (v3.pos[0]-v2.pos[0]); // line v2v3
            if ((v2.pos[0]-v1.pos[0]) != 0)
                slope_2 = (v2.pos[1]-v1.pos[1]) / (v2.pos[0]-v1.pos[0]); // line v1v2
            if ((v3.pos[0]-v1.pos[0]) != 0)
                slope_3 = (v3.pos[1]-v1.pos[1]) / (v3.pos[0]-v1.pos[0]); // line v1v3

            // computer intersect points
            std::vector<vec4> intersect_pnt_left, intersect_pnt_right;

            for (int k = y_min; k < y_max+1; k++) {
                int x_left = INT_MAX;
                int x_right = INT_MIN;
                int x_23, x_12, x_13;

                // line 23
                if ((v3.pos[0]-v2.pos[0]) == 0 && (v3.pos[1] - k) * (v2.pos[1] - k) <= 0) {
                    x_23 = v3.pos[0];
                    x_left = min(x_left, x_23);
                    x_right = max(x_right, x_23);
                }
                else if ((v3.pos[1] - k) * (v2.pos[1] - k) > 0);
                else if (slope_1 == 0 && v2.pos[1] != k);
                else if (slope_1 == 0 && v2.pos[1] == k) {
                    x_left = min(v3.pos[0], v2.pos[0]);
                    x_right = max(v3.pos[0], v2.pos[0]);
                }
                else {
                    x_23 = v2.pos[0] - (v2.pos[1]-k) / slope_1;
                    x_left = min(x_left, x_23);
                    x_right = max(x_right, x_23);
                }

                // line 12
                if (((v1.pos[0]-v2.pos[0]) == 0) && (v1.pos[1] - k) * (v2.pos[1] - k) <= 0) {
                    x_12 = v1.pos[0];
                    x_left = min(x_left, x_12);
                    x_right = max(x_right, x_12);
                }
                else if ((v1.pos[1] - k) * (v2.pos[1] - k) > 0);
                else if (slope_2 == 0 && v1.pos[1] != k);
                else if (slope_2 == 0 && v1.pos[1] == k) {
                    x_left = min(v1.pos[0], v2.pos[0]);
                    x_right = max(v1.pos[0], v2.pos[0]);
                }
                else {
                    x_12 = v1.pos[0] - (v1.pos[1]-k) / slope_2;
                    x_left = min(x_left, x_12);
                    x_right = max(x_right, x_12);
                }

                // line 13
                if (((v1.pos[0]-v3.pos[0]) == 0) && (v1.pos[1] - k) * (v3.pos[1] - k) <= 0) {
                    x_13 = v1.pos[0];
                    x_left = min(x_left, x_13);
                    x_right = max(x_right, x_13);
                }
                else if ((v1.pos[1] - k) * (v3.pos[1] - k) > 0);
                else if (slope_3 == 0 && v1.pos[1] != k);
                else if (slope_3 == 0 && v1.pos[1] == k) {
                    x_left = min(v1.pos[0], v3.pos[0]);
                    x_right = max(v1.pos[0], v3.pos[0]);
                }
                else {
                    x_13 = v1.pos[0] - (v1.pos[1]-k) / slope_3;
                    x_left = min(x_left, x_13);
                    x_right = max(x_right, x_13);
                }
                intersect_pnt_left.push_back((vec4(x_left, k, 0, 1)));
                intersect_pnt_right.push_back((vec4(x_right, k, 0, 1)));

                vec4 color_vec;
                vec4 uv_vec;
                vec4 normal_vec;

                // 2d - color, 3d - uv
                for(int x = x_left; x < x_right+1; x++) {
                    // correct z value
                    if (x >= 0 && x < 512 && k >= 0 && k < 512) {
                        vec4 z_correct_vec = Barycentric_Interpolation(v1, v2, v3, vec4(x, k, 0, 0), 2);
                        // correct for UV
                        uv_vec = Barycentric_Interpolation(v1, v2, v3, vec4(x, k, z_correct_vec[2], 0), 3);
                        normal_vec = Barycentric_Interpolation(v1, v2, v3, vec4(x, k, z_correct_vec[2], 0), 4);

                        if (z_correct_vec[2] > depth[x][k]) {
                            // z buffering
                            depth[x][k] = z_correct_vec[2];

                            // Lambertian Reflection
//                            vec4 E = (LightColor * max(dot(normal_vec, L), 0.0)) / (4 * M_PI * (camera.eye[2] - depth[x][k]) * (camera.eye[2] - depth[x][k]));
//                            vec4 LambertianLight = AmbientColor + E;

                            // for texture mapping
                            QImage *texture = this->polygons[i].getImage();
                            QRgb value = texture->pixel(QPoint(uv_vec[0] * texture->width(), (1-uv_vec[1]) * texture->height()));

                            // for normal mapping
//                            QImage *normal = this->polygons[i].getNormal();
//                            int R_value = normal_vec[0] * 128 + 128;
//                            int G_value = normal_vec[1] * 128 + 128;
//                            int B_value = normal_vec[2] * 128 + 128;
//                            QRgb value = QRgb(R_value, G_value, B_value);

                            image.setPixel(x, k, value);
                        }
                    }
                }
            }
        }
    }

    return image;
}

void Rasterizer::ClearScene()
{
    polygons.clear();
}

// requirement: 1 stands for color mapping, 2 stands for Perspective-correct interpolation for Z.
// 3 stands for Perspective-correct uv mapping, 4 stands for normal mapping.
vec4 Rasterizer::Barycentric_Interpolation(Vertex v1, Vertex v2, Vertex v3, vec4 point, int requirement) {
    float area_1 = length(cross(vec4(v2.pos[0]-point[0], v2.pos[1]-point[1], 0, 0), vec4(v3.pos[0]-point[0], v3.pos[1]-point[1], 0, 0))) / 2;
    float area_2 = length(cross(vec4(v1.pos[0]-point[0], v1.pos[1]-point[1], 0, 0), vec4(v3.pos[0]-point[0], v3.pos[1]-point[1], 0, 0))) / 2;
    float area_3 = length(cross(vec4(v1.pos[0]-point[0], v1.pos[1]-point[1], 0, 0), vec4(v2.pos[0]-point[0], v2.pos[1]-point[1], 0, 0))) / 2;
    float area = area_1 + area_2 + area_3;

    float s1 = area_1 / area;
    float s2 = area_2 / area;
    float s3 = area_3 / area;

    // color mapping - 2d space
    if (requirement == 1) {
        int r_value = v1.color[0] * s1 + v2.color[0] * s2 + v3.color[0] * s3;
        int g_value = v1.color[1] * s1 + v2.color[1] * s2 + v3.color[1] * s3;
        int b_value = v1.color[2] * s1 + v2.color[2] * s2 + v3.color[2] * s3;
        return vec4(r_value, g_value, b_value, 0);
    }
    // Perspective-correct interpolation for Z - 2d space
    else if (requirement == 2){
        float z = 1.0 / (s1 / v1.pos[2] + s2 / v2.pos[2] + s3 / v3.pos[2]);
        return vec4(point[0], point[1], z, point[3]);
    }
    // Perspective-correct uv mapping, using interpolated Z.
    else if (requirement == 3){
        vec4 uv = point[2] * (v1.uv * s1 / v1.pos[2] + v2.uv * s2 / v2.pos[2] + v3.uv * s3 / v3.pos[2]);
        return uv;
    }
    // Perspective-correct normal mapping.
    else if (requirement == 4) {
        vec4 normal = point[2] * (v1.normal * s1 / v1.pos[2] + v2.normal * s2 / v2.pos[2] + v3.normal * s3 / v3.pos[2]);
        return normal;
    }
    else
        return vec4(0, 0, 0, 0);
}

mat4 Camera::ViewMatrix() {
    mat4 O_matrix, T_matrix;

    vec4 o_1 = {right[0], up[0], forward[0], 0};
    vec4 o_2 = {right[1], up[1], forward[1], 0};
    vec4 o_3 = {right[2], up[2], forward[2], 0};
    vec4 o_4 = {0, 0, 0, 1};
    O_matrix = {o_1, o_2, o_3, o_4};

    vec4 m_1 = {1, 0, 0, 0};
    vec4 m_2 = {0, 1, 0, 0};
    vec4 m_3 = {0, 0, 1, 0};
    vec4 m_4 = {-1*eye[0], -1*eye[1], -1*eye[2], 1};
    T_matrix = {m_1, m_2, m_3, m_4};

    mat4 OriginalMatrix = O_matrix * T_matrix;

    return OriginalMatrix;
}

mat4 Camera::ProjectionMatrix() {
    vec4 p_1 = {1 / (aspect_ratio * tan_angle), 0, 0, 0};
    vec4 p_2 = {0, 1 / tan_angle, 0, 0};
    vec4 p_3 = {0, 0, far_plane / (far_plane - near_plane), 1};
    vec4 p_4 = {0, 0, (float)-1.0 * far_plane * near_plane / (far_plane - near_plane), 0};

    return {p_1, p_2, p_3, p_4};
}

void Camera::x_translate(float displacement) {
    eye[0] += displacement;
}

void Camera::y_translate(float displacement) {
    eye[1] += displacement;
}

void Camera::z_translate(float displacement) {
    eye[2] += displacement;
}

void Camera::x_rotate(int angle) {
    mat4 rotate_x = mat4::rotate(angle, 1, 0, 0);
    up = rotate_x * up;
    forward = rotate_x * forward;
}

void Camera::y_rotate(int angle) {
    mat4 rotate_y = mat4::rotate(angle, 0, 1, 0);
    forward = rotate_y * forward;
    right = rotate_y * right;
}

void Camera::z_rotate(int angle) {
    mat4 rotate_z = mat4::rotate(angle, 0, 0, 1);
    up = rotate_z * up;
    right = rotate_z * right;
}

