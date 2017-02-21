#pragma once
#include <QList>
#include <camera.h>
#include <perlin.h>
#include <tuple>
#include <iostream>

enum BlockType {Default = 0, Grass = 1, Dirt = 2, Water = 3, Stone = 4, Wood = 5, Sand = 6};


class Block
{
public:
    std::tuple<int,int,int> pos;

    Block();
    ~Block(){}

    bool isActive();
    void SetActive(bool active);

private:
    bool m_active;
    BlockType blockType;
};

class Scene
{
public:
    Scene();
//    QList<QList<QList<bool>>> objects;//A 3D list of the geometry in the world. Currently, it just lists whether or not a given cell contains a block.
    void CreateTestScene();

    glm::ivec3 dimensions;

    std::tuple<int,int,int> pos;
    std::map<std::tuple<int,int,int>, BlockType> scene;
    std::map<std::tuple<int,int,int>, bool> containBlock;

    int lowerBoundX, higherBoundX, lowerBoundY, higherBoundY, lowerBoundZ, higherBoundZ;
};
