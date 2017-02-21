#ifndef PERLIN_H
#define PERLIN_H


class Perlin
{
public:
    int repeat;
    Perlin(int repeat);
    Perlin();
    double perlin(double x, double y, double z);
    double OctavePerlin(double x, double y, double z, int octaves, double persistence);
    int inc(int num);
    double grad(int hash, double x, double y, double z);
    double fade(double t);
    double lerp(double a, double b, double x);

private:
    int p[512];

};

#endif // PERLIN_H
