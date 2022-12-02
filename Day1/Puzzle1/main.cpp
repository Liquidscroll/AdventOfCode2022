#include <iostream>
#include <fstream>

int main()
{
	std::ifstream file("input.txt");
	std::string str;
	int result = 0;
	int answer = 0;

	while (std::getline(file, str)) {
		if (str == "")
		{
			result = 0;
		}
		if (str != "")
		{
		result += std::stoi(str);

		if (result > answer)
		{
			answer = result;
		}

		}
	}
	
	std::cout << "Answer is: " << answer << std::endl;

	file.close();
	return 0;
}