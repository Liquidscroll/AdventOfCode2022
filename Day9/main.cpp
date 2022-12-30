#include <fstream>
#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include <numeric>
#include <sstream>
#include <map>
#include <cmath>
#include <tuple>

enum direction
{
    U,
    D,
    L,
    R
};

struct ropePoint
{
    int x = 0;
    int y = 0;
};
std::map<std::string, bool> moveTail(ropePoint* points, int arrSize, std::map<std::string, bool>& visited)
{
    for(int knot = 1; knot < arrSize; knot++)
    {
        int xDist = (points[knot-1].x - points[knot].x);
        int yDist = (points[knot-1].y - points[knot].y);

        if(sqrt(xDist * xDist + yDist * yDist) > 1.5)
        {
            if(xDist > 0) { points[knot].x++; }
            else if(xDist < 0) { points[knot].x--; }
        
            if(yDist > 0) { points[knot].y++; }
            else if(yDist < 0) { points[knot].y--; }
        
            if(knot == arrSize - 1)
            {
                std::string pos = std::to_string(points[knot].x) + "," + std::to_string(points[knot].y);

                visited.insert({std::string(pos), true});
            }
        
        }
    }
    return visited;
}
std::map<std::string, bool> movePoint(std::string command, ropePoint* points, int arrSize, std::map<std::string, bool>& visited)
{
   
        if(command == "U") { points[0].y++;}
        else if(command == "D") {points[0].y--;}
        else if (command == "L") { points[0].x--;}
        else if(command == "R") { points[0].x++;}
   

    return moveTail(points, arrSize, visited);
}




int main()
{
    std::ifstream inputFile("input.txt");
    if(inputFile.is_open()) std::cout << "File Opened" << std::endl;
    std::string command, moves;
    const int knotsPart1 = 2;
    const int knotsPart2 = 10;

    ropePoint part1[knotsPart1];
    ropePoint part2[knotsPart2];

    std::map<std::string, bool> visitedPointsPart1;
    std::map<std::string, bool> visitedPointsPart2;

    visitedPointsPart1.insert({"0,0", true});
    visitedPointsPart2.insert({"0,0", true});
    
    while(inputFile >> command >> moves)
    {
        for(int i = 0; i < stoi(moves); i++)
        {
        movePoint(command, part1, knotsPart1, visitedPointsPart1);
        movePoint(command, part2, knotsPart2, visitedPointsPart2);
        }

        
    }
    std::cout << "Answer Part 1: " << visitedPointsPart1.size() << std::endl;
    std::cout << "Answer Part 2: " << visitedPointsPart2.size() << std::endl;
    return 0;
}