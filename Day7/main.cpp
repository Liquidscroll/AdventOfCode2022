#include <fstream>
#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include <numeric>
#include <sstream>

struct dirNode
{
  dirNode* parent;

  int size;

  bool isDir;

  std::string nodeName;
  std::vector<dirNode> children;

  dirNode* returnParent()
  {
    if(parent == nullptr) return this;
    return parent;
  }

  void printDir(int dirLevel = 0)
  {
    std::cout << std::string(dirLevel, ' ') << " - " << nodeName;

    if(size == 0) std::cout << " (dir)";
    else std::cout << " (file, size=" << size << ")";
    std::cout << std::endl;

    for(auto& child : children)
    {
      child.printDir(dirLevel + 1);
    }
  }
    int propagateSizes()
    {
      if(size > 0) return size;

      size = 0;

      for(auto& child: children)
      {
        size += child.propagateSizes();
      }
      return size;
    }

    int sumSizes()
    {
      int size = 0;
      if(isDir && this->size < 100000) { size += this->size; }

      for(auto& child: children)
      {
        size += child.sumSizes();
      }

      return size;
    }

    void enumerateDirSizes(std::vector<int>& sizes)
    {
      if(isDir) { sizes.push_back(size); } 

      for(auto& child: children)
      {
        child.enumerateDirSizes(sizes);
      }
    }
  };




int main() {
  std::ifstream input_file("inputexample.txt", std::ios::binary);
  std::vector<dirNode> children;
  
  dirNode tree { nullptr, 0, true, "/", children};

  dirNode* currentDir = &tree;

  bool commandMode = true;
  
  while(input_file)
  {
    if(commandMode)
    {
      char dollarSign;
      std::string command, arg;
      input_file >> dollarSign >> command;

      if(!input_file) break;

      if(command == "cd")
      {
        input_file >> arg;

        if(arg == "/")
        {
          currentDir = &tree;
        } else if (arg == "..")
        {
          currentDir = currentDir->parent;
        } else for(auto& child: currentDir->children)
        {
          if(child.nodeName == arg)
          {
            currentDir = &child;
            break;
          }
        }
      }
    } else {
     
      std::string command;
      input_file >> command;
      if(!input_file) break;

      if(command == "$")
      {
        input_file.seekg(-2, std::ios::cur);
        commandMode = true;
      } else if (command == "dir")
      {
        std::string name;
        input_file >> name;

        currentDir->children.push_back({currentDir, 0, true, name, children});
      } else {
        int size;
        std::stringstream s(command);
        s >> size;

        std::string name;
        input_file >> name;

        currentDir->children.push_back({currentDir, size, false, name, children});
      }
    }
  }
  
  tree.propagateSizes();

  std::cout << tree.sumSizes() << std::endl;
  

  //std::cout << "Answer Part 1: " << findSignalMark(line, 4) << std::endl;
  //std::cout << "Answer Part 2: " << findSignalMark(line, 14) << std::endl;
  return 0;
}