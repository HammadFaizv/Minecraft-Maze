#ifndef ASSIGN_MAZE_H
#define ASSIGN_MAZE_H

#include <vector>
#include <mcpp/mcpp.h>

class Maze
{

public:
    // using CoordinateBlockPair = std::pair<mcpp::Coordinate, mcpp::BlockType>;

    Maze(mcpp::Coordinate basePoint, unsigned int xlen, 
                                        unsigned int zlen,
                                        bool mode);
    ~Maze();
    mcpp::Coordinate getCoord();
    int getXlen();
    int getZlen();
    void setEnvElement(int row, int col, char c);
    char getEnvElement(int row, int col);
    void build_maze(void);

private:
    std::vector<std::pair<mcpp::Coordinate, mcpp::BlockType>> terrain; // to hold the terrain
    mcpp::Coordinate basePoint;
    int xlen;
    int zlen;
    bool mode;
    char** envStructure;
};




#endif //ASSIGN_MAZE_H