#include <fstream>
#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include <numeric>
#include <sstream>
#include <set>

struct device_info
{
    int value  = 1;
    int cycleNum = 1;
    std::vector<int> signalStrengths;
    std::string deviceScreen;
};

void doCycle(int value, int &cycle, std::vector<int> &strengths, std::string &screen)
{
    if (cycle % 40 == 20)
    {
        strengths.push_back(cycle * value);
    }

    int screenCRT = cycle - 1;
    if(std::abs(screenCRT % 40 - value) < 2)
    {
        screen += '#';
    } else
    {
        screen += '.';
    }
    if (cycle++ % 40 == 0)
    {
        screen += "\n";
    }
}

void printScreen(device_info &device)
{
    std::cout << std::string(17, ' ') << "Screen" << std::string(19, ' ') << std::endl;
    
    std::cout << device.deviceScreen << std::endl;

}

int main()
{
    std::ifstream inputFile("input.txt");
    if(inputFile.is_open()) std::cout << "File Opened" << std::endl;
    std::string inputCommand;
    std::vector<int> toCheck = {20, 60, 100, 140, 180, 220};

    device_info commDevice;

    while(inputFile >> inputCommand)
    {
        doCycle(commDevice.value, commDevice.cycleNum, commDevice.signalStrengths, commDevice.deviceScreen);
        if(inputCommand == "addx")
        {
            int inputValue;
            inputFile >> inputValue;
            doCycle(commDevice.value, commDevice.cycleNum, commDevice.signalStrengths, commDevice.deviceScreen);
            commDevice.value += inputValue;
        }
        
        
    }

    int sum = 0;
    for(auto s : commDevice.signalStrengths)
    {
        sum += s;
    }


    std::cout << "\nAnswer to Part 1: " << sum << std::endl;
    std::cout << "Answer to Part 2: " << std::endl;
    printScreen(commDevice);
    return 0;
}