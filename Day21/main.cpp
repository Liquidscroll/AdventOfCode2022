#include <algorithm>
#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <unordered_map>


struct Monkey
{
    long long id;
    std::string name;
    bool yellIsNumber;
    long long val = 0;

    std::string monk1;
    std::string monk2;

    char oper;
    bool calcPerformed = false;
};

long long calcResult(long long num1, long long num2, char op)
{
    switch (op)
    {
        case '+':
            return num1 + num2;
        case '-':
            return num1 - num2;
        case '*':
            return num1 * num2;
        case '/':
            return num1 / num2;
        default:
            std::cout << "Invalid operation" << std::endl;
            return 0;
    }
}

long long reverseCalcResult(long long num1, long long num2, char op)
{
    switch(op)
    {
        case '+':
            return num1 - num2;
        case '-':
            return num1 + num2;
        case '*':
            return num1 / num2;
        case '/':
            return num1 * num2;
        case '=':
            return num2;
        default:
            std::cout << "Invalid operation" << std::endl;
            return 0;
    }
}
void readMonkeys(std::vector<Monkey>& monkeys,
                 std::unordered_map<std::string, long long>& monkeyMappings,
                 std::string input, bool isPart2)
{
    std::fstream inputFile(input);
    std::string line;

    long long id = 0;

    while(std::getline(inputFile, line))
    {
        auto monkey = Monkey();
        monkey.id = id;
        monkey.name = line.substr(0, 4);

        if(line[6] >= '0' && line[6] <= '9')
        {
            monkey.yellIsNumber = true;
            monkey.val = std::stoll(line.substr(6, line.size() - 6));
        }
        else
        {
            monkey.yellIsNumber = false;
            auto start = 6;
            auto end = line.find(' ', start);
            monkey.monk1 = line.substr(start, end - start);
            start = end + 1;
            monkey.oper = line[start];
            start += 2;
            monkey.monk2 = line.substr(start, line.size() - start);
        }
        if(monkey.name == "root" && isPart2)
        {
            monkey.oper = '=';
            monkey.calcPerformed = true;
            monkey.yellIsNumber = false;
        }
        if(monkey.name == "humn" && isPart2)
        {
            monkey.oper = '?';
            monkey.yellIsNumber = true;
        }
        monkeys.push_back(monkey);
        monkeyMappings[monkey.name] = id;
        id++;
    }

}

void answerPart1(std::vector<Monkey>& monkeys,
                 std::unordered_map<std::string, long long>& monkeyMappings)
{

    while(true)
    {
        for(auto& monkey : monkeys)
        {
            if(monkey.calcPerformed) { continue; }

            if(!monkey.yellIsNumber)
            {
                if(!monkeys[monkeyMappings[monkey.monk1]].calcPerformed)
                {
                    continue;
                }
                if(!monkeys[monkeyMappings[monkey.monk2]].calcPerformed)
                {
                    continue;
                }

                monkey.val = calcResult(monkeys[monkeyMappings[monkey.monk1]].val,
                                         monkeys[monkeyMappings[monkey.monk2]].val,
                                         monkey.oper);
                monkey.calcPerformed = true;

                if(monkey.name == "root")
                {
                    std::cout << "Answer Part 1: " << monkey.val << std::endl;
                    return;
                }
            }
            else
            {
                monkey.calcPerformed = true;
            }
        }
    }

}

void answerPart2(std::vector<Monkey>& monkeys,
                 std::unordered_map<std::string, long long>& monkeyMappings)
{
    while(true)
    {
        for(auto& monkey : monkeys)
        {
            if(!monkey.calcPerformed && monkey.name != "humn")
            {
                if(!monkey.yellIsNumber)
                {
                    if(!monkeys[monkeyMappings[monkey.monk1]].calcPerformed) { continue; }
                    if(!monkeys[monkeyMappings[monkey.monk2]].calcPerformed) { continue; }

                    monkey.val = calcResult(monkeys[monkeyMappings[monkey.monk1]].val,
                                             monkeys[monkeyMappings[monkey.monk2]].val,
                                             monkey.oper);
                    monkey.calcPerformed = true;
                }
                else
                {
                    monkey.calcPerformed = true;
                }
            }

            if(monkey.calcPerformed && !monkey.yellIsNumber)
            {
                int mid1 = monkeyMappings[monkey.monk1];
                int mid2 = monkeyMappings[monkey.monk2];

                if((monkeys[mid1].calcPerformed) && (monkeys[mid2].calcPerformed))
                {
                    continue;
                }
                else if ((monkeys[mid1].calcPerformed) && !(monkeys[mid2].calcPerformed))
                {
                    monkeys[mid2].val = reverseCalcResult(monkey.val, monkeys[mid1].val, monkey.oper);
                    if(monkey.oper == '/')
                    {
                        monkeys[mid2].val = monkeys[mid1].val / monkey.val;
                    }
                    if(monkey.oper == '-')
                    {
                        monkeys[mid2].val = monkeys[mid1].val - monkey.val;
                    }
                    monkeys[mid2].calcPerformed = true;
                }
                else if ((!monkeys[mid1].calcPerformed) && (monkeys[mid2].calcPerformed))
                {
                    monkeys[mid1].val = reverseCalcResult(monkey.val, monkeys[mid2].val, monkey.oper);
                    monkeys[mid1].calcPerformed = true;
                }
                else if (!(monkeys[mid1].calcPerformed) && !(monkeys[mid2].calcPerformed))
                {
                    continue;
                }
            }
            if(monkeys[monkeyMappings["humn"]].calcPerformed)
            {
                std::cout << "Answer Part 2: " << monkeys[monkeyMappings["humn"]].val << std::endl;
                return;
            }

        }
    }

}


int main()
{
    std::string in = "input.txt";

    std::vector<Monkey> monkeys;
    std::vector<Monkey> monkeys2;

    std::unordered_map<std::string, long long> monkeyMappings;
    std::unordered_map<std::string, long long> monkeyMappings2;

    readMonkeys(monkeys, monkeyMappings, in, false);
    readMonkeys(monkeys2, monkeyMappings2, in, true);

    answerPart1(monkeys, monkeyMappings);
    answerPart2(monkeys2, monkeyMappings2);
    return 0;
}