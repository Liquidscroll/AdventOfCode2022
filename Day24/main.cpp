#include <iostream>
#include <string>
#include <fstream>
#include <numeric>
#include <algorithm>
#include <set>
#include <queue>
#include <functional>
#include <vector>

struct Coord
{
    int x, y, z;
};

bool operator<(const Coord& a, const Coord& b)
{
    return std::tuple(a.x, a.y, a.z) < std::tuple(b.x, b.y, b.z);
}

Coord operator+(const Coord& a, const Coord& b)
{
    return {a.x + b.x, a.y + b.y, a.z + b.z};
}

struct Blizzard
{
    char direction;
    int x, y;
};

bool operator<(const Blizzard& a, const Blizzard& b)
{
    return std::tuple(a.x, a.y) < std::tuple(b.x, b.y);
}

struct Valley
{
    std::multiset<Blizzard> blizzards;
    int width = 0;
    int height = 0;

    bool coordInArea(const Coord& coord)
    {
        if((coord.x == source().x && coord.y == source().y) || (coord.x == destination().x && coord.y == destination().y))
        {
            return true;
        }
        return coord.x > 0 && coord.x < width - 1 && coord.y > 0 && coord.y < height - 1;
    }

    bool validCoord(const Coord& coord)
    {
        return coordInArea(coord) && !blizzards.count({' ', coord.x, coord.y});
    }

    Coord source() const { return {1, 0, 0}; }
    Coord destination() const { return {width - 2, height - 1, 0 }; }
};

bool isBlizzardChar(char c)
{
    return c == '>' || c == 'v' || c == '<' || c == '^';
}

Valley readFile(const std::string& input)
{
    std::ifstream inputFile(input);
    std::string line;

    Valley valley;

    while(std::getline(inputFile, line))
    {

        for(int x = 0; x < (int)line.size(); x++)
        {
            if(isBlizzardChar(line[x]))
            {
                valley.blizzards.insert({line[x], x, valley.height});
            }
        }
        valley.width = std::max(valley.width, (int)line.size());
        valley.height++;
    }

    return valley;
}

int nextTile(int i, int end)
{
    if(i >= end - 1)
    {
        return 1;
    }
    else if (i <= 0)
    {
        return end - 2;
    }
    else
    {
        return i;
    }
}

Valley updateValley(const Valley& valley)
{
    Valley returnValley;
    returnValley.width = valley.width;
    returnValley.height = valley.height;
    for(auto& blizz : valley.blizzards)
    {
        switch(blizz.direction)
        {
            case '<':
                returnValley.blizzards.insert({'<', nextTile(blizz.x - 1, returnValley.width), blizz.y});
                break;
            case '>':
                returnValley.blizzards.insert({'>', nextTile(blizz.x + 1, returnValley.width), blizz.y});
                break;
            case '^':
                returnValley.blizzards.insert({'^', blizz.x, nextTile(blizz.y - 1, returnValley.height)});
                break;
            case 'v':
                returnValley.blizzards.insert({'v', blizz.x, nextTile(blizz.y + 1, returnValley.height)});
                break;
        };
    }
    return returnValley;
}

int breadthFirstSearch(std::vector<Valley>& valleys, const Coord& source, const Coord& dest)
{
    std::set<Coord> visitedCoord;
    std::queue<Coord> coordQ;

    coordQ.push(source);

    while(!coordQ.empty())
    {

        auto currCoord = coordQ.front();
        coordQ.pop();

        if(currCoord.x == dest.x && currCoord.y == dest.y)
        {
            return currCoord.z;
        }

        if(visitedCoord.count(currCoord))
        {
            continue;
        }

        visitedCoord.insert(currCoord);

        if(currCoord.z + 1 >= (int)valleys.size())
        {
            valleys.push_back(updateValley(valleys.back()));
        }

        for(auto& c : std::vector<Coord>{ {0, 0, 1}, {-1, 0, 1},
                                          {1, 0, 1}, {0, -1, 1},
                                          {0, 1, 1} })
        {
            Coord newCoord = currCoord + c;
            if (valleys[newCoord.z].validCoord(newCoord) && !visitedCoord.count(newCoord))
            {
                coordQ.push(newCoord);
            }
        }
    }

    return -1;
}

void answerPart1(const Valley& valley)
{
    std::vector<Valley> valleys { valley };
    int ans = breadthFirstSearch(valleys, valley.source(), valley.destination());
    std::cout << "Answer to Part 1: " << ans << std::endl;
}

void answerPart2(const Valley& valley)
{
    std::vector<Valley> valleys{valley};

    auto firstCrossing = breadthFirstSearch(valleys, valley.source(), valley.destination());
    auto secondCrossing = breadthFirstSearch(valleys, {valley.destination().x, valley.destination().y, firstCrossing }, valley.source());
    auto thirdCrossing = breadthFirstSearch(valleys, {valley.source().x, valley.source().y, secondCrossing }, valley.destination());
    std::cout << "Answer to Part 2: " << thirdCrossing << std::endl;
}


int main() {
    std::string input = "input.txt";

    Valley valleyPart1 = readFile(input);
    answerPart1(valleyPart1);
    answerPart2(valleyPart1);
    return 0;
}
