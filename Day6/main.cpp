#include <fstream>
#include <iostream>
#include <unordered_map>
#include <algorithm>
#include <string>
#include <vector>

size_t findSignalMark(const std::string &inputLine, size_t markerSize)
{
  std::unordered_map<char, size_t> charMap;
  size_t start = 0, end = 0;
  while(end - start < markerSize)
  {
    char c = inputLine[end];
    if(charMap.find(c) != charMap.end()) { start = std::max(start, charMap[c] + 1); }
    charMap[c] = end++;
  }

  return end;

}

int main() {
  std::ifstream input_file("input.txt");
  std::string line;

  input_file >> line;
  
  

  std::cout << "Answer Part 1: " << findSignalMark(line, 4) << std::endl;
  std::cout << "Answer Part 2: " << findSignalMark(line, 14) << std::endl;
  return 0;
}