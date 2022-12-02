#include <iostream>
#include <fstream>
//#include <bits/stdc++.h>

int * CompareResults(int values [3], int result)
{
		if (result > values[0])
		{
			values[0] = result;
		}
		
		std::sort(values, values + 3);
		return values;
}

int main()
{
	std::ifstream file("input.txt");
	std::string str;
	int elvesCalories[3] = {};
	int result = 0;
	int answer = 0;

	while (std::getline(file, str)) {
		if (str == "")
		{
			CompareResults(elvesCalories, result);
			result = 0;
		}
		if (str != "")
		{
		result += std::stoi(str);
		}
	}
	
	for (int i = 0; i < 3; i++)
	{
	answer += elvesCalories[i];
	}
	
	std::cout << "Answer: " << answer << std::endl;

	file.close();
	return 0;
}