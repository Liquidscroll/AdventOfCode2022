#include <iostream>
#include <fstream>
#include <array>
#include <vector>
#include <regex>

enum class Action
{
    WAIT,
    ORE,
    CLAY,
    OBSIDIAN,
    GEODE
};

struct ResourceValues
{
    int ore = 0, clay = 0, obsidian = 0, geodes = 0;

    ResourceValues operator-(const ResourceValues& other) const
    {
        auto ans = ResourceValues();
        ans.ore = this->ore - other.ore;
        ans.clay = this->clay - other.clay;
        ans.obsidian = this->obsidian - other.obsidian;
        ans.geodes = this->geodes - other.geodes;
        return ans;
    }

    bool operator==(const ResourceValues& other) const
    {
        return this->ore == other.ore
                && this->clay == other.clay
                && this->obsidian == other.obsidian
                && this->geodes == other.geodes;
    }

    bool operator<(const ResourceValues& other) const
    {
        return this->ore < other.ore
                && this->clay < other.clay
                && this->obsidian < other.obsidian
                && this->geodes < other.geodes;
    }

    bool operator>(const ResourceValues& other) const
    {
        return this->ore > other.ore
                && this->clay > other.clay
                && this->obsidian > other.obsidian
                && this->geodes > other.geodes;
    }

    bool operator>=(const ResourceValues& other) const
    {
        return this->ore >= other.ore
                && this->clay >= other.clay
                && this->obsidian >= other.obsidian
                && this->geodes >= other.geodes;
    }

    friend std::ostream& operator<<(std::ostream& os, const ResourceValues& resources);


};

std::ostream& operator<<(std::ostream& os, const ResourceValues& resources)
{
    os << "\nResources:\nOre: " << resources.ore
       << "\nClay: " << resources.clay
       << "\nObsidian: " << resources.obsidian
       << "\nGeodes: " << resources.geodes << std::endl;
    return os;
}

struct Blueprint
{
    int ID;
    ResourceValues oreRobotCost; //Ore
    ResourceValues clayRobotCost; //Clay
    ResourceValues obsidianRobotCost; //Ore && Clay
    ResourceValues geodeRobotCost; //Ore && Obsidian

    ResourceValues maxCostOfEachBot;

    int maxGeodes;

    void updateMaxCost()
    {
     const auto allCosts = std::vector<ResourceValues> { oreRobotCost,
                                                         clayRobotCost,
                                                         obsidianRobotCost,
                                                         geodeRobotCost };
        maxCostOfEachBot.ore = std::max_element(std::begin(allCosts), std::end(allCosts),
                                                [](const auto& res1, const auto& res2)
                                                {return res1.ore < res2.ore;} )->ore;
        maxCostOfEachBot.clay = std::max_element(std::begin(allCosts), std::end(allCosts),
                                                [](const auto& res1, const auto& res2)
                                                {return res1.clay < res2.clay;} )->clay;
        maxCostOfEachBot.obsidian = std::max_element(std::begin(allCosts), std::end(allCosts),
                                                    [](const auto& res1, const auto& res2)
                                                    {return res1.obsidian < res2.obsidian;} )->obsidian;
        maxCostOfEachBot.geodes = std::max_element(std::begin(allCosts), std::end(allCosts),
                                                  [](const auto& res1, const auto& res2)
                                                  {return res1.geodes < res2.geodes;} )->geodes;

    }

    friend std::ostream& operator<<(std::ostream& os, const Blueprint& bluePrint);
};

std::ostream& operator<<(std::ostream& os, const Blueprint& bluePrint)
{
    os << "Blueprint " << bluePrint.ID << ":" << std::endl
       << "Ore Bot Cost: " << bluePrint.oreRobotCost << std::endl
       << "Clay Bot Cost: " << bluePrint.clayRobotCost << std::endl
       << "Obsidian Bot Cost: " << bluePrint.obsidianRobotCost << std::endl
       << "Geode Bot Cost: " << bluePrint.geodeRobotCost << std::endl
       << "Max Cost: " << bluePrint.maxCostOfEachBot << std::endl
       << "Max Geodes: " << bluePrint.maxGeodes << std::endl;
    return os;
}

Blueprint inputBlueprint(std::string blueprintData)
{
    std::regex pattern(R"(Blueprint ([0-9]+): Each ore robot costs ([0-9]+) ore. Each clay robot costs ([0-9]+) ore. Each obsidian robot costs ([0-9]+) ore and ([0-9]+) clay. Each geode robot costs ([0-9]+) ore and ([0-9]+) obsidian.)");
    std::smatch baseMatch;

    Blueprint bp;

    std::regex_match(blueprintData, baseMatch, pattern);


    bp.ID = stoi(baseMatch[1].str());
    bp.oreRobotCost.ore = stoi(baseMatch[2].str());
    bp.clayRobotCost.ore = stoi(baseMatch[3].str());
    bp.obsidianRobotCost.ore = stoi(baseMatch[4].str());
    bp.obsidianRobotCost.clay = stoi(baseMatch[5].str());
    bp.geodeRobotCost.ore = stoi(baseMatch[6].str());
    bp.geodeRobotCost.obsidian = stoi(baseMatch[7].str());

    bp.updateMaxCost();

    return bp;
}

struct State
{
    int time = 0;
    int ore = 0; 
    int oreBots = 0;
    int clayBots = 0;
    int obsidianBots = 0;
    int geodeBots = 0;
    ResourceValues resources;

    friend std::ostream& operator<<(std::ostream& os, const State& state);

};

std::ostream& operator<<(std::ostream& os, const State& state)
{
    os << "Time: " << state.time
       << "\nOre: " << state.ore 
       << "\nOre Bots: " << state.oreBots
       << "\nClay Bots: " << state.clayBots
       << "\nObsidian Bots: " << state.obsidianBots
       << "\nGeode Bots: " << state.geodeBots << std::endl;
    return os;
}

inline int calcTheoreticalMax(const State& state)
{
    return state.resources.geodes + (state.time + 2 * state.geodeBots) * state.time / 2;
}

bool isActionHelpful(const State& state, const Blueprint& blueprint, const Action& action)
{
    if(action == Action::ORE)
    {
        return state.oreBots < blueprint.maxCostOfEachBot.ore;
    }
    else if (action == Action::CLAY)
    {
        return state.clayBots < blueprint.maxCostOfEachBot.clay;
    }
    else if (action == Action::OBSIDIAN)
    {
        return state.obsidianBots < blueprint.maxCostOfEachBot.obsidian;
    }
    return true;
}

bool isActionPossible(const State& state, const Blueprint& blueprint, const Action& action)
{
    if(action == Action::ORE)
    {
        return state.resources >= blueprint.oreRobotCost;
    }
    else if (action == Action::CLAY)
    {
        return state.resources >= blueprint.clayRobotCost;
    }
    else if(action == Action::OBSIDIAN)
    {
        return state.resources >= blueprint.obsidianRobotCost;
    }
    return state.resources >= blueprint.geodeRobotCost;
}

void updateState(State& state)
{
    state.time -= 1;
    state.resources.ore += state.oreBots;
    state.resources.clay += state.clayBots;
    state.resources.obsidian += state.obsidianBots;
    state.resources.geodes   += state.geodeBots;
}

State performAction(State state, const Blueprint& blueprint, const Action& action)
{
    if(action == Action::ORE)
    {
        state.resources = state.resources - blueprint.oreRobotCost;
        updateState(state);
        state.oreBots++;
    }
    else if(action == Action::CLAY)
    {
        state.resources = state.resources - blueprint.clayRobotCost;
        updateState(state);
        state.clayBots++;
    }
    else if(action == Action::OBSIDIAN)
    {
        state.resources = state.resources - blueprint.obsidianRobotCost;
        updateState(state);
        state.obsidianBots++;
    }
    else
    {
        state.resources = state.resources - blueprint.geodeRobotCost;
        updateState(state);
        state.geodeBots++;
    }
    return state;
}

void depthFirstSearch(State state, const Blueprint& blueprint,
                      int& maxGeodes, const std::array<Action, 4>& actions,
                      const Action action)
{
    if(!isActionHelpful(state, blueprint, action)) { return; }
    if(calcTheoreticalMax(state) <= maxGeodes) { return; }

    while(state.time > 0)
    {
        if(isActionPossible(state, blueprint, action))
        {
            auto newState = performAction(state, blueprint, action);
            for(int i = 0; i < 4; i++)
            {
                depthFirstSearch(newState, blueprint, maxGeodes, actions, actions[i]);
            }
            return;
        }
        else
        {
            updateState(state);
        }
    }
    maxGeodes = std::max(maxGeodes, state.resources.geodes);
}

void answerPart1(std::vector<Blueprint>& blueprints,
               const std::array<Action, 4>& actions)
{
    auto state = State();
    state.time = 24;
    state.oreBots = 1;
    int answer = 0;
    for(auto& blueprint : blueprints)
    {
        int maxGeodes = -1;
        for(int i = 0; i < 4; i++)
        {
            depthFirstSearch(state, blueprint, maxGeodes, actions, actions[i]);
        }
        blueprint.maxGeodes = maxGeodes;
        answer += blueprint.ID * blueprint.maxGeodes;
    }
    std::cout << "Answer Part 1: " << answer << std::endl;
}

void answerPart2(std::vector<Blueprint>& blueprints,
                 const std::array<Action, 4>& actions)
{
    auto state = State();
    state.time = 32;
    state.oreBots = 1;
    int answer = 1;
    for(int j = 0; j < 3; j++)
    {
        int maxGeodes = -1;
        for(int i = 0; i < 4; i++)
        {
            depthFirstSearch(state, blueprints[j], maxGeodes, actions, actions[i]);
        }
        blueprints[j].maxGeodes = maxGeodes;
        answer *= blueprints[j].maxGeodes;
    }
    std::cout << "Answer Part 2: " << answer << std::endl;
}

int main() {
    std::ifstream inputFile("input.txt");

    if(!inputFile.is_open()) { std::cout << "Error, file not opened." << std::endl; }
    std::string line;

    std::vector<Blueprint> bluePrints;
    std::vector<Blueprint> bluePrintsAnswer2;
    while(std::getline(inputFile, line))
    {
        auto tempBP = inputBlueprint((line));
        bluePrints.push_back(tempBP);
        bluePrintsAnswer2.push_back(tempBP);
    }

    constexpr  std::array<Action, 4> actions {Action::ORE,
                                              Action::CLAY,
                                              Action::OBSIDIAN,
                                              Action::GEODE };



    answerPart1( bluePrints, actions);
    answerPart2(bluePrintsAnswer2, actions);


    return 0;
}
