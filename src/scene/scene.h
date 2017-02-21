
#pragma once
#include <QList>
#include <camera.h>
#include <perlin.h>
#include <tuple>
#include <iostream>
#include <math.h>
#include <river.h>
#include "npc.h"
enum BlockType {Default = 0, GrassLand = 1, Dirt = 2, Water = 3, Stone = 4, Wood = 5, Sand = 6,Lava =7,
               Leaf=8,BedRock=9,Coal=10,IronOre=11,Flower=12,Grass=13, Snow=14, SnowSpike=15,Cactus=16,
               Fire=17,WoodOnFire=18};


class Block
{
public:
    std::tuple<int,int,int> pos;

    Block();
    ~Block(){}

    bool isActive();
    void SetActive(bool active);
    int lifeTime;               //added by Yihuang

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
    River river1;
    River river2;
    std::map<std::tuple<int,int>,int> riversideHeight;
    int lowerBoundX, higherBoundX, lowerBoundY, higherBoundY, lowerBoundZ, higherBoundZ;

    void UpdateRiver();
    void MakeWater(int x,int y,int z);
    void CalculateRiverside();
    void CalculateRiversideHelper(int x, int y, int z);
    bool IfWater(int x, int z);


};
