#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <functional>
#include <unordered_set>
#include <unordered_map>

enum Direction
{
    north = 0,
    northEast,
    east,
    southEast,
    south,
    southWest,
    west,
    northWest,
    none
};

struct coord
{
    int x, y;
};

struct coordHash
{
    std::size_t operator()(const coord& pos) const
    {
        return std::hash<int>()(pos.x) ^ std::hash<int>()(pos.y);
    }
};

struct resultData
{
    int emptyTiles;
    int round;
};

bool operator==(const coord& a, const coord& b)
{
    return std::tuple(a.x, a.y) == std::tuple(b.x, b.y);
}

coord operator+(const coord& a, const coord& b)
{
    return {a.x + b.x, a.y + b.y};
}

coord dirMove[8] { {0, -1}, {1, -1}, {1, 0}, {1, 1},
                   {0, 1}, {-1, 1}, {-1, 0}, {-1, -1}};

std::unordered_set<coord, coordHash> readFile(const std::string& input)
{
    std::unordered_set<coord, coordHash> result;

    std::ifstream inputFile(input);
    std::string line;
    int y = 0;
    while(std::getline(inputFile, line))
    {
        for(int x = 0; x < (int)line.size(); x++)
        {
            if(line[x] == '#')
            {
                result.insert({x, y});
            }
        }
        y++;
    }

    return result;
}

Direction checkPositions(const std::unordered_set<coord, coordHash>& elves, const coord& elf, int round)
{
    bool adjacent[8];
    for(int i = 0; i < 8; i++)
    {
        adjacent[i] = elves.count(elf + dirMove[i]);
    }

    if(std::none_of(adjacent, adjacent + 8, [](auto b){ return b;}))
    {
        return none;
    }

    std::function<Direction()> checks[] = {
            [&]{ return (!adjacent[north] && !adjacent[northEast] && !adjacent[northWest]) ? north : none; },
            [&]{ return (!adjacent[south] && !adjacent[southEast] && !adjacent[southWest]) ? south : none; },
            [&]{ return (!adjacent[west] && !adjacent[northWest] && !adjacent[southWest]) ? west : none; },
            [&]{ return (!adjacent[east] && !adjacent[northEast] && !adjacent[southEast]) ? east : none; }
    };

    for(int i = 0; i < 4; i++)
    {
        Direction result = checks[(round + i) % 4]();
        if(result != none)
        {
            return result;
        }
    }

    return none;
}

int numEmptyTiles(const std::unordered_set<coord, coordHash>& elves)
{
    coord minCoord = { INT_MAX, INT_MAX };
    coord maxCoord = { INT_MIN, INT_MIN };

    for(auto& pos : elves)
    {
        minCoord.x = std::min(pos.x, minCoord.x);
        minCoord.y = std::min(pos.y, minCoord.y);
        maxCoord.x = std::max(pos.x, maxCoord.x);
        maxCoord.y = std::max(pos.y, maxCoord.y);
    }

    int width = maxCoord.x - minCoord.x + 1;
    int height = maxCoord.y - minCoord.y + 1;

    return (width * height) - (int)elves.size();
}

resultData answerPart1(std::unordered_set<coord, coordHash>& elves)
{
    int round = 0;

    while(true)
    {
        std::unordered_map<coord, std::vector<coord>, coordHash> proposedMoves;
        for(auto& e : elves)
        {
            Direction proposedDir = checkPositions(elves, e, round);
            if(proposedDir != none)
            {
                proposedMoves[e + dirMove[proposedDir]].push_back(e);
            }
        }

        if(proposedMoves.empty())
        {
            break;
        }

        for(auto& [dest, sources] : proposedMoves)
        {
            if(sources.size() == 1)
            {
                elves.erase(sources.front());
                elves.insert(dest);
            }
        }

        round++;
        if(round == 10)
        {
            break;
        }
    }

    return resultData {numEmptyTiles(elves), round + 1};
}

resultData answerPart2(std::unordered_set<coord, coordHash>& elves)
{
    int round = 0;

    while(true)
    {
        std::unordered_map<coord, std::vector<coord>, coordHash> proposedMoves;
        for(auto& e : elves)
        {
            Direction proposedDir = checkPositions(elves, e, round);
            if(proposedDir != none)
            {
                proposedMoves[e + dirMove[proposedDir]].push_back(e);
            }
        }
        if(proposedMoves.empty())
        {
            break;
        }

        for(auto& [dest, sources] : proposedMoves)
        {
            if(sources.size() == 1)
            {
                elves.erase(sources.front());
                elves.insert(dest);
            }
        }

        round++;
    }

    return resultData {numEmptyTiles(elves), round + 1};
}

int main() {
    std::string input = "input.txt";
    auto elves = readFile(input);
    auto elves2 = readFile(input);
    auto ansPart1 = answerPart1(elves);
    auto ansPart2 = answerPart2(elves2);

    std::cout << "Answer to Part 1: " << ansPart1.emptyTiles << std::endl;
    std::cout << "Answer to Part 2: " << ansPart2.round << std::endl;
    return 0;
}
