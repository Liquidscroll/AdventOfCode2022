#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <tuple>
#include <algorithm>
#include <array>
std::array<int, 3> &compareResults(std::array<int,3> &values, int result)
{
	if(result > values.front())
	{
		values.front() = result;
	}
	std::sort(values.begin(), values.end());
	return values;
}

std::tuple<int, int> answerOne(std::string inputString, int num, int ans)
{
	if(inputString == "")
	{
		num = 0;
	}
	if(inputString != "")
	{
		num += std::stoi(inputString);
	}
	if(num > ans)
	{
		ans = num;
	}
	
	return {num, ans};

}

int answerTwo(std::string inputString, int num, std::array<int, 3> &values)
{
	if(inputString == "")
	{
		values = compareResults(values, num);
		num = 0;
	}
	if (inputString != "")
	{
		num += std::stoi(inputString);
	}
	
	return num;
}
int main()
{
	std::ifstream file("input.txt");
	std::string str;
	int result1 = 0, result2 = 0;
	int answer1 = 0, answer2 = 0;
	std::array<int, 3> valuesArray[3] = {};

	while (std::getline(file, str)) {
		std::tie(result1, answer1) = answerOne(str, result1, answer1);
		result2 = answerTwo(str, result2, *valuesArray);
	}
	
	for(auto itr = valuesArray->begin(); itr != valuesArray->end(); itr++)
	{
		answer2 += *itr;
	}
	
	
	std::cout << "Answer is: " << answer1 << std::endl;
	std::cout << "Answer is: " << answer2 << std::endl;
	/*
	for (std::size_t i = 0, max = result.size(); i != max; i++)
	{
		std::cout << result[i] <<"\n";
	}
	*/
	file.close();
	return 0;
}