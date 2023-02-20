#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <queue>
#include <iterator>

// using Path = std::unordered_set<std::string, NodeHasher>;

struct valveDistanceHash
{
    std::size_t operator()(const std::string &valve) const
    {
        //return (valve[0] - 'A') * 26 + (valve[1] - 'A');
        return std::hash<std::string>{}(valve);
    }
};

struct pathFlow
{
    std::unordered_set<std::string, valveDistanceHash> path;
    int flow;

    pathFlow(const std::unordered_set<std::string, valveDistanceHash> &p, const int f) : path(p), flow(f) {}
};

struct pathHash
{
    std::size_t operator()(const std::unordered_set<std::string, valveDistanceHash> &path) const
    {
        std::size_t result = 1;
        for (const auto &v : path)
        {
            //result = result * 26 + (v[0] - 'A') * 26 + (v[1] - 'A');
            result *= std::hash<std::string>{}(v);
        }

        return result;
    }
};

struct valveDistance
{
    std::string valve;
    int distance;

    valveDistance(const std::string &valve, int distance) : valve(valve), distance(distance) {}

    valveDistance &operator=(const valveDistance &other) = default;

    bool operator<(const valveDistance &other) const
    {
        return this->distance < other.distance;
    }

    bool operator>(const valveDistance &other) const
    {
        return this->distance > other.distance;
    }

    bool operator==(const valveDistance &other) const
    {
        return this->valve == other.valve;
    }
};
struct valveComparator
{
    bool operator()(const valveDistance &lhs, const valveDistance &rhs)
    {
        return lhs.distance > rhs.distance;
    }
};

std::unordered_map<std::string, int> findDistances(
    std::unordered_map<std::string, std::vector<std::string>> &valveMap,
    const std::string sourceValve)
{
    std::unordered_map<std::string, int> distances;
    std::priority_queue<valveDistance, std::vector<valveDistance>, valveComparator> valveQueue;
    std::unordered_set<std::string> visitedValves;
    valveQueue.push(valveDistance(sourceValve, 0));

    while (!valveQueue.empty())
    {
        const auto currentValve = valveQueue.top();
        valveQueue.pop();

        if (visitedValves.find(currentValve.valve) != visitedValves.end())
        {
            continue;
        }

        visitedValves.insert(currentValve.valve);
        distances[currentValve.valve] = currentValve.distance;

        for (const auto &nextValve : valveMap[currentValve.valve])
        {
            valveQueue.push(valveDistance(nextValve, currentValve.distance + 1));
        }
    }

    return distances;
}

void depthFirstSearch(std::vector<std::string> &path, std::vector<std::vector<std::string>> &paths,
                      std::unordered_set<std::string> &visitedValves, std::unordered_map<std::string, int> &flowRates,
                      std::unordered_map<std::string, std::vector<std::string>> &valveMap,
                      const std::string &currentValve, const int time,
                      std::unordered_map<std::string, std::unordered_map<std::string, int>> &valveDistances)
{
    if (time == 0)
    {
        paths.push_back(path);
        return;
    }

    for (const auto &nextValve : valveDistances[currentValve])
    {
        if (visitedValves.find(nextValve.first) != visitedValves.end())
        {
            continue;
        }

        if (time - nextValve.second - 1 < 0)
        {
            paths.push_back(path);
            continue;
        }

        if (flowRates[nextValve.first] == 0)
        {
            continue;
        }

        visitedValves.insert(nextValve.first);
        path.push_back(nextValve.first);
        depthFirstSearch(path, paths, visitedValves, flowRates, valveMap, nextValve.first, time - nextValve.second - 1, valveDistances);
        visitedValves.erase(nextValve.first);
        path.pop_back();
    }
    paths.push_back(path);
    return;
}

void depthFirstSearch2(std::unordered_set<std::string, valveDistanceHash> &path,
                      std::unordered_set<std::string>& visitedValves,
                      std::unordered_map<std::unordered_set<std::string, valveDistanceHash>, int, pathHash>& paths,
                      std::unordered_map<std::string, int>& flowRates,
                      std::unordered_map<std::string, std::vector<std::string>>& valveMap,
                      const std::string &currentValve, const int time,
                      std::unordered_map<std::string, std::unordered_map<std::string, int>> &valveDistances,
                      int flow = 0)
{
    auto itr = paths.find(path);
    if (time == 0)
    {
        if(itr == paths.end() || itr->second < flow)
        {
            paths[path] = flow;
        }
        return;
    }

    flow += flowRates[currentValve] * time;

    for(const auto& valve : valveDistances[currentValve])
    {
        if(visitedValves.find(valve.first) != visitedValves.end()) { continue; }

        if(time - valve.second - 1 < 0)
        {
            auto itr = paths.find(path);
            if(itr == paths.end() || itr->second < flow)
            {
                paths[path] = flow;
            }
            continue;
        }

        if(flowRates[valve.first] == 0) { continue; }

        visitedValves.insert(valve.first);
        path.insert(valve.first);
        depthFirstSearch2(path, visitedValves, paths, flowRates, valveMap, valve.first, time - valve.second - 1, valveDistances, flow);
        visitedValves.erase(valve.first);
        path.erase(valve.first);
    }
    if(itr == paths.end() || itr->second < flow)
    {
        paths[path] = flow;
    }

    flow -= flowRates[currentValve] * time;
    return;
}

std::vector<std::vector<std::string>> getAllPaths(const std::string &sourceValve, const int time,
                                                  std::unordered_map<std::string, std::unordered_map<std::string, int>> &valveDistances,
                                                  std::unordered_map<std::string, int> flowRates,
                                                  std::unordered_map<std::string, std::vector<std::string>> &valveMap)
{
    std::vector<std::vector<std::string>> paths;
    std::vector<std::string> path{sourceValve};
    std::unordered_set<std::string> visitedValves;
    visitedValves.insert(sourceValve);

    depthFirstSearch(path, paths, visitedValves, flowRates, valveMap, sourceValve, time, valveDistances);

    return paths;
}

int calcFlow(const std::vector<std::string> &path, std::unordered_map<std::string, int> &flowRates,
             std::unordered_map<std::string, std::unordered_map<std::string, int>> &valveDistances, int time)
{
    int score = 0;
    for (int i = 0; i < (int)path.size() - 1; i++)
    {
        const auto prevPath = path[i];
        const auto nextPath = path[i + 1];

        time = time - (valveDistances[prevPath][nextPath]) - 1;
        score += flowRates[nextPath] * time;
    }

    return score;
}

std::unordered_map<std::unordered_set<std::string, valveDistanceHash>, int, pathHash> getAllPaths
( std::unordered_map<std::string, std::unordered_map<std::string, int>> &valveDistances,
  std::unordered_map<std::string, int> flowRates,
  std::unordered_map<std::string, std::vector<std::string>> &valveMap,
  const std::string& sourceValve,
  const int time)
{
    std::unordered_map<std::unordered_set<std::string, valveDistanceHash>, int, pathHash> paths;
    std::unordered_set<std::string, valveDistanceHash> path;

    std::unordered_set<std::string> visitedValves;
    visitedValves.insert(sourceValve);
    depthFirstSearch2(path, visitedValves, paths, flowRates, valveMap, sourceValve, time, valveDistances, 0);
    return paths;


}
void answerPart1(std::vector<std::string> &valves,
                 std::unordered_map<std::string, int> &flowRates,
                 std::unordered_map<std::string, std::vector<std::string>> &valveMap)
{
    std::unordered_map<std::string, std::unordered_map<std::string, int>> valveDistances;
    for (const auto &valve : valves)
    {
        valveDistances[valve] = findDistances(valveMap, valve);
    }

    const std::string startValve = "AA";
    const int time = 30;

    const auto paths = getAllPaths(startValve, time, valveDistances, flowRates, valveMap);

    int maxFlow = 0;

    // int maxPath = -1;

    for (int i = 0; i < (int)paths.size(); i++)
    {

        const auto newFlow = calcFlow(paths[i], flowRates, valveDistances, time);
        if (newFlow > maxFlow)
        {
            // maxPath = i;
            maxFlow = newFlow;
        }

    }

    std::cout << "Answer to Part 1: " << maxFlow << std::endl;
}

void answerPart2(std::vector<std::string> &valves,
                 std::unordered_map<std::string, int> &flowRates,
                 std::unordered_map<std::string, std::vector<std::string>> &valveMap)
{
    using namespace std;
    std::unordered_map<std::string, std::unordered_map<std::string, int>> valveDistances;
    for (const auto &valve : valves)
    {
        valveDistances[valve] = findDistances(valveMap, valve);
    }

    const std::string startValve = "AA";
    const int time = 26;

    std::unordered_map<std::unordered_set<std::string, valveDistanceHash>, int, pathHash> paths = getAllPaths(valveDistances, flowRates, valveMap, startValve, time);
    bool noIntersect = true;
    int maxFlow = 0;



    for(const auto &p1 : paths)
    {

        for(const auto &p2 : paths)
        {
            noIntersect = true;
            for(const auto &v : p1.first)
            {
                if(p2.first.find(v) != p2.first.end())
                {
                    noIntersect = false;
                    break;
                }
            }

            if(noIntersect)
            {
                maxFlow = std::max(maxFlow, p1.second + p2.second);
            }
        }

    }

    std::cout << "Answer to Part 2: " << maxFlow << std::endl;
}

void readFile(std::string &input, bool part2)
{
    std::fstream inputFile(input);
    std::string line;

    std::vector<std::string> valves;
    std::unordered_map<std::string, int> flowRates;
    std::unordered_map<std::string, std::vector<std::string>> valveMap;

    while (std::getline(inputFile, line))
    {
        auto delimPos = line.find(';');
        auto half1 = line.substr(0, delimPos);

        std::string valve = half1.substr(6, 2);
        valves.push_back(valve);

        auto eqPos = half1.find('=');
        int flow = stoi(half1.substr(eqPos + 1, delimPos - eqPos - 1));
        flowRates[valve] = flow;

        auto half2 = line.substr(delimPos + 1, line.size() - delimPos);
        auto firstComma = half2.find_first_of(',');
        std::vector<std::string> valveConnects;

        if (firstComma == std::string::npos)
        {
            valveMap[valve] = std::vector<std::string>{half2.substr(half2.size() - 2, 2)};
            continue;
        }
        else
        {
            std::string connectingValve;
            half2 = half2.substr(firstComma - 2);

            for (auto ch : half2)
            {
                if (ch == ' ')
                {
                    continue;
                }
                if (ch == ',')
                {
                    valveConnects.push_back(connectingValve);
                    connectingValve.clear();
                    continue;
                }

                connectingValve += ch;
            }
            valveConnects.push_back(connectingValve);
            valveMap[valve] = valveConnects;
        }
    }

    if (!part2)
    {
        answerPart1(valves, flowRates, valveMap);
    }
    else
    {
        answerPart2(valves, flowRates, valveMap);
    }
}

int main()
{
    std::string input = "./input.txt";

    readFile(input, false);
    readFile(input, true); //true == Part2
    return 0;
}