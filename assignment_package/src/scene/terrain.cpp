#include <scene/terrain.h>

#include <scene/cube.h>

Terrain::Terrain() : dim(100, 100)
{}

float Terrain::getHeightAt(float x, float z) const
{
    if(glm::abs(glm::fract(x) - 0.5f) < 0.001f) {
        return 1.f;
    }
    return 0.f;
}

float Terrain::random(glm::vec2 n) {
    return glm::fract(sin(glm::dot(n, glm::vec2(12.9898,78.233))) * 43758.5453123);
}

float Terrain::interpolation(glm::vec2 pos) {
    glm::vec2 numComponent(glm::floor(pos[0]), glm::floor(pos[1]));
    glm::vec2 fracComponent(glm::fract(pos[0]), glm::fract(pos[1]));

    float a = random(numComponent);
    float b = random(numComponent + glm::vec2(1.0, 0.0));
    float c = random(numComponent + glm::vec2(0.0, 1.0));
    float d = random(numComponent + glm::vec2(1.0, 1.0));

    glm::vec2 u = fracComponent * fracComponent * glm::vec2(3.0 - 2.0 * fracComponent[0], 3.0 - 2.0 * fracComponent[1]);

    return glm::mix(a, b, u[0]) + (c - a)* u[1] * (1.0 - u[0]) + (d - b) * u[0] * u[1];
}

float Terrain::fbm(glm::vec2 pos) {
    float ret = 0.f;
    float amplitude = 24;
    int frequency = 8;

    for (int i = 0; i < frequency; i++) {
        ret = ret + amplitude * interpolation(pos * (float)glm::pow(2, i));
        amplitude = 0.25 * amplitude;
    }

    return ret;
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
