#include <iostream>
#include <fstream>
#include <array>
#include <vector>
#include <set>
#include <unordered_map>
#include <bitset>
#include <cstdlib>
#include <thread>


const int gridCol = 7;

using coord = std::pair<long long, long long>;
using namespace std::chrono_literals;

struct gridMap
{
    std::vector<std::bitset<gridCol>> map;
};

struct blockShape
{
    int rows = 0;
    int cols = 0;
    std::vector<bool> blocks;
};

bool detectCollision(const gridMap& map, const blockShape& shape, const coord& pos)
{
    if(pos.second < 0)
    {
        return true;
    }
    if(pos.first < 0)
    {
        return true;
    }
    if(pos.first > gridCol - shape.cols)
    {
        return true;
    }
    for(int i = 0; i < (int)shape.blocks.size(); i++)
    {
        if(!shape.blocks[i])
        {
            continue;
        }
        int row = pos.second + (i / shape.cols);
        int col = pos.first + (i % shape.cols);
    //check this??
        if(map.map[row][gridCol - col - 1])
        {
            return true;
        }
    }
    return false;
}

void writeRock(gridMap& map, const blockShape& shape, const coord& pos)
{
    for(int i = 0; i < (int)shape.blocks.size(); i++)
    {
        if(!shape.blocks[i])
        {
            continue;
        }
        int row = pos.second + (i / shape.cols);
        int col = pos.first + (i % shape.cols);
        map.map[row][gridCol - col - 1] = true;
    }
}

void drawMap(const gridMap& map)
{
    for(size_t row = map.map.size(); row != 0; row--)
    {
        std::cout << "|";
        for(int col = 0; col < gridCol; col++)
        {
            if(map.map[row-1][gridCol - col - 1])
            {
                std::cout << "#";
            }
            else
            {
                std::cout << ".";
            }
        }
        std::cout << "|" << std::endl;
    }
    std::cout << "+-------+" << std::endl;
}

int findHighestBlock(const gridMap& map)
{
    for (size_t row = map.map.size(); row != 0; row--)
    {
        //check this??

        if(map.map[row - 1] != 0)
        {
            return row - 1;
        }
    }
    return 0;
}

int main()
{

    std::ifstream inputFile("input.txt");


    std::string line;
    inputFile >> line;

    std::size_t numDirection = line.length();

    const int numShapes = 5;
    std::array<blockShape, numShapes> shapes;

    shapes[0].rows = 1;
    shapes[0].cols = 4;
    shapes[0].blocks = {1, 1, 1, 1};

    shapes[1].rows = 3;
    shapes[1].cols = 3;
    shapes[1].blocks = {0, 1, 0, 1, 1, 1, 0, 1, 0};

    shapes[2].rows = 3;
    shapes[2].cols = 3;
    shapes[2].blocks = {1, 1, 1, 0, 0, 1, 0, 0, 1};

    shapes[3].rows = 4;
    shapes[3].cols = 1;
    shapes[3].blocks = {1, 1, 1, 1};

    shapes[4].rows = 2;
    shapes[4].cols = 2;
    shapes[4].blocks = {1, 1, 1, 1};

    gridMap map;



    std::unordered_map<std::string, int> mapKeys;
    std::string repeatKey;

    int currHeight = 0;
    coord pos{2, currHeight + 3};
    coord prev_pos{2, currHeight + 3};

    int dirCounter = 0;
    std::vector<int> heights;
    int rock;

    for(rock = 0; rock < 10000; rock++)
    {
        while((int)map.map.size() < currHeight + 10)
        {
            map.map.push_back(0);
        }

        int blockIdx = rock % numShapes;

        while(1)
        {
            char direction = line[dirCounter % numDirection];
            dirCounter = (dirCounter + 1) % numDirection;
            if(direction == '>')
            {
                pos.first += 1;
            }
            else
            {
                pos.first -= 1;
            }

            if(detectCollision(map, shapes[blockIdx], pos))
            {
                pos = prev_pos;
            }
            else
            {
                prev_pos = pos;
            }

            pos.second -= 1;

            if(detectCollision(map, shapes[blockIdx], pos))
            {
                writeRock(map, shapes[blockIdx], prev_pos);
                break;
            }
            else
            {
                prev_pos = pos;
            }
        }

        currHeight = findHighestBlock(map) + 1;
        heights.push_back(currHeight);



        int searchRepetition = 20;
        if(currHeight > searchRepetition)
        {
            std::string key;
            key = std::to_string(blockIdx) + "_" + std::to_string(dirCounter) + "_";
            for(int row = 0; row < searchRepetition;row++)
            {
                key += map.map[findHighestBlock(map) - row].to_string();
            }
            if(mapKeys.find(key) == mapKeys.end())
            {
                mapKeys[key] = rock;
            }
            else
            {
                repeatKey = key;
                break;
            }
        }

        if(repeatKey != "")
        {
            break;
        }

        pos.first = 2;
        pos.second = currHeight + 3;
        //drawMap(map);
    }

    if(repeatKey == "")
    {
        throw std::runtime_error("No repeating section was found.");
    }

    long long rocksDropped = rock - mapKeys[repeatKey];
    long long heightDiff = currHeight - heights[mapKeys[repeatKey]];


    long long numRocks = 2022;
    long long start = mapKeys[repeatKey];
    long long repeats = (numRocks - start) / rocksDropped;
    long long height = heights[numRocks - rocksDropped * repeats - 1] + repeats * heightDiff;


    std::cout << "Answer to Part 1: " << height << std::endl;
    numRocks = 1000000000000;
    start = mapKeys[repeatKey];
    repeats = (numRocks - start) / rocksDropped;
    height = heights[numRocks - rocksDropped * repeats - 1] + repeats * heightDiff;

    std::cout << "Answer to Part 2: " << height << std::endl;

    return 0;
}
