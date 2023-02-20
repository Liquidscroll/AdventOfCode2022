#include <fstream>
#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include <numeric>
#include <sstream>

struct fileFolder
{
    fileFolder* upperDirectory;

    int size;

    bool isDirectory;

    std::string name;
    std::vector<fileFolder> children;

    fileFolder* returnUpperDirectory()
    {
        if(upperDirectory == nullptr) return this;
        return upperDirectory;
    }

    void printDirectory(int depth)
    {
        std::cout << std::string(depth, ' ') << " - " << name;

        if(this->isDirectory)
        {
            std::cout << " (dir, size: " << size << ")";
        }
        else 
        {
            std::cout << " (file, size: " << size << ")";
        }
        std::cout << std::endl;
        for(auto& child : children)
        {
            child.printDirectory(depth + 1);
        }
    }

    int sumSizes()
    {
        int sum = 0;
        if(isDirectory && this->size < 100000) { sum += this->size; }

        for(auto& child : children)
        {
            sum += child.sumSizes();
        }
        std::cout << "sum: " << sum << std::endl;
        return sum;
    }

    int fillDirSizes()
    {
        if (this->isDirectory == false) { return this->size; }

        int totalSize = 0;

        for(auto &child : children)
        {
            totalSize += child.fillDirSizes();
        }
        this->size = totalSize;
        return this->size;
    }
    int returnDirSizes(int filterSize)
    {
        int totalSize = 0;
        if(this->isDirectory && this->size <= filterSize) { totalSize += this->size; }
        
        for(auto &child : children)
        {
         totalSize += child.returnDirSizes(filterSize);   
        }

        return totalSize;
    }
    void returnSizePart2(std::vector<int> &sizeVector)
    {
        if(this->isDirectory)
        { 
            sizeVector.push_back(this->size);
        }
        
        for(auto &child : children)
        {
            child.returnSizePart2(sizeVector);
        }
        return;
    }

};


int main()
{
    int const &capacity = 70000000;
    std::ifstream inputFile("input.txt");

    std::vector<fileFolder> dirChildren;
    bool interpretCommands = false;
    fileFolder fileTree { nullptr, 0, true, "/", dirChildren};
    std::string line;
    fileFolder* currentDir = &fileTree;

    std::string firstInput;
    while(std::getline(inputFile, line))
    {
        if(line.front() == '$')
        {
            interpretCommands = true;
        }
        else if (line.front() != '$')
        {
            interpretCommands = false;
        }
        
        if(interpretCommands)
        {
            std::string command, argument;
            size_t secondSpace = line.find_first_of(' ', 2);
            command = line.substr(2, 2);
            if(command == "ls") continue;
            argument = line.substr(secondSpace + 1, line.size() - secondSpace);
            if(command == "cd")
            {
                if(argument == "..")
                {
                    currentDir = currentDir->upperDirectory;
                }
                if(argument == "/")
                {
                    currentDir = &fileTree;
                }
                else for(auto &child : currentDir->children)
                {
                    if(child.name == argument)
                    {
                        currentDir = &child;
                    }
                }
            }
        } 
        else if(interpretCommands == false)
        {
            std::string command, argument;
            size_t space = line.find_first_of(' ');
            command = line.substr(0, space);
            argument = line.substr(space + 1);
            if (command == "dir")
            {
                currentDir->children.push_back( {currentDir, 0, true, argument, dirChildren});
            }
            else
            {
                currentDir ->children.push_back({currentDir, std::stoi(command), false, argument, dirChildren});
            }
        }  
    }
    
    fileTree.fillDirSizes();
    //fileTree.printDirectory(0);
    int answer = fileTree.returnDirSizes(100000);

    int stored = fileTree.size;
    int freeSpace = capacity - stored;
    int toFind = 30000000 - freeSpace;

    std::vector<int> dirSizeVector;
    fileTree.returnSizePart2(dirSizeVector);
    std::sort(dirSizeVector.begin(), dirSizeVector.end());
    int smallestDir = fileTree.size;
    for(int i = dirSizeVector.size(); i >= 0; i--)
    {
        if(dirSizeVector[i] > toFind && dirSizeVector[i] < smallestDir)
        {
            smallestDir = dirSizeVector[i];
        }
    }

    std::cout << "Answer to Part 1: " << answer << std::endl;
    std::cout << "Answer to Part 2: " << smallestDir << std::endl;
    return 0;
}