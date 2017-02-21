#include "mat4.h"
#include "vec4.h"
#include "math.h"
#include <assert.h>
#include <iomanip>
#include <iostream>

mat4:: mat4() : data{vec4{1.f, 0.f, 0.f, 0.f}, vec4{0.f, 1.f, 0.f, 0.f}, vec4{0.f, 0.f, 1.f, 0.f}, vec4{0.f, 0.f, 0.f, 1.f}}{}

mat4:: mat4(float diag) : data{vec4{diag, 0.f, 0.f, 0.f}, vec4{0.f, diag, 0.f, 0.f}, vec4{0.f, 0.f, diag, 0.f}, vec4{0.f, 0.f, 0.f, diag}}{}

mat4:: mat4(const vec4 &col0, const vec4 &col1, const vec4 &col2, const vec4& col3) : data{col0, col1, col2, col3}{}

// copy constructor
mat4::mat4(const mat4 &m2) {
    data[0] = m2.data[0];
    data[1] = m2.data[1];
    data[2] = m2.data[2];
    data[3] = m2.data[3];
};

/// Returns the values of the column at the index
vec4 mat4:: operator[](unsigned int index) const {
    assert(index < 4);
    return data[index];
};

/// Returns a reference to the column at the index
vec4& mat4:: operator[](unsigned int index) {
    return (&data[0])[index];
};

// it returns the rotation matrix itself, NOT the result after rotation
mat4 mat4:: rotate(float angle, float x, float y, float z) {
//    float angle_radian = angle * 3.141592653589793 / 180.0;
    float angle_radian = angle * M_PI * 1.0 / 180.0;
    float cos_angle = cos(angle_radian), sin_angle = sin(angle_radian);
    if (fabs(cos_angle)< 0.0000001)
        cos_angle = 0;
    if (fabs(cos_angle) > 0.9999999)
        cos_angle = 1;
    if (fabs(sin_angle) < 0.0000001)
        sin_angle = 0;
    if (fabs(sin_angle) > 0.9999999)
        sin_angle = 1;

    vec4 col1, col2, col3, col4;
    col1 = vec4{cos_angle+x*x*(1-cos_angle), y*x*(1-cos_angle)+z*sin_angle, z*x*(1-cos_angle)-y*sin_angle, 0};
    col2 = vec4{x*y*(1-cos_angle)-z*sin_angle, cos_angle+y*y*(1-cos_angle), z*y*(1-cos_angle)+x*sin_angle, 0};
    col3 = vec4{x*z*(1-cos_angle)+y*sin_angle, y*z*(1-cos_angle) - x*sin_angle, cos_angle+z*z*(1-cos_angle), 0};
    col4 = {0.f, 0.f, 0.f, 1.f};

    return mat4(col1, col2, col3, col4);
};

mat4 mat4:: translate(float x, float y, float z) {
    vec4 col1 = vec4{1.f, 0.f, 0.f, 0.f};
    vec4 col2 = vec4{0.f, 1.f, 0.f, 0.f};
    vec4 col3 = vec4{0.f, 0.f, 1.f, 0.f};
    vec4 col4 = vec4{x, y, z, 1.f};
    
    return mat4(col1, col2, col3, col4);
};

mat4 mat4:: scale(float x, float y, float z) {
    vec4 col1 = vec4{x, 0.f, 0.f, 0.f};
    vec4 col2 = vec4{0.f, y, 0.f, 0.f};
    vec4 col3 = vec4{0.f, 0.f, z, 0.f};
    vec4 col4 = vec4{0.f, 0.f, 0.f, 1.f};
    
    return mat4(col1, col2, col3, col4);
};

mat4 mat4:: identity() {
    vec4 col1 = vec4{1.f, 0.f, 0.f, 0.f};
    vec4 col2 = vec4{0.f, 1.f, 0.f, 0.f};
    vec4 col3 = vec4{0.f, 0.f, 1.f, 0.f};
    vec4 col4 = vec4{0.f, 0.f, 0.f, 1.f};
    
    return mat4(col1, col2, col3, col4);
};

mat4& mat4:: operator=(const mat4 &m2) {
    for (int i = 0; i < 4; i++) {
        data[i] = m2.data[i];
    }
    return *this;
};

bool mat4:: operator==(const mat4 &m2) const {
    for (int i = 0; i < 4; i++) {
        if (data[i] != m2[i])
            return false;
    }
    return true;
};

bool mat4:: operator!=(const mat4 &m2) const {
    for (int i = 0; i < 4; i++) {
        if (data[i] != m2[i])
            return true;
    }
    return false;
}

mat4& mat4:: operator+=(const mat4 &m2) {
    for (int i = 0; i < 4; i++) {
        data[i] += m2.data[i];
    }
    return  *this;
};

mat4& mat4:: operator-=(const mat4 &m2) {
    for (int i = 0; i < 4; i++) {
        data[i] -= m2.data[i];
    }
    return  *this;
};

mat4& mat4:: operator*=(float c) {
    for (int i = 0; i < 4; i++) {
        data[i] *= c;
    }
    return  *this;
};

mat4& mat4:: operator/=(float c) {
    float c_inv = 1.0f / c;
    for (int i = 0; i < 4; i++) {
        data[i] *= c_inv;
    }
    return  *this;
};


mat4 mat4:: operator+(const mat4 &m2) const {
    return mat4(data[0] + m2.data[0], data[1] + m2.data[1], data[2] + m2.data[2], data[3] + m2.data[3]);
};


mat4 mat4:: operator-(const mat4 &m2) const {
    return mat4(data[0] - m2.data[0], data[1] - m2.data[1], data[2] - m2.data[2], data[3] - m2.data[3]);
};

mat4 mat4:: operator*(float c) const {
    return mat4(data[0] * c, data[1] * c, data[2] * c, data[3] * c);
};

mat4  mat4:: operator/(float c) const {
    float c_inv = 1.0f / c;
    return mat4(data[0] * c_inv, data[1] * c_inv, data[2]* c_inv, data[3] * c_inv);
};

mat4 mat4:: operator*(const mat4 &m2) const {
    float res1, res2, res3, res4;
    
    res1 = data[0][0]*m2.data[0][0] + data[1][0]*m2.data[0][1] + data[2][0]*m2.data[0][2] + data[3][0]*m2.data[0][3];
    res2 = data[0][1]*m2.data[0][0] + data[1][1]*m2.data[0][1] + data[2][1]*m2.data[0][2] + data[3][1]*m2.data[0][3];
    res3 = data[0][2]*m2.data[0][0] + data[1][2]*m2.data[0][1] + data[2][2]*m2.data[0][2] + data[3][2]*m2.data[0][3];
    res4 = data[0][3]*m2.data[0][0] + data[1][3]*m2.data[0][1] + data[2][3]*m2.data[0][2] + data[3][3]*m2.data[0][3];

    vec4 col1 = {res1, res2, res3, res4};
    
    res1 = data[0][0]*m2.data[1][0] + data[1][0]*m2.data[1][1] + data[2][0]*m2.data[1][2] + data[3][0]*m2.data[1][3];
    res2 = data[0][1]*m2.data[1][0] + data[1][1]*m2.data[1][1] + data[2][1]*m2.data[1][2] + data[3][1]*m2.data[1][3];
    res3 = data[0][2]*m2.data[1][0] + data[1][2]*m2.data[1][1] + data[2][2]*m2.data[1][2] + data[3][2]*m2.data[1][3];
    res4 = data[0][3]*m2.data[1][0] + data[1][3]*m2.data[1][1] + data[2][3]*m2.data[1][2] + data[3][3]*m2.data[1][3];
    
    vec4 col2 = {res1, res2, res3, res4};
    
    res1 = data[0][0]*m2.data[2][0] + data[1][0]*m2.data[2][1] + data[2][0]*m2.data[2][2] + data[3][0]*m2.data[2][3];
    res2 = data[0][1]*m2.data[2][0] + data[1][1]*m2.data[2][1] + data[2][1]*m2.data[2][2] + data[3][1]*m2.data[2][3];
    res3 = data[0][2]*m2.data[2][0] + data[1][2]*m2.data[2][1] + data[2][2]*m2.data[2][2] + data[3][2]*m2.data[2][3];
    res4 = data[0][3]*m2.data[2][0] + data[1][3]*m2.data[2][1] + data[2][3]*m2.data[2][2] + data[3][3]*m2.data[2][3];
    
    vec4 col3 = {res1, res2, res3, res4};
    
    res1 = data[0][0]*m2.data[3][0] + data[1][0]*m2.data[3][1] + data[2][0]*m2.data[3][2] + data[3][0]*m2.data[3][3];
    res2 = data[0][1]*m2.data[3][0] + data[1][1]*m2.data[3][1] + data[2][1]*m2.data[3][2] + data[3][1]*m2.data[3][3];
    res3 = data[0][2]*m2.data[3][0] + data[1][2]*m2.data[3][1] + data[2][2]*m2.data[3][2] + data[3][2]*m2.data[3][3];
    res4 = data[0][3]*m2.data[3][0] + data[1][3]*m2.data[3][1] + data[2][3]*m2.data[3][2] + data[3][3]*m2.data[3][3];
    
    vec4 col4 = {res1, res2, res3, res4};
    
    return mat4{col1, col2, col3, col4};
};

/// Matrix/vector multiplication (m * v)
/// Assume v is a column vector (ie. a 4x1 matrix)
vec4 mat4:: operator*(const vec4 &v) const {
    float res1, res2, res3, res4;
    
    res1 = data[0][0]*v[0] + data[1][0]*v[1] + data[2][0]*v[2] + data[3][0]*v[3];
    res2 = data[0][1]*v[0] + data[1][1]*v[1] + data[2][1]*v[2] + data[3][1]*v[3];
    res3 = data[0][2]*v[0] + data[1][2]*v[1] + data[2][2]*v[2] + data[3][2]*v[3];
    res4 = data[0][3]*v[0] + data[1][3]*v[1] + data[2][3]*v[2] + data[3][3]*v[3];
    
    return vec4(res1, res2, res3, res4);
};

mat4 transpose(const mat4 &m) {
    vec4 col1 = vec4(m[0][0], m[1][0], m[2][0], m[3][0]);
    vec4 col2 = vec4(m[0][1], m[1][1], m[2][1], m[3][1]);
    vec4 col3 = vec4(m[0][2], m[1][2], m[2][2], m[3][2]);
    vec4 col4 = vec4(m[0][3], m[1][3], m[2][3], m[3][3]);
    
    return mat4(col1, col2, col3, col4);
};

vec4 row(const mat4 &m, unsigned int index) {
    return vec4(m[0][index], m[1][index], m[2][index], m[3][index]);
};

mat4 operator*(float c, const mat4 &m) {
    return mat4(m[0]*c, m[1]*c, m[2]*c, m[3]*c);
};

vec4 operator*(const vec4 &v, const mat4 &m) {
    float res0, res1, res2, res3;
    
    res0 = v[0]*m[0][0] + v[1]*m[0][1] + v[2]*m[0][2] + v[3]*m[0][3];
    res1 = v[0]*m[1][0] + v[1]*m[1][1] + v[2]*m[1][2] + v[3]*m[1][3];
    res2 = v[0]*m[2][0] + v[1]*m[2][1] + v[2]*m[2][2] + v[3]*m[2][3];
    res3 = v[0]*m[3][0] + v[1]*m[3][1] + v[2]*m[3][2] + v[3]*m[3][3];
    
    return vec4(res0, res1, res2, res3);
};

std::ostream &operator<<(std::ostream &o, const mat4 &m) {
    return o << std::left << std::setw(15) << m[0][0] << std::setw(15) << m[1][0] << std::setw(15) << m[2][0] << std::setw(15) << m[3][0] << "\n" << std::setw(15) << m[0][1] << std::setw(15) << m[1][1] << std::setw(15) << m[2][1] << std::setw(15) << m[3][1] << "\n" << std::setw(15) << m[0][2] << std::setw(15) << m[1][2] << std::setw(15) << m[2][2] << std::setw(15) << m[3][2] << "\n" << std::setw(15) << m[0][3] << std::setw(15) << m[1][3] << std::setw(15) << m[2][3] << std::setw(15) << m[3][3] << "\n";
};
