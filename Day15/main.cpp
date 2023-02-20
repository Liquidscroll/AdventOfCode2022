#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <limits>
#include <algorithm>
#include <unordered_set>




struct pos
{
    int x, y;
};

struct beaconPair
{
    pos sensor, beacon;
};

std::vector<beaconPair> readFile(std::string& input)
{
    std::fstream inputFile(input);
    std::string line;

    std::vector<beaconPair> result;

    while(std::getline(inputFile, line))
    {
        std::string half1 = line.substr(0, line.find(':'));
        std::string half2 = line.substr(line.find(':') + 1);

        int x = stoi(half1.substr(half1.find_first_of('=') + 1, (half1.find_first_of(',') - half1.find_first_of('='))));
        int y = stoi(half1.substr(half1.find_last_of('=') + 1));

        int x2 = stoi(half2.substr(half2.find_first_of('=') + 1, (half1.find_first_of(',') - half1.find_first_of('='))));
        int y2 = stoi(half2.substr(half2.find_last_of('=') + 1));

        result.push_back({{x, y}, {x2, y2}});

    }

    return result;

}

bool checkCover(const std::vector<std::pair<pos, int>>& coverage, int x, int y)
{
    constexpr int maxPos = 4000000;
    if(x < 0 || x > maxPos || y < 0 || y > maxPos)
    {
        return true;
    }

    for(const auto& c: coverage)
    {
        int distance = std::abs(c.first.x - x) + std::abs(c.first.y - y);
        if(distance <= c.second)
        {
            return true;
        }
    }

    return false;
}

pos intersect(int a, int a1, int b, int b1)
{
    int x = (b1 - a1) / (a - b);
    int y = a * x + a1;
    return {x, y};
}

void answerPart1(std::vector<beaconPair>& pairs)
{
    constexpr int yTarget = 2000000;

    int minX = INT_MAX;
    int maxX = INT_MIN;

    for(const auto& p : pairs)
    {
        int dist = std::abs(p.sensor.x - p.beacon.x) + std::abs(p.sensor.y - p.beacon.y);
        int yDiff = std::abs(p.sensor.y - yTarget);


        if(yDiff < dist)
        {
            minX = std::min(minX, p.sensor.x - (dist - yDiff));
            maxX = std::max(maxX, p.sensor.x + (dist - yDiff));
        }
    }
    int beacons = 0;
    std::unordered_set<int> inc;
    for(const auto& p : pairs)
    {
        if(p.beacon.y == yTarget && p.beacon.x >= minX && p.beacon.x <= maxX)
        {
            int64_t h = ((int64_t)p.beacon.x << 32) | (int64_t)p.beacon.y;
            auto itr = inc.find(h);
            if(itr != inc.end())
            {
                continue;
            }
            inc.insert(h);
            beacons++;
        }

    }

    auto ans = std::to_string(maxX - minX - beacons + 1);
    std::cout << "Answer Part1: " << ans << std::endl;
    return;
}


void answerPart2(std::vector<beaconPair>& pairs)
{
    std::vector<std::pair<pos, int>> coverage;
    pos ans;
    for(const auto& p : pairs)
    {
        int dist = std::abs(p.sensor.x - p.beacon.x) + std::abs(p.sensor.y - p.beacon.y);
        coverage.push_back({p.sensor, dist});
    }
    bool brk = false;
    for(size_t i = 0; i < coverage.size(); i++)
    {
        if(brk) { break; }
        for(size_t j = i + 1; j < coverage.size(); j++)
        {

            auto a = coverage[i].first;
            auto da = coverage[i].second;
            auto b = coverage[j].first;
            auto db = coverage[j].second;

            int dist = std::abs(a.x - b.x) + std::abs(a.y - b.y);
            if(dist - 1 > da + db)
            {
                continue;
            }

            pos p;
            p = intersect(1, -(a.x - da) + a.y, -1, (b.x - db) + b.y);
            if(!checkCover(coverage, p.x - 1, p.y))
            {
                ans = p;
                brk = true;
                break;
            }

            p = intersect(1, -(a.x - da) + a.y, -1, (b.x + db) + b.y);
            if(!checkCover(coverage, p.x, p.y + 1))
            {
                ans = p;
                brk = true;
                break;
            }

            p = intersect(1, -(a.x + da) + a.y, -1, (b.x - db) + b.y);
            if (!checkCover(coverage, p.x, p.y - 1))
            {
                ans = p;
                brk = true;
                break;
            }

            p = intersect(1, -(a.x + da) + a.y, -1, (b.x + db) + b.y);
            if (!checkCover(coverage, p.x + 1, p.y))
            {
                ans = p;
                brk = true;
                break;
            }

            p = intersect(-1, (a.x + da) + a.y, 1, -(b.x + db) + b.y);
            if (!checkCover(coverage, p.x + 1, p.y))
            {
                ans = p;
                brk = true;
                break;
            }

            p = intersect(-1, (a.x + da) + a.y, 1, -(b.x - db) + b.y);
            if (!checkCover(coverage, p.x, p.y + 1))
            {
                ans = p;
                brk = true;
                break;
            }

            p = intersect(-1, (a.x - da) + a.y, 1, -(b.x + db) + b.y);
            if (!checkCover(coverage, p.x, p.y - 1))
            {
                ans = p;
                brk = true;
                break;
            }

            p = intersect(-1, (a.x - da) + a.y, 1, -(b.x - db) + b.y);
            if (!checkCover(coverage, p.x - 1, p.y))
            {
                ans = p;
                brk = true;
                break;
            }
        }
    }

    auto ans2 = std::to_string((long long)ans.x * 4000000 + (long long)ans.y);
    std::cout << "Answer Part2: " << ans2 << std::endl;
    return;

}

int main()
{
    std::string in = "./input.txt";
    auto bPairs = readFile(in);

    answerPart1(bPairs);
    answerPart2(bPairs);

    return 0;
}