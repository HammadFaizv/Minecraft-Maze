#ifndef ASSIGN3_AGENT_H
#define ASSIGN3_AGENT_H

#include <iostream>
#include <chrono>
#include <thread>
#include <mcpp/mcpp.h>

#define MOVE_XPLUS mcpp::Coordinate(1,0,0)
#define MOVE_XMINUS mcpp::Coordinate(-1,0,0)
#define MOVE_ZPLUS mcpp::Coordinate(0,0,1)
#define MOVE_ZMINUS mcpp::Coordinate(0,0,-1)

enum solveAlgorithm{
        RIGHT_HAND_FOLLOW,
        BREATH_FIRST_SEARCH,
};

enum AgentOrientation{
    X_PLUS,
    Z_PLUS,
    X_MINUS,
    Z_MINUS
};

void carpet_delay() {
    using namespace std::this_thread; // sleep_for, sleep_until
    using namespace std::chrono; // nanoseconds, system_clock, seconds

    sleep_for(nanoseconds(10));
    sleep_until(system_clock::now() + seconds(1));
}

class Agent
{

public:
    Agent(mcpp::Coordinate startLoc);
    void setOriantation(mcpp::Coordinate coord);
    void solveMaze(mcpp::Coordinate start);
    mcpp::Coordinate forward(mcpp::Coordinate coord);
    bool checkRight(mcpp::Coordinate coord);
    bool checkFront(mcpp::Coordinate coord);
    bool checkEnd(mcpp::Coordinate coord);
    void turnRight();
    void turnLeft();
    ~Agent();

private:
    /* data */
    mcpp::MinecraftConnection mc;
    std::vector<mcpp::Coordinate> solution;
    mcpp::Coordinate _startLoc;
    solveAlgorithm RHF;
    AgentOrientation front;
    AgentOrientation right;
};



#endif //ASSIGN3_AGENT_H