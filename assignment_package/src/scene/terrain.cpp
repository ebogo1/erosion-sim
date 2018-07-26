#include <scene/terrain.h>
#include <scene/cube.h>

#include "iostream"

Terrain::Terrain(float W, float S, float C, float E, float T)
    : dim(100, 100), K_rain(W), K_sed(S), K_carry(C), K_evap(E), Talus(T)
{}

void Terrain::GenerateBaseTerrain()
{
    for(int x = 0; x < dim.x; ++x)
    {
        for(int z = 0; z < dim.y; ++z)
        {
            heightmap[x][z] = fbm(glm::vec2(x, z) / 15.f);
            watermap[x][z] = 0;
            sedmap[x][z] = 0;
        }
    }
}

// Helper function for erosion algorithm
void Terrain::PopulateNeighbors(int x, int z, std::vector<std::vector<int>> &vector)
{
    for(int i = x-1; i <= x+1; i++)
    {
        for(int j = z-1; j <= z+1; j++)
        {
            if(i >= 0 && j >= 0 && i < dim.x && j < dim.y)
            {
                if(!(i == x && j == z))
                {
                    std::vector<int> coords = std::vector<int>();
                    coords.push_back(i);
                    coords.push_back(j);
                    vector.push_back(coords);
                }
            }
        }
    }    
}

void Terrain::RunHydraulicErosion(int n)
{
    for(int i = 0; i < n; ++i)
    {
        for(int x = 0; x < dim.x; ++x)
        {
            for(int z = 0; z < dim.y; ++z)
            {
                // Step 1: Put a constant amount of rain in current cell
                watermap[x][z] += K_rain;

                // Step 2: Turn a portion of water into sediment
                float newSediment = K_sed * watermap[x][z];
                heightmap[x][z] -= newSediment;
                sedmap[x][z] += newSediment;

                // Step 3: Distribute water & sediments between neighbors
                float altitude = heightmap[x][z] + watermap[x][z];
                float dA; // Average altitude of all neighbors
                float dTotal; // Sum of all positive altitude differences

                std::vector<std::vector<int>> neighbors = std::vector<std::vector<int>>();
                PopulateNeighbors(x, z, neighbors);
                // Set up dA and dTotal
                foreach(auto n, neighbors)
                {
                    float altitude_n = heightmap[n[0]][n[1]] + watermap[n[0]][n[1]];
                    dA += altitude_n;
                    float di = altitude - altitude_n;
                    if(di > 0)
                    {
                        dTotal += di;
                    }
                }
                dA /= neighbors.size();
                // Distribute water and sediment to each neighbor
                foreach(auto n, neighbors)
                {
                    // Distribute water
                    float altitude_n = heightmap[n[0]][n[1]] + watermap[n[0]][n[1]];
                    float di = altitude - altitude_n;
                    float wi = glm::max(glm::min(watermap[x][z], dA) * di / dTotal, 0.f);
                    if(wi <= watermap[x][z])
                    {
                        watermap[x][z] -= wi;
                        watermap[n[0]][n[1]] += wi;
                        // Distribute sediment
                        float mi = sedmap[x][z] * wi / watermap[x][z];
                        sedmap[x][z] -= mi;
                        sedmap[n[0]][n[1]] += mi;
                    }
                }

                // Step 4: Evaporate water from current cell & convert leftover sediment to rock
                watermap[x][z] *= 1.f - K_evap;
                float sedDiff = K_carry * watermap[x][z] - sedmap[x][z];
                if(sedDiff > 0)
                {
                    sedmap[x][z] -= sedDiff;
                    heightmap[x][z] += sedDiff;
                }
            }
        }
    }
}

void Terrain::RunThermalErosion(int n)
{
    for(int i = 0; i < n; ++i)
    {
        for(int x = 0; x < dim.x; ++x)
        {
            for(int z = 0; z < dim.y; ++z)
            {
                // Step 1: Find shortest neighbor (rotated Von Neumann neighborhood)
                float dmax = 0.f; // Maximum height difference with neighbors
                int xdir, zdir; // Direction of neighbor
                // NE neighbor
                if(x < dim.x - 1 && z < dim.y - 1)
                {
                    float d = heightmap[x][z] - heightmap[x+1][z+1];
                    if(d > dmax)
                    {
                        dmax = d;
                        xdir = 1;
                        zdir = 1;
                    }
                }
                // SE neighbor
                if(x < dim.x - 1 && z > 0)
                {
                    float d = heightmap[x][z] - heightmap[x+1][z-1];
                    if(d > dmax)
                    {
                        dmax = d;
                        xdir = 1;
                        zdir = -1;
                    }
                }
                // SW neighbor
                if(x > 0 && z > 0)
                {
                    float d = heightmap[x][z] - heightmap[x-1][z-1];
                    if(d > dmax)
                    {
                        dmax = d;
                        xdir = -1;
                        zdir = -1;
                    }
                }
                // NW neighbor
                if(x > 0 && z < dim.y - 1)
                {
                    float d = heightmap[x][z] - heightmap[x+1][z-1];
                    if(d > dmax)
                    {
                        dmax = d;
                        xdir = 1;
                        zdir = -1;
                    }
                }

                // Step 2: Level out with shortest neighbor
                if(dmax > Talus)
                {
                    heightmap[x][z] -= dmax / 2.f;
                    heightmap[x+xdir][z+zdir] += dmax / 2.f;
                }
            }
        }
    }
}

float Terrain::getHeightAt(int x, int z) const
{
    return heightmap[x][z];
}

float Terrain::random(glm::vec2 n)
{
    return glm::fract(sin(glm::dot(n, glm::vec2(12.9898,78.233))) * 43758.5453123);
}

float Terrain::interpolation(glm::vec2 pos)
{
    glm::vec2 numComponent(glm::floor(pos[0]), glm::floor(pos[1]));
    glm::vec2 fracComponent(glm::fract(pos[0]), glm::fract(pos[1]));

    float a = random(numComponent);
    float b = random(numComponent + glm::vec2(1.0, 0.0));
    float c = random(numComponent + glm::vec2(0.0, 1.0));
    float d = random(numComponent + glm::vec2(1.0, 1.0));

    glm::vec2 u = fracComponent * fracComponent * glm::vec2(3.0 - 2.0 * fracComponent[0], 3.0 - 2.0 * fracComponent[1]);

    return glm::mix(a, b, u[0]) + (c - a)* u[1] * (1.0 - u[0]) + (d - b) * u[0] * u[1];
}

float Terrain::fbm(glm::vec2 pos)
{
    float ret = 0.f;
    float amplitude = 24;
    int frequency = 8;
    for (int i = 0; i < frequency; i++)
    {
        ret += amplitude * interpolation(pos * (float)glm::pow(2, i));
        amplitude *= 0.25;
    }
    return ret;
}
