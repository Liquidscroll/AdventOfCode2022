
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <bits/stdc++.h>
#include <algorithm>

const int WIN = 6;
const int DRAW = 3;

enum Primary{
	A = 1,
	B,
	C
};

enum Secondary{
	X = 1,
	Y,
	Z
};

int answerOne(int answer, char PValue, char SValue)
{
	switch(SValue)
		{
			case 'X':
				switch(PValue)
				{
					case 'A':
					answer += (X + DRAW);
					break;
					case 'B':
					answer += X;
					break;
					case 'C':
					answer += (X + WIN);
					break;
				}
				break;
			case 'Y':
				switch(PValue)
				{
					case 'A':
					answer += (Y + WIN);
					break;
					case 'B':
					answer += (Y + DRAW);
					break;
					case 'C':
					answer += Y;
					break;
				}
				break;
			case 'Z':
				switch(PValue)
				{
					case 'A':
					answer += Z;
					break;
					case 'B':
					answer += (Z + WIN);
					break;
					case 'C':
					answer += (Z + DRAW);
					break;
				}
				break;
		}
		return answer;
}

	
int main()
{
	std::ifstream file("input.txt");
	std::string str;
	int answer1 = 0, answer2 = 0;
	int counter = 0;
	char PValue1, SValue1;
	
	char PValue2[3] = {'A', 'B', 'C'};
	char SValue2[3] = {'X', 'Y', 'Z'};
	
	int comboValues[] = {3, 4, 8, 1, 5, 9, 2, 6, 7};
	
	std::map<std::string, int> outcomes;
	
	for(int i = 0; i < 3; i++)
	{
		for(int j = 0; j < 3; j++)
		{
			std::string str = std::string(1, PValue2[i]) + ' ' + SValue2[j];
			outcomes.insert(std::pair<std::string, int>(str, comboValues[counter]));
			counter++;
		}
	}

	while (std::getline(file, str)) {
		
		PValue1 = str[0];
		SValue1 = str[2];
		/*
		A - ROCK - X
		B - PAPER - Y
		C - SCISSORS - Z
		*/
		answer1 = answerOne(answer1, PValue1, SValue1);
		
		std::map<std::string, int>::iterator itr;
		for(itr = outcomes.begin(); itr!= outcomes.end(); itr++)
		{
			if(str == itr->first)
			{
				answer2 += itr->second;
			}
		}
	}
	std::cout << "Answer to Part 1: " << answer1 << std::endl;
	std::cout << "Answer to Part 2: " << answer2 << std::endl;
	file.close();
	return 0;
}