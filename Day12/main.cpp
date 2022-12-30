#include <fstream>
#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include <numeric>
#include <queue>

struct node
{
    int x = -1, y = -1;
    int cost = 0;

    bool operator> (node const& otherNode) const { return cost > otherNode.cost; }
    bool operator== (node const& otherNode) const { return x == otherNode.x && y == otherNode.y;}


};

static std::ostream& operator<< (std::ostream& os, node const& n)
{
    os << "(" << n.x << "," << n.y << ")";
    return os;
}
static std::vector<node> getNodeNeighbours(int currentX, int currentY, std::vector<std::string> const& heights)
{
    std::vector<node> neighbours{};

    int nodeLeft = currentX - 1;
    int nodeRight = currentX + 1;
    int nodeAbove = currentY - 1;
    int nodeBelow = currentY + 1;

    char currentHeight = heights[currentY][currentX];

    if(nodeLeft >= 0 &&
        heights[currentY][nodeLeft] <= (currentHeight + 1))
    {
        neighbours.push_back({.x = nodeLeft, .y = currentY});
    }
    if(nodeAbove >= 0 &&
        heights[nodeAbove][currentX] <= (currentHeight + 1))
    {
        neighbours.push_back({.x = currentX, .y = nodeAbove});
    }
    if(nodeRight < heights[0].size() &&
        heights[currentY][nodeRight] <= (currentHeight + 1))
    {
        neighbours.push_back({.x = nodeRight, .y = currentY});
    }
    if(nodeBelow < heights.size() &&
        heights[nodeBelow][currentX] <= (currentHeight + 1))
    {
        neighbours.push_back({.x = currentX, .y = nodeBelow});
    }

    return neighbours;
}

node answerPart1(node const& start, node const& end, std::vector<std::string> const& heights)
{
    std::vector<node> visited{};
    std::vector<node> pushed{};

    std::priority_queue<node, std::vector<node>, std::greater<node>> toVisit;
    toVisit.push(start);

    auto alreadyVisited = [](node const& loc)
            { return [&loc] (node n) { return loc.x == n.x && loc.y == n.y; }; };
    auto alreadyPushed = [](node const& loc)
            { return [&loc] (node n) { return loc.x == n.x && loc.y == n.y; }; };

    while(!toVisit.empty())
    {
        auto currentLoc = toVisit.top();
        toVisit.pop();

        visited.push_back(currentLoc);

        auto possibleNeighbours = getNodeNeighbours(currentLoc.x, currentLoc.y, heights);

        for(node n : possibleNeighbours)
        {
            if(auto ptr = std::ranges::find_if(visited, alreadyVisited(n)); ptr != visited.end())
            {
                continue;
            }
            if(auto ptr = std::ranges::find_if(pushed, alreadyPushed(n)); ptr != pushed.end())
            {
                continue;
            }

            n.cost = currentLoc.cost + 1;
            toVisit.push(n);
            pushed.push_back(n);
        }
    }

    auto itr = std::ranges::find_if(visited, [&end](node const& loc)
    { return loc==end; });
    if(itr != visited.end())
    {
        return *itr;
    }

}

node answerPart2(node const& end, std::vector<std::string> const& heightMap)
{
    std::vector<node> locations{};

    for(int row = 0; row < heightMap.size(); row++)
    {
        for(int col = 0; col < heightMap[row].size(); col++)
        {
            if(heightMap[row][col] == 'a')
            {
                locations.push_back({.x = col, .y = row, .cost = 0});
            }
        }
    }
    std::vector<node> answerVec;
    for(node const& start : locations)
    {
    answerVec.push_back(answerPart1(start, end, heightMap));
    }
    node final = answerVec[0];
    for(node const& ans : answerVec)
    {
        if (ans.cost < final.cost)
        {
            final = ans;
        }
    }

    return final;
}

int main()
{
    std::ifstream inputFile("input.txt");
    if(inputFile.is_open()) std::cout << "File Opened" << std::endl;

    std::vector<std::string> heightMap;

    std::string line;
    node start;
    node end;
    while(inputFile)
    {
        inputFile >> line;

        heightMap.push_back(line);
        if(size_t pos = line.find_first_of('S'); pos != std::string::npos)
        {
            start = {.x = static_cast<int>(pos), .y = static_cast<int>(heightMap.size() - 1) };
            heightMap.back()[pos] = 'a';
        }

        if(size_t pos = line.find_first_of('E'); pos != std::string::npos)
        {
            end = { .x = static_cast<int>(pos), .y = static_cast<int>(heightMap.size() - 1 )};
            heightMap.back()[pos] = 'z';
        }
    }

    node ans1 = answerPart1(start, end, heightMap);
    std::cout << "\nAnswer to Part 1: " << ans1.cost << std::endl;

    node ans2 = answerPart2(end, heightMap);
    std::cout << "Answer to Part 2: " << ans2.cost << std::endl;
    return 0;
}