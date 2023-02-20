#include <fstream>
#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include <sstream>
#include <set>
#include <deque>
#include <cstdint>

static inline void trimWhiteSpaceStart(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
        return !std::isspace(ch);
    }));
}

struct monkey
{
    std::deque<long long> items;

    std::string symb;
    std::string opValue;
    int valueForTest;

    size_t truePass;
    size_t falsePass;

    long long passCount;

    monkey() {
        symb = "";
        opValue = "";
        valueForTest = -1;
        truePass = -1;
        falsePass = -1;
        passCount = 0;
    }

    void pass(std::vector<monkey>& monkeys, bool pass, long long item)
    {
        if(pass)
        {
            monkeys[this->truePass].items.push_back(item);
        }
        if(!pass)
        {
            monkeys[this->falsePass].items.push_back(item);

        }
        this->passCount++;
    }




    bool testItems(long long& item) const
    {
        if(item % this->valueForTest == 0)
        {
            return true;
        }

        return false;
    }

    void printMonkey()
    {
        std::cout << "Monkey:\nItems: ";
        for(auto item : this->items)
        {
            std::cout << item << ",";
        }
        std::cout << std::endl;

        std::cout << "OpSymbol: " << this->symb << "\nOpValue: " << this->opValue << std::endl;
        std::cout << "Value for test: " << this->valueForTest << std::endl;

        std::cout << "True pass: " << this->truePass << std::endl;
        std::cout << "False pass: " << this->falsePass << std::endl;

        std::cout << "Pass Count: " << this->passCount << std::endl;
    }
};

void inspection(std::vector<monkey>& monkeys, monkey& monkey, int currentMonkey)
{


    for(auto& item : monkey.items)
    {
        long long operatorVal;
        if(monkey.opValue == "old") {
            operatorVal = item;
        }
        else
        {
            operatorVal = stoi(monkey.opValue);
        }
        if(monkey.symb == "+")
        {
            item += operatorVal;
        }
        else if(monkey.symb == "*")
        {
            item *= operatorVal;
        }


        item = (item / 3);


        if(monkey.testItems(item))
        {
            monkey.pass(monkeys, true, item);
            monkeys[currentMonkey].items.erase(std::remove(monkeys[currentMonkey].items.begin(), monkeys[currentMonkey].items.end(), item));
            monkeys[currentMonkey].passCount++;
        } else
        {
            monkey.pass(monkeys, false, item);
            monkeys[currentMonkey].items.erase(std::remove(monkeys[currentMonkey].items.begin(), monkeys[currentMonkey].items.end(), item));
            monkeys[currentMonkey].passCount++;
        }
    }
}

void inspectionPart2(std::vector<monkey>& monkeys, monkey& monkey, int currentMonkey, int product)
{


    for(auto& item : monkey.items)
    {
        long long operatorVal;
        if(monkey.opValue == "old") {
            operatorVal = item;
        }
        else
        {
            operatorVal = stoi(monkey.opValue);
        }
        if(monkey.symb == "+")
        {
            item += operatorVal;
        }
        else if(monkey.symb == "*")
        {
            item *= operatorVal;
        }


        //item = (item / 3);

        item = (item % product);

        if(monkey.testItems(item))
        {
            monkey.pass(monkeys, true, item);
            monkeys[currentMonkey].items.erase(std::remove(monkeys[currentMonkey].items.begin(), monkeys[currentMonkey].items.end(), item));
            monkeys[currentMonkey].passCount++;
        } else
        {
            monkey.pass(monkeys, false, item);
            monkeys[currentMonkey].items.erase(std::remove(monkeys[currentMonkey].items.begin(), monkeys[currentMonkey].items.end(), item));
            monkeys[currentMonkey].passCount++;
        }
    }
}

int main()
{
    std::fstream inputFile("E:\\AdventOfCode2022\\Day11\\input.txt");
    if(!inputFile.is_open()) { std::cout << "fail open"; return 0;}
    std::string inputCommand;
    std::vector<monkey> monkeys;
    int currentMonkey = 0;
    int product = 1;

    monkeys.emplace_back();

    while(std::getline(inputFile, inputCommand))
    {
        size_t delimPos = inputCommand.find_first_of(':');
        std::string half1 = inputCommand.substr(0, delimPos);
        std::string half2 = inputCommand.substr(delimPos + 1);
        if(half1[0] == 'M')
        {
            long long monkeyNum = (long long)half1[half1.size() - 1] - 48;
            if(monkeyNum > 0) {
                monkeys.emplace_back();
                currentMonkey++;
            }
        }
        trimWhiteSpaceStart(half1);
        trimWhiteSpaceStart(half2);
        if(half1 == "Starting items")
        {
            std::istringstream data(half2);
            while(data)
            {
                char comma;
                long long value;
                data >> value >> comma;

                monkeys[currentMonkey].items.push_back(value);
            }
        }
        if(half1 == "Operation")
        {
            std::istringstream data(half2);
            std::string newValue, equal, oldValue, operation, operationValue;
            data >> newValue >> equal >> oldValue >> operation >> operationValue;

            monkeys[currentMonkey].symb = operation;
            monkeys[currentMonkey].opValue = operationValue;
        }
        if(half1 == "Test")
        {
            std::string op, junk, opValue;
            std::istringstream data(half2);
            data  >> op >> junk >> opValue;
            monkeys[currentMonkey].valueForTest = stoi(opValue);
            product *= monkeys[currentMonkey].valueForTest;
        }
        if(half1 == "If true")
        {
            monkeys[currentMonkey].truePass = half2[half2.size() - 1] - 48;
        }
        if(half1 == "If false")
        {
            monkeys[currentMonkey].falsePass = half2[half2.size() - 1] - 48;
        }


    }

    auto monkeys2 = monkeys;
    long long round = 20;
    long long round2 = 10000;

    for(long long i = 0; i < round; i++) {
        int curMonk = 0;
        for (auto monk: monkeys) {
            inspection(monkeys, monk, curMonk);
            curMonk++;
        }
    }

    for(long long i = 0; i < round2; i++)
    {
        int curMonk = 0;
        for(auto monk: monkeys2)
        {
            inspectionPart2(monkeys2, monk, curMonk, product);
            curMonk++;
        }
    }


    std::vector<long long> passCounts;
    std::vector<long long> passCounts2;
    for(auto& monk : monkeys) {
        passCounts.push_back(monk.passCount);
    }
    for(auto& monk : monkeys2)
    {
        passCounts2.push_back(monk.passCount);
    }

    std::sort(passCounts.begin(), passCounts.end(), std::greater<long long>());
    std::sort(passCounts2.begin(), passCounts2.end(), std::greater<long long>());

    long long answer = passCounts[0] * passCounts[1];
    long long answer2 = passCounts2[0] * passCounts2[1];

    std::cout << "Answer to Part 1: " << answer << std::endl;
    std::cout << "Answer to Part 2: " << answer2 << std::endl;

    return 0;
}