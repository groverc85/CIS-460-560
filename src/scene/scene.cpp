#include <scene/scene.h>
#include <scene/cube.h>
#include <time.h>
#include <random>

#define BOUNDRY 160
#define M_PI  3.14159
const int Step=3;
const int seaLevel=79;
const int riversideLimit=50;
Block::Block() : pos(0,0,0),lifeTime(8), m_active(true), blockType(Default)
{
}

bool Block::isActive()
{
    return m_active;
}

void Block::SetActive(bool active)
{
    m_active = active;
}

Scene::Scene() : dimensions(BOUNDRY, BOUNDRY, BOUNDRY), lowerBoundX(0), higherBoundX(BOUNDRY), lowerBoundY(0), \
    higherBoundY(BOUNDRY), lowerBoundZ(0), higherBoundZ(BOUNDRY)
{
    for (int x = 0; x < BOUNDRY; x++)
        for (int y = 0; y < BOUNDRY; y++)
            for (int z = 0; z <BOUNDRY; z++)
            {
                pos = std::make_tuple(x,y,z);
                containBlock[pos] = false;
//                scene[pos] = Default;
            }

    river1=River(10,35,1);
    river2=River(40,130,2);
    CalculateRiverside();
}

void Scene::CreateTestScene()
{
    Perlin* perlin_generator = new Perlin();
    srand (time(NULL));


    for (int x = lowerBoundX; x < higherBoundX; x++)
        for(int z = lowerBoundZ; z < higherBoundZ; z++)
        {
            int height = perlin_generator->OctavePerlin((float)x/BOUNDRY, 0, (float)z/BOUNDRY, 6, 1) * BOUNDRY;

            // height field is around ~80 with 16 as variance
            // terrain part + part between sea level and bedrock
            // COORDINATE: bedrock: 16, sea level: 80, above sea level(terrain): 160

//            // generate snow effect, Y: height~160
//            pos = std::make_tuple(x, height + 10 + clock() % 30, z);
//            containBlock[pos] = true;
//            scene[pos] = Snow;

            // generate Ice Plains Spikes, Y max: height+27
            for (int count = 0; count < 4; count++)
            {
                if ((x >= (lowerBoundX+higherBoundX)/2- 7*count - 21) && (x <= (lowerBoundX+higherBoundX)/2 - 7*count - 18) &&
                        (z >= (lowerBoundZ+higherBoundZ)/2 - 7*count - 21) && (z <= (lowerBoundZ+higherBoundZ)/2 - 7*count - 18))
                {
                    int spikeHeight = (int)((double)rand()/RAND_MAX*10); // random value between 0 to 10
                    for (int i = height+16; i < height + 16 + spikeHeight; i++)
                    {
                        pos = std::make_tuple(x, i, z);
                        containBlock[pos] = true;
                        scene[pos] = Lava;
                    }
                }
            }


//            // generate grass
//            if (x >= (lowerBoundX+higherBoundX)/3-3 && x <= (lowerBoundX+higherBoundX)/3+3 && z >= (lowerBoundZ+higherBoundZ)/3-3 && z <= (lowerBoundZ+higherBoundZ)/3+3)
//            {
//                int grassHeight = (int)((double)rand()/RAND_MAX*3); // random value between 0 to 2
//                pos = std::make_tuple(x, height+16+grassHeight, z);
//                containBlock[pos] = true;
//                scene[pos] = Grass;
//            }

            //generate Ice Plains
            pos = std::make_tuple(x, height+15, z);
            containBlock[pos] = true;
            scene[pos] = Snow;

//            // generate grassland
//            pos = std::make_tuple(x, height+15, z);
//            containBlock[pos] = true;
//            scene[pos] = GrassLand;

            // terrain
            for (int i = 64; i < height+15; i++)
            {
                pos = std::make_tuple(x, i, z);
                containBlock[pos] = true;
                scene[pos] = Default;
            }

            // stone
            for (int i = 16; i < 64; i++)
            {
                pos = std::make_tuple(x, i, z);
                containBlock[pos] = true;
                scene[pos] = Stone;
            }
            // At a depth of 128 blocks, a layer of BEDROCK should be generated;
            for (int i = 0; i < 16; i++)
            {
                pos = std::make_tuple(x, i, z);
                containBlock[pos] = true;
                scene[pos] = BedRock;
            }

        }
    ///------------------added by yi huang----------------------------///
    for(int x=70;x<76;x++){
               for(int z=70;z<76;z++){

                   pos = std::make_tuple(x, 100, z);
                   containBlock[pos] = true;
                   scene[pos] = Wood;
                   pos = std::make_tuple(x, 101, z);
                   containBlock[pos] = true;
                   scene[pos] = Wood;
               }
           }

    // generate random underground x, y, z pos

    int stride = 1;
    int radius = 1;
    int minLength = 10;
    int maxLength = 30;
    int maxCount = maxLength / minLength;
    double randomEllipsoid = 0;
    double randomCoreIron = 0;
    int numberOfTunnels = (double) rand() / RAND_MAX * 5 + 1; // random number between 1-5;
    //int numberOfTunnels = 1;
    int currentTunnelNumber = 0;

    while (currentTunnelNumber < numberOfTunnels)
    {
        int x_pos = BOUNDRY/2 - (double)rand() / RAND_MAX * 50;
        int y_pos = BOUNDRY/2 + 30;
        int z_pos = BOUNDRY/2 - (double)rand() / RAND_MAX * 50;
        int count = 0;
        bool ellipsoidUsed = false;


        double walkLength = (double)rand() / RAND_MAX;
        printf("walklength is : %f\n", walkLength);

        while (walkLength <= 0.5 && count <= maxCount)
        {
            for (int i = 0; i < minLength; i++)
            {
                double angle = perlin_generator->OctavePerlin((float)x_pos, (float)y_pos, (float)z_pos, 6, 1) *M_PI ;
                printf("sin: %f, cos: %f\n", sin(angle), cos(angle));
                if (cos(angle) <= 0)
                    x_pos -= stride * 4;
                else
                    x_pos += stride * 4;
                if (sin(angle) <= 0)
                    z_pos -= stride * 4;
                else
                    z_pos += stride * 4;

                y_pos -= stride;
                radius = ((double)rand() / RAND_MAX) * 2 + 2; // random value of 2,3

                randomEllipsoid = (double)rand() / RAND_MAX;

                printf("randomEllipsoid is : %f\n", randomEllipsoid);


                if (randomEllipsoid >= 0.5 && ellipsoidUsed == false)
                {
                    // hollow ellipsoid cave & make LAVA blocks、
                    printf("creating cave\n");

                    int x_pos_random = (double) rand()/RAND_MAX * 4;
                    int y_pos_random = (double) rand()/RAND_MAX * 4;
                    int z_pos_random = (double) rand()/RAND_MAX * 4;

                    for (int y = y_pos - y_pos_random; y <= y_pos + y_pos_random; y++)
                    {
                        // bottom layer should be LAVA.
                        if (y == y_pos - y_pos_random)
                        {
                            for (int x = x_pos - x_pos_random; x <= x_pos + x_pos_random; x++)
                                for (int z = z_pos - z_pos_random; z <= z_pos + z_pos_random; z++)
                                {
                                    pos = std::make_tuple(x, y, z);
                                    containBlock[pos] = true;
                                    scene[pos] = Lava;
                                }

                        }
                        else
                        {
                            for (int x = x_pos - x_pos_random; x <= x_pos + x_pos_random; x++)
                                for (int z = z_pos - z_pos_random; z <= z_pos + z_pos_random; z++)
                                {
                                    pos = std::make_tuple(x, y, z);

                                    randomCoreIron = (double) rand() / RAND_MAX;

                                    if (randomCoreIron > 0.9)
                                    {
                                        containBlock[pos] = true;
                                        scene[pos] = IronOre;
                                    }
                                    else if (randomCoreIron > 0.8)
                                    {
                                        containBlock[pos] = true;
                                        scene[pos] = Coal;
                                    }
                                    else
                                        containBlock[pos] = false;
                                }
                        }

                        x_pos_random = (double) rand()/RAND_MAX * 4;
                        z_pos_random = (double) rand()/RAND_MAX * 4;
                    }
                    ellipsoidUsed = true;
                }

                // tunnel create
                else
                {
                    for (int x = x_pos - radius; x <= x_pos + radius; x++)
                        for (int y = y_pos - radius; y <= y_pos + radius; y++)
                            for (int z = z_pos - radius; z <= z_pos + radius; z++)
                            {
                                pos = std::make_tuple(x, y, z);
                                // randomly distribute COAL and IRON ORE blocks throughout your tunnels and large caves
                                randomCoreIron = (double) rand() / RAND_MAX;
                                if (randomCoreIron > 0.9)
                                {
                                    containBlock[pos] = true;
                                    scene[pos] = IronOre;
                                }
                                else if (randomCoreIron > 0.8)
                                {
                                    containBlock[pos] = true;
                                    scene[pos] = Coal;
                                }
                                else
                                    containBlock[pos] = false;
                            }
                }

                printf("%d %d %d\n", x_pos, y_pos, z_pos);


            }
    //         generate random number between 0 to 1 to determine if we should halt walking
            walkLength = (double) rand() / RAND_MAX;
            randomEllipsoid = (double) rand() / RAND_MAX;
            count += 1;
        }

        currentTunnelNumber++;
    }

    UpdateRiver();
}


void Scene::UpdateRiver()
{
    for (int x = lowerBoundX; x < higherBoundX; x++)
    {
        for(int z = lowerBoundZ; z < higherBoundZ; z++)
        {
            std::tuple<int,int,int> posr = std::make_tuple(x, 0, z);
            if((river1.containRiver[posr]==true)&&
                   (containBlock[posr]==true)&&(scene[posr]!=Water))
            {
                MakeWater(x,0+seaLevel,z);
            }
            if((river2.containRiver[posr]==true)&&
                    (containBlock[posr]==true)&&(scene[posr]!=Water))
            {
                MakeWater(x,0+seaLevel,z);
            }
            int highty=riversideHeight[std::make_tuple(x,z)];
            if((highty<10000)&&(highty>0))
            {
                for(int y=lowerBoundY;y<higherBoundY;y++)
                {
                    if(y>highty)
                    {
                        containBlock[std::make_tuple(x,y,z)]=false;

                    }
                }
            }
        }

    }
}



void Scene::MakeWater(int x, int y, int z)
{
    for(int yn=lowerBoundY;yn<higherBoundY;yn++)
    {
        if(yn>y)
            containBlock[std::make_tuple(x,yn,z)]=false;
        else
        {    containBlock[std::make_tuple(x,yn,z)]=true;
             scene[std::make_tuple(x,yn,z)]=Water;
        }
    }
}

void Scene::CalculateRiverside()
{
    int minx1=0;int minx2=0;
    int maxx1=0;int maxx2=0;
    int minz1=0;int minz2=0;
    int maxz1=0;int maxz2=0;
    river1.GetRiverBound(minx1,maxx1,minz1,maxz1);
    river2.GetRiverBound(minx2,maxx2,minz2,maxz2);
    if(minx1>minx2)
        minx1=minx2;
    if(minz1>minz2)
        minz1=minz2;
    if(maxx1<maxx2)
        maxx1=maxx2;
    if(maxz1<maxz2)
        maxz1=maxz2;
    minx1-=riversideLimit;
    minz1-=riversideLimit;
    maxx1+=riversideLimit;
    maxz1+=riversideLimit;
    for (int x = minx1; x <= maxx1; x++)
    {
        for(int z = minz1; z <= maxz1; z++)
        {
            riversideHeight[std::make_tuple(x,z)]=10000;
        }
    }

    for (int x = minx1; x <= maxx1; x++)
    {
        for(int z = minz1; z <= maxz1; z++)
        {
            if(IfWater(x,z))
            {
                int mht=0+seaLevel;
                int aht=0+seaLevel;

                if(IfWater(x,z-1)==false)
                {
                    for(int minusfactor=1;minusfactor<=riversideLimit;minusfactor++)
                    {
                        mht+=Step;
                        if(IfWater(x,z-minusfactor)==false)
                        {
                            std::tuple<int,int> posside=std::make_tuple(x,z-minusfactor);
                            if(riversideHeight[posside]>mht)
                             riversideHeight[posside]=mht;

                        }else break;
                    }
                }
                if(IfWater(x,z+1)==false)
                {
                    for(int addfactor=1;addfactor<riversideLimit;addfactor++)
                    {
                        //std::tuple<int,int,int> posnear = std::make_tuple(x, 0, z+addfactor);
                        aht+=Step;
                        if(IfWater(x,z+addfactor)==false)
                        {
                            std::tuple<int,int> posside=std::make_tuple(x,z+addfactor);
                            if(riversideHeight[posside]>aht)
                             riversideHeight[posside]=aht;

                        }else break;
                    }
                }




                int xmht=0+seaLevel;
                int xaht=0+seaLevel;
                bool corner=false;
                if(IfWater(x,z-1)==false)
                        corner=true;
                if(IfWater(x,z+1)==false)
                        corner=true;

                if(IfWater(x-1,z)==false)
                {


                    for(int xminusfactor=1;xminusfactor<=riversideLimit;xminusfactor++)
                    {


                        xmht+=Step;
                        if(IfWater(x-xminusfactor,z)==false)
                        {
                            std::tuple<int,int> posside=std::make_tuple(x-xminusfactor,z);
                            if(riversideHeight[posside]>xmht)
                            { riversideHeight[posside]=xmht;
                             }
                            if(corner)
                                    CalculateRiversideHelper(x-xminusfactor,xmht,z);



                        }else break;
                    }
                }

                if(IfWater(x+1,z)==false)
                {
                    for(int addfactor=1;addfactor<riversideLimit;addfactor++)
                    {

                        xaht+=Step;
                        if(IfWater(x+addfactor,z)==false)
                        {


                            std::tuple<int,int> posside=std::make_tuple(x+addfactor,z);
                            if(riversideHeight[posside]>xaht){
                             riversideHeight[posside]=xaht;}

                            if(corner)
                                    CalculateRiversideHelper(x+addfactor,xaht,z);


                        }else break;
                    }
                }


            }
        }

    }

}
bool Scene::IfWater(int x, int z)
{
    std::tuple<int,int,int> posi=std::make_tuple(x,0,z);
    if(river1.containRiver[posi]==true)
        return true;
    else if(river2.containRiver[posi]==true)
        return true;
    else
        return false;
}
void Scene::CalculateRiversideHelper(int x, int y, int z)
{
    int mht=y;
    int aht=y;
    if(IfWater(x,z-1)==false)
    {
        for(int minusfactor=1;minusfactor<=riversideLimit;minusfactor++)
        {

            mht+=Step;
            if(IfWater(x,z-minusfactor)==false)
            {
                std::tuple<int,int> posside=std::make_tuple(x,z-minusfactor);
                if(riversideHeight[posside]>mht)
                 riversideHeight[posside]=mht;

            }else
                break;
        }
    }
    if(IfWater(x,z+1)==false)
    {
        for(int addfactor=1;addfactor<riversideLimit;addfactor++)
        {
            aht+=Step;
            if(IfWater(x,z+addfactor)==false)
            {
                std::tuple<int,int> posside=std::make_tuple(x,z+addfactor);
                if(riversideHeight[posside]>aht)
                 riversideHeight[posside]=aht;

            }else
                break;
        }
    }
}
