#pragma once
#include <QList>
#include <la.h>

class Terrain
{
public:
    Terrain();

    glm::vec2 dim;
    float heightmap[100][100];

    void GenerateBaseTerrain();

    float getHeightAt(float x, float z) const;
    float random(glm::vec2 n);
    float interpolation(glm::vec2 pos);
    float fbm(glm::vec2 pos);
};
