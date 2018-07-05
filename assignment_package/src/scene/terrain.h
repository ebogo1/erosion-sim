#pragma once
#include <QList>
#include <la.h>

class Terrain
{
public:
    Terrain();

    glm::vec2 dim;
    float heightmap[4][4];

    void GenerateBaseTerrain();

    float getHeightAt(float x, float z) const;
};
