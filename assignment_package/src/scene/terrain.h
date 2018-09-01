#pragma once
#include <QList>
#include <la.h>

class Terrain
{
public:
    Terrain(float W, float S, float C, float E, float T);

    glm::vec2 dim; // Dimensions of terrain (number of cells)
    float K_rain; // Constant for water amount received by every cell in each iteration
    float K_sed; // Solubility constant for water to turn into sediment
    float K_carry; // Carrying capacity of water for sediment
    float K_evap; // Evaporation constant for each cell
    float Talus; // Constant for thermal erosion
    float heightmap[120][120];
    float watermap[120][120];
    float sedmap[120][120];

    // Metrics for defining Talus in thermal erosion
    void updateSlopeMap();
    float getErosionScore();
    float slopemap[120][120];
    float score0; // score after 0 iterations

    void GenerateBaseTerrain(); // Populates the heightmap according to a FBM function
    void PopulateNeighbors(int x, int z, std::vector<std::vector<int>> &vector); // Helper function for erosion
     // Erodes heightmap for n iterations
    void RunHydraulicErosion(int n);
    void RunThermalErosion(int n);
    float getHeightAt(int x, int z) const;

    // FBM noise helper functions
    float random(glm::vec2 n);
    float interpolation(glm::vec2 pos);
    float fbm(glm::vec2 pos);

};
