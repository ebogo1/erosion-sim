#include <scene/terrain.h>

#include <scene/cube.h>

Terrain::Terrain() : dim(4, 4)
{}

float Terrain::getHeightAt(float x, float z) const
{
    if(glm::abs(glm::fract(x) - 0.5f) < 0.001f) {
        return 0.25f;
    }
    return 0.f;
}

void Terrain::GenerateBaseTerrain()
{
    for(int x = 0; x < dim.x; ++x) {
        for(int z = 0; z < dim.y; ++z) {
            if(x == 0) {
                heightmap[x][z] = 1;
            }
            else {
                heightmap[x][z] = 0;
            }
        }
    }
}
