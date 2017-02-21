#ifndef RIVER_H
#define RIVER_H
#include <la.h>
#include <tuple>
#include <iostream>

enum OperationType{Fwd = 0,Brh =1,RtnHere =2,Tn=3,RtnEnd=4};
class River
{
public:
    River();
    River(int x,int z,int riverType);
    std::map<std::tuple<int,int,int>, bool> containRiver;
    void RiverGenerator1(std::vector<OperationType> rList,glm::vec2 pos,int &times);
    void MakeRiver(glm::vec2);
    void Line(glm::vec2 v1,glm::vec2 v2,int t);
    void Line2(glm::vec2 v1,glm::vec2 v2,int t);
    void LsystemGenerator1();
    void LsystemGenerator2();
    void RiverGenerator2(std::vector<OperationType> rList,glm::vec2 pos,int &times);
    void GetRiverBound(int &Xmin,int&Xmax,int &Zmin,int &Zmax);
private:
    int startPosX;
    int startPosZ;
    int RiverType;   //river type: 1 or 2
    int width;
    int minX;
    int maxX;
    int minZ;
    int maxZ;
};

#endif // RIVER_H
