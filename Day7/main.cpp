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

    void printDirectory(size_t depth = 0)
    {
        std::cout << std::string(depth, ' ') << " - " << name;

        if(size == 0)
        {
            std::cout << " (dir)";
        }
        else 
        {
            std::cout << " (file, size=" << size << ")";
        }
        std::cout << std::endl;

        for(auto& child : children)
        {
            child.printDirectory(depth + 1);
        }
    }

    int sumSizes()
    {
        std::cout << "size: " << this->size << std::endl;
        int sum = 0;
        if(isDirectory && this->size < 100000) { sum += this->size; }

        for(auto& child : children)
        {
            sum += child.sumSizes();
        }
        std::cout << "sum: " << sum << std::endl;
        return sum;
    }

    int distributeChildSizes()
    {
        if (size > 0) { return size; }

        size = 0;

        for(auto &child : children)
        {
            size += child.distributeChildSizes();
        }
        return size;
    }

};

int main()
{
    std::ifstream inputFile("inputexample.txt");
    std::vector<fileFolder> dirChildren;
    bool interpretCommands = true;
    int counter = 0;
    fileFolder fileTree { nullptr, 0, true, "/", dirChildren};

    fileFolder* currentDir = &fileTree;
    std::string line;
    while(inputFile)
    {
        char dollar;
        std::string command, argument;
        if (inputFile.eof()) break;
        counter++;
        std::cout << counter << std::endl;
        if(interpretCommands)
        {
            inputFile >> dollar >> command;
            std::cout << dollar << command << std::endl;
            if(command == "cd")
            {
                inputFile >> argument;
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
                        continue;
                    }
                }
            }
            if(command == "ls")
            {
                interpretCommands = false;
                
            }

        } 
        else
        {
            inputFile >> command;
            std::cout << command << std::endl;

            if(command == "$")
            {
                inputFile.seekg(2, std::ios::cur);
                interpretCommands = true;
            } else if (command == "dir")
            {
                std::string inputName;
                inputFile >> inputName;

                currentDir->children.push_back( {currentDir, 0, true, inputName, dirChildren});
            }
            else
            {
                int size;
                std::stringstream s(command);
                s >> size;

                std::string name;
                inputFile >> name;

                currentDir ->children.push_back({currentDir, size, false, name, dirChildren});
            }
        }   
    }
    

    fileTree.distributeChildSizes();
    fileTree.printDirectory();
    int answer = fileTree.sumSizes();
    std::cout << "Answer to Part 1: " << answer << std::endl;
    return 0;
}