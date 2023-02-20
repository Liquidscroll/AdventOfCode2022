#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <array>
#include <set>
#include <queue>
#include <tuple>
#include <algorithm>
#include <numeric>
#include <functional>


struct lavaCoord
{
    int x, y, z;

    lavaCoord operator+( const lavaCoord& b)
    {
        return {this->x + b.x, this->y + b.y, this->z+b.z};
    }

    bool operator<(const lavaCoord& b) const
    {
        return std::tuple(this->x, this->y, this->z) < std::tuple(b.x, b.y, b.z);
    }

    bool operator>(const lavaCoord& b) const
    {
        return std::tuple(this->x, this->y, this->z) > std::tuple(b.x, b.y, b.z);
    }

    lavaCoord minimum(const lavaCoord& b)
    {
        return {std::min(this->x, b.x), std::min(this->y, b.y), std::min(this->z, b.z)};
    }

    lavaCoord maximum(const lavaCoord& b)
    {
        return { std::max(this->x, b.x), std::max(this->y, b.y), std::max(this->z, b.z)};
    }
};



struct lavaDrop
        {
    lavaCoord coord;

    int area = 6;

    lavaDrop(int x, int y, int z)
    {
        coord.x = x;
        coord.y = y;
        coord.z = z;
    }

    void printDrop()
    {
        std::cout << "(" << coord.x
                  << "," << coord.y
                  << "," << coord.z
                  << ")"
                  << " -- Area: " << area
                  << std::endl;
    }

    void cubeDist(lavaDrop& drop)
    {
        int dx = std::abs(drop.coord.x - coord.x);
        int dy = std::abs(drop.coord.y - coord.y);
        int dz = std::abs(drop.coord.z - coord.z);

        if(dx == 1 && dy == 0 && dz == 0)
        {
            this->area -= 1;
        }
        if(dy == 1 && dx == 0 && dz == 0)
        {
            this->area -= 1;
        }
        if(dz == 1 && dx == 0 && dy == 0)
        {
            this->area -= 1;
        }
    }

    bool cubePresent(const int& x, const int& y, const int& z)
    {
        if(x == this->coord.x && y == this->coord.y && z == this->coord.z)
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    bool isExterior(int minX, int maxX, int minY, int maxY, int minZ, int maxZ)
    {
        if(coord.x == minX || coord.x == maxX
        || coord.y == minY || coord.y == maxY
        || coord.z == minZ || coord.z == maxZ)
        {
            return true;
        }
        return false;
    }

    bool operator== (const lavaDrop& other)
    {
        return (this->coord.x == other.coord.x
                && this->coord.y == other.coord.y
                && this->coord.z == other.coord.z);
    }
        };

struct lavaMap
{
    std::vector<lavaDrop> droplets;

    bool findDrop(lavaDrop drop) const
    {
        for(auto d : droplets)
        {
            if(d == drop)
            {
                return true;
            }
        }

        return false;
    }
};

int pathFindingDFS(lavaCoord coord, int maxX, int maxY, int maxZ,
                  std::vector<std::vector<std::vector<bool>>> lava,
                  std::vector<std::vector<std::vector<bool>>> visited)
{
    if(coord.x < 0 || coord.x > maxX ||
       coord.y < 0 || coord.y > maxY ||
       coord.z < 0 || coord.z > maxZ)
    {
        return 1;
    }

    if(visited[coord.z][coord.y][coord.x]) { return 0; }

    visited[coord.z][coord.y][coord.x] = true;



    const lavaCoord exits[6] ={
            {coord.x + 1, coord.y, coord.z},
            {coord.x - 1, coord.y, coord.z},
            {coord.x, coord.y + 1, coord.z},
            {coord.x, coord.y - 1, coord.z},
            {coord.x, coord.y, coord.z + 1},
            {coord.x, coord.y, coord.z - 1}
            };

    for(int i = 0; i < 6; i++)
    {
        const int x = exits[i].x;
        const int y = exits[i].y;
        const int z = exits[i].z;

        if(x < 0 || x > maxX ||
           y < 0 || y > maxY ||
           z < 0 || z > maxZ ||
                (!visited[z][y][x] && !lava[z][y][x]))
        {
			lavaCoord lc = {x, y, z};
            const int thisExit = pathFindingDFS(lc,
                                                maxX, maxY, maxZ,
                                                lava,
                                                visited);

            if(thisExit) { return 1; }
        }
    }

    return 0;
}

bool insideBox(const lavaCoord& pos, const lavaCoord& minPos, const lavaCoord& maxPos)
{
    return pos.x >= minPos.x && pos.x < maxPos.x
          && pos.y >= minPos.y && pos.y < maxPos.y
          && pos.z >= minPos.z && pos.z < maxPos.z;
}
std::set<lavaCoord> floodFill(const lavaMap& map,
                              const lavaCoord& pos,
                              const lavaCoord& minPos,
                              const lavaCoord& maxPos)
{
    std::set<lavaCoord> result;

    std::queue<lavaCoord> q;
    q.push(pos);

    lavaCoord outsideMinPosition = {minPos.x - 1, minPos.y - 1, minPos.z - 1};
    lavaCoord outsideMaxPosition = {maxPos.x + 1, maxPos.y + 1, maxPos.z + 1};

    while(!q.empty())
    {
        lavaCoord curr = q.front();
        q.pop();

        if(result.count(curr))
        {
            continue;
        }

        result.insert(curr);

        std::vector<lavaCoord> surrounds = {{-1, 0, 0},
                                            {1, 0, 0},
                                            {0, -1, 0},
                                            {0, 1, 0},
                                            {0, 0, -1},
                                            {0, 0, 1}};

        for(auto& s : surrounds)
        {
            lavaCoord newPos = curr + s;
            if(!map.findDrop(lavaDrop{newPos.x, newPos.y, newPos.z})
            && !result.count(newPos) && insideBox(newPos, outsideMinPosition, outsideMaxPosition))
            {
                q.push(newPos);
            }
        }
    }

    return result;
}

int main() {
    std::ifstream inputFile("input.txt");

    std::string line;


    lavaMap map;

    while(inputFile >> line)
    {
        auto firstComma = line.find(',');
        auto lastComma = line.find_last_of(',');

        auto x = stoi(line.substr(0, firstComma));
        auto y = stoi(line.substr(firstComma + 1, lastComma - firstComma - 1));
        auto z = stoi(line.substr(lastComma + 1));

        map.droplets.emplace_back(x, y, z);
    }
    int maxX = 0, maxY = 0, maxZ = 0;
    int minX = map.droplets.begin()->coord.x;
    int minY = map.droplets.begin()->coord.y;
    int minZ = map.droplets.begin()->coord.z;

    for(auto d : map.droplets)
    {
        if(d.coord.x > maxX)
        {
            maxX = d.coord.x;
        }
        if(d.coord.x < minX)
        {
            minX = d.coord.x;
        }
        if(d.coord.y > maxY)
        {
            maxY = d.coord.y;
        }
        if(d.coord.y < minY)
        {
            minY = d.coord.y;
        }
        if(d.coord.z > maxZ)
        {
            maxZ = d.coord.z;
        }
        if(d.coord.z < minZ)
        {
            minZ = d.coord.z;
        }
    }

    for(auto& drop : map.droplets)
    {
        for(auto& drop2 : map.droplets)
        {
            drop.cubeDist(drop2);
        }
    }
    int answerPart1 = 0;
    for(auto drop : map.droplets)
    {
        answerPart1 += drop.area;
    }

    lavaCoord minPos = lavaCoord{minX - 1, minY - 1, minZ - 1};
    lavaCoord maxPos = lavaCoord {maxX + 1, maxY + 1, maxZ + 1};
    std::set<lavaCoord> result = floodFill(map, minPos, minPos, maxPos);

    int answerPart2 = 0;

    for(auto& pos : map.droplets)
    {
        for(auto& d : std::vector<lavaDrop>{{-1, 0, 0},
                                             {1, 0, 0},
                                             {0, -1, 0},
                                             {0, 1, 0},
                                             {0, 0, -1},
                                             {0, 0, 1}})
        {
            lavaDrop newPos {pos.coord.x + d.coord.x,
                             pos.coord.y + d.coord.y,
                             pos.coord.z + d.coord.z};

            answerPart2 += ! map.findDrop(newPos) && result.count(newPos.coord);
        }
    }



    std::cout << "Answer to Part 1: " << answerPart1 << std::endl;
    std::cout << "Answer to Part 2: " << answerPart2 << std::endl;
    return 0;
}
