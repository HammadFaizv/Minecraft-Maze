#include "Agent.h"

Agent::Agent(mcpp::Coordinate startLoc)
{
    mcpp::MinecraftConnection mc;
    this->_startLoc = startLoc;
    // bool z_m = mc.getBlock(_startLoc + mcpp::Coordinate(0,0,-1)) == mcpp::Blocks::AIR ? 1 : 0;
    // bool z_p = mc.getBlock(_startLoc  + mcpp::Coordinate(0,0,1)) == mcpp::Blocks::AIR ? 1 : 0;
    // bool x_m = mc.getBlock(_startLoc  + mcpp::Coordinate(-1,0,0)) == mcpp::Blocks::AIR ? 1 : 0;
    // bool x_p = mc.getBlock(_startLoc + mcpp::Coordinate(1,0,0)) == mcpp::Blocks::AIR ? 1 : 0;
}

void Agent::setOriantation(mcpp::Coordinate coord) {
    int z_m = mc.getBlock(_startLoc + mcpp::Coordinate(0,0,-1)) == mcpp::Blocks::AIR ? 1 : 0;
    int z_p = mc.getBlock(_startLoc  + mcpp::Coordinate(0,0,1)) == mcpp::Blocks::AIR ? 1 : 0;
    int x_m = mc.getBlock(_startLoc  + mcpp::Coordinate(-1,0,0)) == mcpp::Blocks::AIR ? 1 : 0;
    int x_p = mc.getBlock(_startLoc + mcpp::Coordinate(1,0,0)) == mcpp::Blocks::AIR ? 1 : 0;

    int sides = z_m + z_p + x_m + x_p;
    if(sides == 1) {
        if(z_m == 1) {front = Z_MINUS; right = X_PLUS;}
        else if(z_p == 1) {front = Z_PLUS; right = X_MINUS;}
        else if(x_p == 1) {front = X_PLUS; right = Z_PLUS;}
        else if(x_m == 1) {front = X_MINUS; right = Z_MINUS;}
    }
    if(sides == 2) {
        // parallel
        if(z_m && z_p) {front = Z_MINUS; right = X_PLUS;}
        if(x_m && x_p) {front = X_MINUS; right = Z_MINUS;}
        
        //adjecent
        if(z_m && x_m) {front = X_MINUS; right = Z_MINUS;}
        if(z_m && x_p) {front = Z_MINUS; right = X_PLUS;}
        if(z_p && x_m) {front = Z_PLUS; right = X_MINUS;}
        if(z_p && x_p) {front = X_PLUS; right = Z_PLUS;}
    }
    if(sides == 3) {
        if(x_m == 0) {front = Z_MINUS; right = X_PLUS;}
        else if(x_p == 0) {front = Z_PLUS; right = X_MINUS;}
        else if(z_m == 0) {front = X_PLUS; right = Z_PLUS;}
        else if(z_p == 0) {front = X_MINUS; right = Z_MINUS;}
    }
}

mcpp::Coordinate Agent::forward(mcpp::Coordinate coord) {
    if(front == Z_MINUS) return coord + mcpp::Coordinate(0,0,-1);
    else if(front == Z_PLUS) return coord + mcpp::Coordinate(0,0,1);
    else if(front == X_MINUS) return coord + mcpp::Coordinate(-1,0,0);
    else return coord + mcpp::Coordinate(1,0,0);
}

bool Agent::checkFront(mcpp::Coordinate coord) {
    if(front == Z_MINUS) return mc.getBlock(coord + mcpp::Coordinate(0,0,-1)) == mcpp::Blocks::AIR;
    else if(front == Z_PLUS) return mc.getBlock(coord + mcpp::Coordinate(0,0,1)) == mcpp::Blocks::AIR;
    else if(front == X_MINUS) return mc.getBlock(coord + mcpp::Coordinate(-1,0,0)) == mcpp::Blocks::AIR;
    else return mc.getBlock(coord + mcpp::Coordinate(1,0,0)) == mcpp::Blocks::AIR;
}
bool Agent::checkRight(mcpp::Coordinate coord) {
    if(right == Z_MINUS) return mc.getBlock(coord + mcpp::Coordinate(0,0,-1)) == mcpp::Blocks::AIR;
    else if(right == Z_PLUS) return mc.getBlock(coord + mcpp::Coordinate(0,0,1)) == mcpp::Blocks::AIR;
    else if(right == X_MINUS) return mc.getBlock(coord + mcpp::Coordinate(-1,0,0)) == mcpp::Blocks::AIR;
    else return mc.getBlock(coord + mcpp::Coordinate(1,0,0)) == mcpp::Blocks::AIR;
}

void Agent::turnRight() {
    if(front == Z_MINUS) {front = X_PLUS; right = Z_PLUS;} 
    else if(front == Z_PLUS) {front = X_MINUS; right = Z_MINUS;}
    else if(front == X_MINUS){front = Z_MINUS; right = X_PLUS;}
    else {front = Z_PLUS; right = X_MINUS;}
}

bool Agent::checkEnd(mcpp::Coordinate coord) {
    int z_m = mc.getBlock(coord + mcpp::Coordinate(0,-1,-1)) == mcpp::Blocks::SMOOTH_RED_SANDSTONE ? 1 : 0;
    int z_p = mc.getBlock(coord  + mcpp::Coordinate(0,-1,1)) == mcpp::Blocks::SMOOTH_RED_SANDSTONE ? 1 : 0;
    int x_m = mc.getBlock(coord  + mcpp::Coordinate(-1,-1,0)) == mcpp::Blocks::SMOOTH_RED_SANDSTONE ? 1 : 0;
    int x_p = mc.getBlock(coord + mcpp::Coordinate(1,-1,0)) == mcpp::Blocks::SMOOTH_RED_SANDSTONE ? 1 : 0;

    if(z_m + z_p + x_m + x_p == 4) {
        return false;
    } else {
        return true;
    }
}

void Agent::turnLeft() {
    this->turnRight();
    this->turnRight();
    this->turnRight();
}

void Agent::solveMaze(mcpp::Coordinate start) {
    mcpp::Coordinate curCoord = start;
    this->setOriantation(curCoord);
    int step = 1;
    std::vector<mcpp::Coordinate> steps;
    steps.push_back(curCoord);
    bool end = false;
    while(!end) {
        if( this->checkRight(curCoord) == true) {
            this->turnRight();
            curCoord = this->forward(curCoord);
            std::cout<<"Step["<<step++<<"]: "<<curCoord.x<<", "<<curCoord.y<<", "<<curCoord.z<<std::endl;
            steps.push_back(curCoord);
        }
        if(this->checkRight(curCoord) == false && this->checkFront(curCoord) == true) {
            curCoord = this->forward(curCoord);
            std::cout<<"Step["<<step++<<"]: "<<curCoord.x<<", "<<curCoord.y<<", "<<curCoord.z<<std::endl;
            steps.push_back(curCoord);
        }
        if(this->checkRight(curCoord) == false && this->checkFront(curCoord) == false) {
            this->turnLeft();
        }
        end = this->checkEnd(curCoord);
    }
    std::cout<<"Step["<<step++<<"]: End reached"<<std::endl;
    
    for(int i = 0; i<steps.size(); i++) {
        mc.setBlock(steps[i], mcpp::Blocks::LIME_CARPET);
        carpet_delay();
        mc.setBlock(steps[i], mcpp::Blocks::AIR);
    }
}

Agent::~Agent()
{
}