#include "vec4.h"
#include <iostream>
#include "math.h"
#include <assert.h>

vec4::vec4() : data{0, 0, 0, 0} {}

vec4::vec4(float x, float y, float z, float w) : data{x, y, z, w} {}

vec4::vec4(const vec4 &v2) {
    for (int i = 0; i < 4; i++) {
        data[i] = v2.data[i];
    }
}; // copy constructor


float vec4:: operator[](unsigned int index) const {
    assert(index < 4);
    return data[index];
};

float& vec4:: operator[](unsigned int index) {
    return (&data[0])[index];
};

vec4& vec4::operator=(const vec4 &v2){
    for (int i = 0; i < 4; i++) {
        data[i] = v2.data[i];
    }
    return *this;
};

bool vec4:: operator==(const vec4 &v2) const {
    for (int i = 0; i < 4; i++) {
        if (fabsf(data[i] - v2[i]) > 0.001)
            return false;
    }
    return true;
};

bool vec4:: operator!=(const vec4 &v2) const {
    for (int i = 0; i < 4; i++) {
        if (fabsf(data[i] - v2[i]) > 0.001)
            return true;
    }
    return false;
};

vec4& vec4:: operator+=(const vec4 &v2) {
    for (int i = 0; i < 4; i++) {
        data[i] += v2.data[i];
    }
    return *this;
};

vec4& vec4:: operator-=(const vec4 &v2) {
    for (int i = 0; i < 4; i++) {
        data[i] -= v2.data[i];
    }
    return *this;
};

vec4& vec4:: operator*=(float c) {
    for (int i = 0; i < 4; i++) {
        data[i] *= c;
    }
    return *this;
};

vec4& vec4:: operator/=(float c) {
    float c_inv = 1.0f / c;
    for (int i = 0; i < 4; i++) {
        data[i] *= c_inv;
    }
    return *this;
};

vec4 vec4:: operator+(const vec4 &v2) const {
    return vec4(data[0] + v2.data[0], data[1] + v2.data[1], data[2] + v2.data[2], data[3] + v2.data[3]);
};

vec4 vec4:: operator-(const vec4 &v2) const {
    return vec4(data[0] - v2.data[0], data[1] - v2.data[1], data[2] - v2.data[2], data[3] - v2.data[3]);
};

vec4 vec4:: operator*(float c) const {
    return vec4(data[0] * c, data[1] * c, data[2] * c, data[3] * c);
};

vec4 vec4:: operator/(float c) const {
    float c_inv = 1.0f / c;
    return vec4(data[0] * c_inv, data[1] * c_inv, data[2] * c_inv, data[3] * c_inv);
};

float dot(const vec4 &v1, const vec4 &v2) {
    float result;
    for (int i = 0; i < 4; i++) {
        result += v1[i] * v2[i];
    }
    return result;
};

vec4 cross(const vec4 &v1, const vec4 &v2) {
    return vec4(v1[1]*v2[2] - v1[2]*v2[1], v1[2]*v2[0] - v1[0]*v2[2], v1[0]*v2[1] - v1[1]*v2[0], 1);
};

float length(const vec4 &v) {
    return (float)sqrt(v[0]*v[0] + v[1]*v[1] + v[2]*v[2] + v[3]*v[3]);
};

vec4 operator*(float c, const vec4 &v) {
    return vec4(c * v[0], c * v[1], c * v[2], c * v[3]);
};

vec4 normalize(const vec4& v) {
    float leng = length(v);
    return vec4(v[0]/leng, v[1]/leng, v[2]/leng, v[3]/leng);
};

std::ostream &operator<<(std::ostream &o, const vec4 &v) {
    return o << v[0] << " " << v[1] << " " << v[2] << " " << v[3] << "\n";
};


