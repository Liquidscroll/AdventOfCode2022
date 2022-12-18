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
    void returnDirSizes(std::vector<int>& sizes)
    {
        if(isDirectory) { sizes.push_back(size);
        
        for(auto &child : children)
        {
         child.returnDirSizes(sizes);   
        }
    }
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
    while(std::getline(inputFile, line))
    {
        if(line[0] == '$')
        {
            interpretCommands = true;
        }
        else
        {
            interpretCommands = false;
        }
        
        counter++;
        std::cout << "Loop: " << counter << std::endl;
        if(interpretCommands)
        {
            char dollar;
            std::string command, argument;
            inputFile >> dollar >> command;
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
                    }
                }
            }
            if(command == "ls")
            {
                interpretCommands = false;
                continue;
                
            }

        } 
        else
        {
            std::string command;
            inputFile >> command;

            if(command == "$")
            {
                inputFile.seekg(-2, std::ios::cur);
                interpretCommands = true;
            } else if (command == "dir")
            {
                std::string inputName;
                inputFile >> inputName;
                std::cout << "command: " << command << " arg: " << inputName << std::endl;

                currentDir->children.push_back( {currentDir, 0, true, inputName, dirChildren});
            }
            else
            {
                int size;
                //std::stringstream s(command);
                inputFile >> size;

                std::string name;
                inputFile >> name;

                currentDir ->children.push_back({currentDir, size, false, name, dirChildren});
            }
        }   
    }
    

    fileTree.distributeChildSizes();
    std::vector<int> sizes;
    fileTree.returnDirSizes(sizes);
    fileTree.printDirectory();
    int answer = 0;
    for(auto &size : sizes)
    {
        if(size < 100000)
        {
        answer += size;
        }
    }
    std::cout << "Answer to Part 1: " << answer << std::endl;
    return 0;
}