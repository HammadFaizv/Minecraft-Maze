#include "Maze.h"
#include <random>

// using CoordinateBlockPair = std::pair<mcpp::Coordinate, mcpp::BlockType>;


void clearAbove(int row, int col, mcpp::Coordinate basePoint) {
    mcpp::MinecraftConnection mc; 
    mcpp::Coordinate cord = basePoint + mcpp::Coordinate(row, 0, col); //gets the coordinates of the highest non air block

    int y = mc.getHeight(cord.x, cord.z); //y value of the highest non air block
   // std::cout << "cord:           " << cord << std::endl;
   // std::cout << "cord y highest: " << y << std::endl;
    int bpHeight = basePoint.y; //gets the y value of the basePoint

    while(y>bpHeight){
        cord.y = 0;
        mc.setBlock(cord + mcpp::Coordinate(0, y, 0), mcpp::Blocks::AIR);
        y--;
    }
}

void captureTerrain(int row, int col, mcpp::Coordinate basePoint, std::vector<std::pair<mcpp::Coordinate, mcpp::BlockType>>* terrainPtr) {
    mcpp::MinecraftConnection mc;
    mcpp::Coordinate cord = basePoint + mcpp::Coordinate(row, 0, col); 
    int y = mc.getHeight(cord.x, cord.z); // getting highest non-air block y
    int bpHeight = basePoint.y; 

    std::vector<std::pair<mcpp::Coordinate, mcpp::BlockType>>& terrain = *terrainPtr; //setting vector to the pointer

    while(bpHeight > y) {
        cord.y = 0;
        mcpp::Coordinate vectorCords = cord + mcpp::Coordinate(0, y, 0);
        mcpp::BlockType block = mc.getBlock(vectorCords);
        terrain.push_back(std::make_pair(vectorCords, block));
        y++;
    }
}

void buildBelow(int row, int col, mcpp::Coordinate basePoint) {
    mcpp::MinecraftConnection mc; 
    mcpp::Coordinate cord = basePoint + mcpp::Coordinate(row, 0, col); 
    int y = mc.getHeight(cord.x, cord.z); 
    int bpHeight = basePoint.y; 

    while(bpHeight > y) {
        cord.y = 0;
        mc.setBlock(cord + mcpp::Coordinate(0, y, 0), mcpp::Blocks::SMOOTH_RED_SANDSTONE);
        y++;
    }
}

void restoreTerrain(std::vector<std::pair<mcpp::Coordinate, mcpp::BlockType>>* terrainPtr) {
    mcpp::MinecraftConnection mc; 
    std::vector<std::pair<mcpp::Coordinate, mcpp::BlockType>>& terrain = *terrainPtr;
    for(const auto& pair : terrain) {
        mc.setBlock(pair.first, pair.second);
    }
}



Maze::Maze(mcpp::Coordinate basePoint, unsigned int xlen, 
                                    unsigned int zlen,
                                    bool mode)
{
    this->basePoint = basePoint;
    this->xlen = xlen;
    this->zlen = zlen;
    this->mode = mode;

    envStructure = new char*[this->xlen];
    for(int i =0; i < this->xlen; i++){
        envStructure[i] = new char[this->zlen];
    }
}

mcpp::Coordinate Maze::getCoord(){
    return this->basePoint;
}
int Maze::getXlen(){
    return this->xlen;
}
int Maze::getZlen(){
    return this->zlen;
}

void Maze::setEnvElement(int row, int col, char c) {
    this->envStructure[row][col] = c;
}

char Maze::getEnvElement(int row, int col){
    return this->envStructure[row][col];
}

void Maze::build_maze(void){
    mcpp::MinecraftConnection mc;
    mc.postToChat("Generating maze...");

    // std::vector<CoordinateBlockPair> terrain; 
    std::vector<std::pair<mcpp::Coordinate, mcpp::BlockType>>* terrainPtr = &terrain;  // pointer to the terrain vector
    
    mc.setPlayerPosition(this->basePoint + mcpp::Coordinate(0, 10, 0));

    // first capture all non-air blocks
    // now we can clear all blocks and make the maze

    for (int row = 0; row < this->xlen; row++) {
        for (int col = 0; col < this->zlen; col++) {
            captureTerrain(row, col, this->basePoint, terrainPtr); // capture blocks
            clearAbove(row, col, this->basePoint); //function to clear all blocks above basepoint
            mc.setBlock(this->basePoint + mcpp::Coordinate(row, 0, col), mcpp::Blocks::AIR); //replace env with air
            buildBelow(row, col, this->basePoint); //function to build up blocks beneath maze
        }
    }

    std::cout << "Terrain size: " << terrain.size() << std::endl; //prints how many blocks under the maze that got replaced

    for (const auto& pair : terrain) { //for loop to print out coordinates and blockType of terrain
        const mcpp::Coordinate& coord = pair.first;
        const mcpp::BlockType& blockType = pair.second;
    }

    for (int row = 0; row < this->xlen; row++) { //maze building
        for (int col = 0; col < this->zlen; col++) {
            std::cout << envStructure[row][col];
            mc.setBlock(this->basePoint + mcpp::Coordinate(row, 0, col), mcpp::Blocks::AIR);
            if (envStructure[row][col] == 'x') {
                mc.setBlock(this->basePoint + mcpp::Coordinate(row, 0, col), mcpp::Blocks::BRICKS);
                mc.setBlock(this->basePoint + mcpp::Coordinate(row, 1, col), mcpp::Blocks::BRICKS);
                mc.setBlock(this->basePoint + mcpp::Coordinate(row, 2, col), mcpp::Blocks::BRICKS);
            }
        }
        std::cout << std::endl;
    }
    mc.postToChat("Maze is ready!!!");
}




Maze::~Maze()
{
    restoreTerrain(&terrain); // Restore the terrain when Maze is destroyed
    for (int row = 0; row < this->xlen ; row++) {
        delete[] envStructure[row];
    }
    delete[] envStructure;
}