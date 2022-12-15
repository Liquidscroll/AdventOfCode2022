#include <fstream>
#include <iostream>
#include <unordered_map>
#include <string>
#include <vector>

void printCrates(std::vector<std::vector<std::string>> crates)
{
  system("cls");
  size_t height = 0;
  //Print crates in column format
  for(auto c: crates)
  {
    if(c.size() > height)
    {
      height = c.size();
    }
  }
  
  for(int i = height; i >= 0; i--)
  {

      for(auto innerValues : crates)
      {
        try
        {
        auto value = innerValues.at(i);
        std::cout << "[" << value << "]";
        }
        catch(std::out_of_range& e)
        {
          std::cout << "   ";
        }
      }
      std::cout << " " << std::endl;
  }
  for(size_t i = 0; i < crates.size(); i++)
  {
    std::cout << " " << i + 1 << " ";
  }
}

std::vector<std::vector<std::string>> arrangeCrates(std::vector<std::vector<std::string>> inputCrates,
                                                    int src, int dest, int moveAmt)
{
  std::string temp;

  for(int i = 0; i < moveAmt; i++)
  {
    temp = inputCrates.at(src - 1).back();
    inputCrates.at(src - 1).pop_back();
    inputCrates.at(dest - 1).push_back(temp);
  }

  return inputCrates;
}

std::vector<std::vector<std::string>> arrangeMultipleCrates(std::vector<std::vector<std::string>> inputCrates,
                                                            int src, int dest, int numCrates)
{
  
  std::vector<std::string> temp;
  for(int j = 0; j < numCrates; j++)
  {
  temp.push_back(inputCrates.at(src - 1).back());
  inputCrates.at(src - 1).pop_back();
  }
  
  for(size_t i = temp.size(); i > 0; i--)
  {

  inputCrates.at(dest - 1).push_back(temp.at(i - 1));

  }

  

  return inputCrates;
}
int main() {
  std::ifstream input_file("input.txt");
  std::string line;

  std::vector<std::vector<std::string>> inputCrates1 {
                                       {"F", "C", "J", "P", "H", "T", "W"}, 
                                       {"G", "R", "V", "F", "Z", "J", "B", "H"},
                                       {"H", "P", "T", "R"},
                                       {"Z", "S", "N", "P", "H", "T"},
                                       {"N", "V", "F", "Z", "H", "J", "C", "D"},
                                       {"P", "M", "G", "F", "W", "D", "Z"},
                                       {"M", "V", "Z", "W", "S", "J", "D", "P"},
                                       {"N", "D", "S"},
                                       {"D", "Z", "S", "F", "M"}, };
  std::vector<std::vector<std::string>> inputCrates2 = inputCrates1;

  //printCrates(inputCrates1);
  std::string junk, junk1, junk2;
  int source, moveAmount, destination;

  while(input_file >> junk >> moveAmount >> junk1 >> source >> junk2 >> destination)
  {
    inputCrates1 = arrangeCrates(inputCrates1, source, destination, moveAmount);
    inputCrates2 = arrangeMultipleCrates(inputCrates2, source, destination, moveAmount);
    //printCrates(inputCrates1);
  
  }
  std::cout << "\nAnswer Part 1:" << std::endl;
  for(auto crateGroup : inputCrates1)
  {
    std::cout <<  crateGroup.back();
  }
  std::cout << "\nAnswer Part 2:" << std::endl;
  for(auto crateGroup : inputCrates2)
  {
    std::cout << crateGroup.back();
  }
  return 0;
}