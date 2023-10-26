#include <iostream>
#include <vector>
#include <utility>

#include <mcpp/mcpp.h>

#include "menuUtils.h"
#include "Maze.h"
#include "Agent.h"

#define NORMAL_MODE 0
#define TESTING_MODE 1
mcpp::MinecraftConnection mc; 

enum States{
    ST_Main,
    ST_GetMaze,
    ST_BuildMaze,
    ST_SolveMaze,
    ST_Creators,
    ST_Exit
};

int main(void){

    bool mode = NORMAL_MODE;
    //read Mode

    States curState;
    

    if(mode == NORMAL_MODE){
        
        
    }else{
        std::cout << "not";
        printStartText();
    }

    
    mc.doCommand("time set day"); 

    curState = ST_Main; 

    
    Maze* newMaze = nullptr;   
    Agent* newAgent = nullptr;
    while (curState != ST_Exit)
    {
        

            if (curState == ST_Main) {
            printMainMenu();

            int menuChoice = 0;
            std::cin >> menuChoice;

            if (menuChoice == 1) {
                curState = ST_GetMaze;
            } else if (menuChoice == 2) {
                curState = ST_BuildMaze;
            } else if (menuChoice == 3) {
                curState = ST_SolveMaze;
            } else if (menuChoice == 4) {
                curState = ST_Creators;
            } else if (menuChoice == 5) {
                curState = ST_Exit;
            } else {
                std::cout << "Input Error: Enter a number between 1 and 5..." << std::endl;
            }
        } else if (curState == ST_GetMaze) {
            printGenerateMazeMenu();
            int generateChoice = 0;
            std::cin >> generateChoice;

            if (generateChoice == 1) {
                std::cout << "Reading Maze from terminal..." << std::endl;
                int build_x = 0;
                int build_y = 0;
                int build_z = 0;
                ReadBuildLocation(build_x, build_y, build_z);
                mcpp::Coordinate startCoord(build_x, build_y, build_z);

                int envHeight = 0;
                int envWidth = 0;
                ReadEnvSize(envHeight, envWidth); 
                newMaze = new Maze(startCoord, envHeight, envWidth, mode);

                std::cout << "Enter the maze structure:" << std::endl;
                char readChar;
                for (int row = 0; row < envHeight; row++) {
                    for (int col = 0; col < envWidth; col++) {
                        std::cin >> readChar;
                        newMaze->setEnvElement(row, col, readChar);
                    }
                }
                
                std::cout << "Maze generated successfully" << std::endl;
                std::cout << "**Printing Maze**" << std::endl;

                std::cout << "BasePoint: (" << build_x <<", " << build_y << ", " << build_z << ")" << std::endl;
                std::cout << "Structure:" << std::endl;
                
                // generate maze in game
                // newMaze->build_maze();
                

                for (int row = 0; row < envHeight; row++) {
                    for (int col = 0; col < envWidth; col++) {
                        std::cout << newMaze->getEnvElement(row, col);
                    }
                    std::cout << std::endl;
                }

                std::cout << "**End Printing Maze**" << std::endl;

                curState = ST_Main;
            } else if (generateChoice == 2) {
                
                std::cout << "Generating Random Maze..." << std::endl;
                curState = ST_Main;
            } else if (generateChoice == 3) {
                curState = ST_Main; 
            } else {
                std::cout << "Input Error: Enter a number between 1 and 3..." << std::endl;
            }
        } else if (curState == ST_BuildMaze) {
            if(newMaze == nullptr){
                std::cout << "Generate a maze to build(opt. 1)";
            }else{
                newMaze->build_maze();
                std::cout << "Building maze" << std::endl;
            }
            
            curState = ST_Main;
        } else if (curState == ST_SolveMaze) {
            if(newMaze == nullptr) {
                std::cout<<"First build a maze"<<std::endl;
            } else {
                printSolveMazeMenu();
                int num;
                std::cin >> num;
                if(num == 1) {
                    std::cout<<"Go ahead!!! All the best!!!"<<std::endl;
                } else if (num == 2) {
                    // agent
                    std::cout<<"Enter the position of the maze you are at:"<<std::endl;
                    int x, y, z;
                    std::cin>>x>>y>>z;
                    mcpp::Coordinate startLoc(x,y,z);
                    newAgent = new Agent(startLoc);
                    // solve the mage using agent
                    newAgent->solveMaze(startLoc);
                }
            }
            
            curState = ST_Main;
        } else if (curState == ST_Creators) {
            printTeamInfo();
            curState = ST_Main;
        }
            
    }
    
    if (curState == ST_Exit) {
        delete newMaze;
        printExitMassage();
        
    }

    


    return EXIT_SUCCESS;

}
