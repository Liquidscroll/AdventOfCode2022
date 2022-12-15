#include <fstream>
#include <iostream>
#include <unordered_map>
#include <string>
#include <cstring>
char findCommonChar(std::string line)
{
    std::string splitLine[2];
    splitLine[0] = line.substr(0, line.size() / 2);
    splitLine[1] = line.substr(line.size() / 2, line.size());

    for(size_t i = 0; i < splitLine[0].size(); i++) 
    {
      for(size_t j = 0; j < splitLine[1].size(); j++)
      {
        if(splitLine[0][i] == splitLine[1][j])
        {
          return splitLine[0][i];
        }
      }
    }
    return '\0';
}

char findCommonChar(std::string line1, std::string line2, std::string line3)
{
  for(size_t i = 0; i < line1.size(); i++)
  {
    for(size_t j = 0; j < line2.size(); j++)
    {
      if(line1[i] == line2[j])
      {
        for(size_t k = 0; k < line3.size(); k++)
        {
          if(line1[i] == line3[k])
          {
            return line1[i];
          }
        }
      }
    }
  }

  return '\0';
}
int main() {
  // Open the input file
  std::ifstream input_file("input.txt");

  std::string alphabet = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
  std::unordered_map<char, int> alphabetValues;


  for (size_t i = 0; i < alphabet.size(); i++) {
    alphabetValues.insert(std::pair<char, int>(alphabet[i], i + 1));
  }  
  
  std::string line, line1, line2;
  std::string commonCharPart1 = "", commonCharPart2 = "";
  int sum1 = 0, sum2 = 0;
  
  while (input_file >> line >> line1 >> line2) {
    commonCharPart1 += findCommonChar(line);
    commonCharPart1 += findCommonChar(line1);
    commonCharPart1 += findCommonChar(line2);
    commonCharPart2 += findCommonChar(line, line1, line2);    
  }


  for(size_t i = 0; i < commonCharPart1.size(); i++)
  {
    sum1 += (alphabetValues.find(commonCharPart1[i]))->second;
  }
  for(size_t i = 0; i < commonCharPart2.size(); i++)
  {
    sum2 += (alphabetValues.find(commonCharPart2[i]))->second;
  }
  // Print the sum of priorities
  std::cout << "Answer Part 1: " << sum1 << std::endl;
  std::cout << "Answer Part 2: " << sum2 << std::endl;
  return 0;
}