#include <fstream>
#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include <numeric>
#include <sstream>

enum direction
{
    U,
    D,
    L,
    R
};

struct ropePoint
{
    int x;
    int y;

    bool isTail;
    int tailSteps;


    ropePoint(int x, int y, bool isTail)
    {
        this->x = x;
        this->y = y;
        this->isTail = isTail;
        this->tailSteps = 0;
    }

    void movePoint(std::string dir, int numSteps, ropePoint &tail)
    {
        std::cout << "tailSteps: " << tail.tailSteps << std::endl;
        if(dir == "U")
        {
                for(int i = 0; i < numSteps; i++)
                {
                    this->y--;
                    tail.updateTail(this->x, this->y);
                }
        }
                if(dir == "D")
                {
                    for(int i = 0; i < numSteps; i++)
                {
                    this->y++;
                    tail.updateTail(this->x, this->y);
                }
                }
            if(dir == "L")
            {
            for(int i = 0; i < numSteps; i++)
            {
                    this->x--;
                    tail.updateTail(this->x, this->y);
            }
            }
            if(dir == "R")
            {
                for(int i = 0; i < numSteps; i++)
            {
                    this->y--;
                    tail.updateTail(this->x, this->y);
            }
            }

        }
    void updateTail(int headX, int headY)
    {
        bool moved = false;
        if(std::abs(this->x-headX) > 1)
        {
            moved = true;
            if(this->x > headX)
            {
            this->x++;
            } else
            {
                this->x--;
            }
        }
        if(std::abs(this->y-headY) > 1)
        {
            moved = true;
            if(this->y < headY)
            {
                this->y++;
            } else
            {
                this->y--;
            }
        }
        if(moved)
        {
            this->tailSteps++;
        }
    }

};

int main()
{
    std::ifstream inputFile("inputexample.txt");
    if(inputFile.is_open()) std::cout << "File Opened" << std::endl;
    std::string command, moves;

    ropePoint head(0, 0, false), tail(0, 0, true);


    while(inputFile >> command >> moves)
    {
        head.movePoint(command, std::stoi(moves), tail);
        std::cout << command << " " << moves << std::endl;
    }


    std::cout << "Answer to Part 1: " << tail.tailSteps << std::endl;
    //std::cout << "Answer to Part 2: " << smallestDir << std::endl;
    return 0;
}