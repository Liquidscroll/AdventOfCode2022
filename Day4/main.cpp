#include <fstream>
#include <iostream>
#include <unordered_map>
#include <string>
#include <vector>

std::vector<int> returnNumberRange(std::string input)
{
    std::vector<int> numbers;
    size_t dashLocation = input.find('-');
    numbers.push_back(std::stoi(input.substr(0, dashLocation)));
    numbers.push_back(std::stoi(input.substr(dashLocation + 1)));
    
    return numbers;

}

int checkRangePart1(std::vector<int> range1, std::vector<int> range2, int answer)
{
    if(range1[0] <= range2[0] && range1[1] >= range2[1]) {
      answer++;
    }
    else if(range1[0] >= range2[0] && range1[1] <= range2[1]) {
      answer++;
    }
  
    return answer;
}
int checkRangePart2(std::vector<int> range1, std::vector<int> range2, int answer)
{
  if(range1[0] >= range2[0] && range1[0] <= range2[1]) {
      return ++answer;
    }
    if(range1[1] >= range2[0] && range1[1] <= range2[1]) {
      return ++answer;
    }
    if(range2[0] >= range1[0] && range2[0] <= range1[1]) {
      return ++answer;
    }
    if(range2[1] >= range1[0] && range2[1] <= range1[1]) {
      return ++answer;
    }

    return answer;
}
int main() {
  std::ifstream input_file("input.txt");
  std::string line;
  int part1Answer = 0;
  int part2Answer = 0;
  
  while(input_file >> line) {
    size_t comma_location = line.find(',');

    std::vector<int> range1 = returnNumberRange(line.substr(0, comma_location));
    std::vector<int> range2 = returnNumberRange(line.substr(comma_location + 1));

    part1Answer = checkRangePart1(range1, range2, part1Answer);
    part2Answer = checkRangePart2(range1, range2, part2Answer);

  }

  std::cout << "Answer Part1: " << part1Answer << std::endl;
  std::cout << "Answer Part2: " << part2Answer << std::endl;
  return 0;
}