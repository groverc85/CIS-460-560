#include <scene/scene.h>
#include <scene/cube.h>

Block::Block() : pos(0,0,0), m_active(true), blockType(Default)
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

Scene::Scene() : dimensions(64, 64, 64), lowerBoundX(0), higherBoundX(64), lowerBoundY(0), \
    higherBoundY(64), lowerBoundZ(0), higherBoundZ(64)
{
    for (int x = 0; x < 64; x++)
        for (int y = 0; y < 64; y++)
            for (int z = 0; z < 64; z++)
            {
                pos = std::make_tuple(x,y,z);
                containBlock[pos] = false;
//                scene[pos] = Default;
            }
}

void Scene::CreateTestScene()
{
    Perlin* perlin_generator = new Perlin();

    for (int x = lowerBoundX; x < higherBoundX; x++)
        for(int z = lowerBoundZ; z < higherBoundZ; z++)
        {
            int height = perlin_generator->OctavePerlin(x/64.0, 0, z/64.0, 6, 1) * 64;
            for (int i = 0; i < height; i++)
            {
                pos = std::make_tuple(x, i, z);
                containBlock[pos] = true;
                scene[pos] = Default;
            }
        }
}
