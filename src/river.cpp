#include "river.h"
#include <math.h>

#define M_PI 3.14159265358979323846
River::River()
{
}
River::River(int x, int z, int riverType):
    startPosX(x),startPosZ(z),RiverType(riverType),width(15),
    minX(0),maxX(0),minZ(0),maxZ(0)
{
    if(RiverType==2)
        LsystemGenerator2();
    else
        LsystemGenerator1();
}
void River::LsystemGenerator1()
{
    glm::vec2 test(startPosX,startPosZ);
    int t=0;
    std::vector<OperationType> rList;
    rList.push_back(Fwd);
    rList.push_back(Fwd);
    rList.push_back(Tn);
    rList.push_back(Fwd);
    rList.push_back(Tn);
    //rList.push_back(Fwd);
    rList.push_back(RtnHere);

    rList.push_back(Tn);
    rList.push_back(Tn);
    rList.push_back(Fwd);
    //rList.push_back(Fwd);
    rList.push_back(Brh);
    rList.push_back(RtnEnd);
    rList.push_back(Fwd);
    rList.push_back(Tn);
    rList.push_back(Fwd);
    rList.push_back(Fwd);
    rList.push_back(Tn);
    rList.push_back(Brh);
    rList.push_back(Fwd);
    RiverGenerator1(rList,test,t);
}
void River::LsystemGenerator2()
{
    glm::vec2 test(startPosX,startPosZ);
    int t=0;
    std::vector<OperationType> rList;
    rList.push_back(Fwd);
    rList.push_back(Tn);
    rList.push_back(Brh);
    rList.push_back(Fwd);
    rList.push_back(RtnHere);
    rList.push_back(Tn);
    rList.push_back(Brh);
    rList.push_back(RtnEnd);
    rList.push_back(Tn);
    //rList.push_back(Fwd);
    rList.push_back(RtnHere);
    rList.push_back(Tn);
    rList.push_back(Tn);
    rList.push_back(Fwd);
    //rList.push_back(Fwd);
    rList.push_back(Brh);
    rList.push_back(RtnEnd);
    rList.push_back(Fwd);
    rList.push_back(Tn);
    //rList.push_back(Fwd);
    rList.push_back(Brh);
    rList.push_back(RtnHere);
    rList.push_back(Tn);
    rList.push_back(Tn);
    rList.push_back(Fwd);
    rList.push_back(Brh);
    rList.push_back(Fwd);
    rList.push_back(RtnEnd);
    rList.push_back(Brh);
    rList.push_back(Fwd);
    rList.push_back(Tn);
    rList.push_back(RtnHere);
    rList.push_back(Brh);
    rList.push_back(Tn);
    rList.push_back(Tn);
    //rList.push_back(Fwd);
    rList.push_back(Fwd);
    rList.push_back(RtnEnd);
    //rList.push_back(Fwd);
    RiverGenerator1(rList,test,t);

}
void River::MakeRiver(glm::vec2 r)
{
    int x=round(r[0]);
    int z=round(r[1]);
    containRiver[std::make_tuple(x,0,z)]=true;
    if(x<minX)
        minX=x;
    if(x>maxX)
        maxX=x;
    if(z<minZ)
        minZ=z;
    if(z>maxZ)
        maxZ=z;
}
void River::RiverGenerator1(std::vector<OperationType> rList,glm::vec2 pos,int &times)
{
    float forwardLength;
    float angle=0.0f;
    glm::vec2 origin=pos;
    glm::vec2 returnpos;
    float returnangle;
    //MakeRiver(origin);
    times++;
    for(unsigned int i=0;i<rList.size();i++)
    {
        OperationType ni=rList[i];
        if(ni==Fwd)
        {
            forwardLength=rand()%20+5;
            //std::cout<<forwardLength<<std::endl;
            glm::vec2 newpos;
            newpos[0]=origin[0]+cos(angle)*forwardLength;
            newpos[1]=origin[1]+sin(angle)*forwardLength;
            Line(origin,newpos,times);
            origin=newpos;
            //std::cout<<"pos "<<origin[0]<<","<<origin[1]<<std::endl;

        }
        else if(ni==Brh)
        {
            int branchchance=rand()%100;
            if(branchchance>times*9)
            {
                RiverGenerator1(rList,origin,times);
                //std::cout<<"branch !! "<<std::endl;
            }
        }
        else if(ni==RtnHere)
        {
            returnpos=origin;
            returnangle=angle;
        }
        else if(ni==RtnEnd)
        {
            origin=returnpos;
            angle=returnangle;
        }
        else if(ni==Tn)
        {
            float newangle=((float) rand() / (RAND_MAX))*60.0-30.0;
            //std::cout<<"angle : "<<newangle<<std::endl;
            newangle=glm::radians(newangle);
            angle=angle+newangle;
            if(angle+FLT_EPSILON>M_PI)
                angle-=M_PI;
            if(angle+FLT_EPSILON<-1*M_PI)
                angle+=M_PI;
        }
    }
}

void River::RiverGenerator2(std::vector<OperationType> rList,glm::vec2 pos,int &times)
{
    float forwardLength;
    float angle=0.0f;
    glm::vec2 origin=pos;
    glm::vec2 returnpos;
    float returnangle;
    MakeRiver(origin);
    times++;
    for(unsigned int i=0;i<rList.size();i++)
    {
        OperationType ni=rList[i];
        if(ni==Fwd)
        {
            forwardLength=rand()%8+2;
            //std::cout<<forwardLength<<std::endl;
            glm::vec2 newpos;
            newpos[0]=origin[0]+cos(angle)*forwardLength;
            newpos[1]=origin[1]+sin(angle)*forwardLength;
            Line2(origin,newpos,times);
            origin=newpos;
            //std::cout<<"pos "<<origin[0]<<","<<origin[1]<<std::endl;

        }
        else if(ni==Brh)
        {
            int branchchance=rand()%100;
            if(branchchance>times*7)
            {
                RiverGenerator2(rList,origin,times);
            }
        }
        else if(ni==RtnHere)
        {
            returnpos=origin;
            returnangle=angle;
        }
        else if(ni==RtnEnd)
        {
            origin=returnpos;
            angle=returnangle;
        }
        else if(ni==Tn)
        {
            float newangle=((float) rand() / (RAND_MAX))*120.0-60.0;
            //std::cout<<"angle : "<<newangle<<std::endl;
            newangle=glm::radians(newangle);
            angle=angle+newangle;
            if(angle+FLT_EPSILON>M_PI)
                angle-=M_PI;
            if(angle+FLT_EPSILON<-1*M_PI)
                angle+=M_PI;
        }
    }
}
void River::Line(glm::vec2 v1,glm::vec2 v2,int t)     //using Bresenham's line algorithm
{
    float x1=v1[0];
    float x2=v2[0];
    float y1=v1[1];
    float y2=v2[1];
    int tx,ty;
    int widthcontroller1=(rand()%4-2);
    //cout<<"x,y"<<endl<<x1<<endl<<x2<<endl<<y1<<endl<<y2<<endl;
    const bool steep = (fabs(y2 - y1) > fabs(x2 - x1));
      if(steep)
      {
        std::swap(x1, y1);
        std::swap(x2, y2);
      }

      if(x1 > x2)
      {
        std::swap(x1, x2);
        std::swap(y1, y2);
      }

      const float dx = x2 - x1;
      const float dy = fabs(y2 - y1);

      float error = dx / 2.0f;
      const int ystep = (y1 < y2) ? 1 : -1;
//      int y = (int)y1;

//      const int maxX = (int)x2;
      int y = round(y1);
      const int maxX = round(x2);
//      for(int x=(int)x1; x<maxX; x++)
      for(int x=round(x1); x<maxX; x++)
      {

            if(steep)
            {
                tx=y;
                ty=x;
            }
            else
            {
                tx=x;
                ty=y;
            }
            //containRiver[std::make_tuple(tx,0,ty)]=true;
            int widthcontroller=(rand()%2-1);
            int wd=width+widthcontroller+widthcontroller1-t;
            if(wd<0)
                wd=0;
            ty+=widthcontroller;
            for(int q=0;q<wd;q++)
            {
                containRiver[std::make_tuple(tx,0,ty+q)]=true;
                if(tx<minX)
                    minX=tx;
                if(tx>this->maxX)
                    this->maxX=tx;
                if(ty+q<minZ)
                    minZ=ty+q;
                if(ty+q>maxZ)
                    maxZ=ty+q;
            }






        error -= dy;
        if(error < 0)
        {
            y += ystep;
            error += dx;
        }
      }
}
void River::Line2(glm::vec2 v1,glm::vec2 v2,int t)     //using Bresenham's line algorithm
{
    float x1=v1[0];
    float x2=v2[0];
    float y1=v1[1];
    float y2=v2[1];
    int tx,ty;
    int widthcontroller1=(rand()%3-2);
    //cout<<"x,y"<<endl<<x1<<endl<<x2<<endl<<y1<<endl<<y2<<endl;
    const bool steep = (fabs(y2 - y1) > fabs(x2 - x1));
      if(steep)
      {
        std::swap(x1, y1);
        std::swap(x2, y2);
      }

      if(x1 > x2)
      {
        std::swap(x1, x2);
        std::swap(y1, y2);
      }

      const float dx = x2 - x1;
      const float dy = fabs(y2 - y1);

      float error = dx / 2.0f;
      const int ystep = (y1 < y2) ? 1 : -1;
//      int y = (int)y1;

//      const int maxX = (int)x2;
      int y = round(y1);
      const int maxX = round(x2);
//      for(int x=(int)x1; x<maxX; x++)
      for(int x=round(x1); x<maxX; x++)
      {

            if(steep)
            {
                tx=y;
                ty=x;
            }
            else
            {
                tx=x;
                ty=y;
            }
            //containRiver[std::make_tuple(tx,0,ty)]=true;
            int widthcontroller=(rand()%2-1);
            int wd=width+widthcontroller+widthcontroller1-t;
            if(wd<0)
                wd=0;
            ty+=widthcontroller;
            for(int q=0;q<wd;q++)
            {
                containRiver[std::make_tuple(tx,0,ty+q)]=true;
                if(tx<minX)
                    minX=tx;
                if(tx>this->maxX)
                    this->maxX=tx;
                if(ty+q<minZ)
                    minZ=ty+q;
                if(ty+q>maxZ)
                    maxZ=ty+q;
            }






        error -= dy;
        if(error < 0)
        {
            y += ystep;
            error += dx;
        }
      }
}

void River::GetRiverBound(int &Xmin, int &Xmax, int &Zmin, int &Zmax)
{
    Xmin=minX;
    Xmax=maxX;
    Zmin=minZ;
    Zmax=maxZ;
}
