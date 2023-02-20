#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cmath>
#include <algorithm>

std::vector<std::string> readFile(const std::string& input)
{
    std::ifstream inputFile(input);
    std::string line;
    std::vector<std::string> lines;

    while(std::getline(inputFile, line))
    {
        lines.push_back(line);
    }

    return lines;
}

int64_t snafuToDecimal(const std::string& inputSnafu)
{
    int64_t decimalResult = 0;
    int64_t power = 5;
    int64_t exp = 0;

    for(int64_t i = inputSnafu.size() - 1; i >= 0; i--)
    {
        char digit = inputSnafu[i];
        switch(digit)
        {
            case '0':
                decimalResult += 0 * pow(power, exp);
                break;
            case '1':
                decimalResult += 1 * pow(power, exp);
                break;
            case '2':
                decimalResult += 2 * pow(power, exp);
                break;
            case '-':
                decimalResult += -1 * pow(power, exp);
                break;
            case '=':
                decimalResult += -2 * pow(power, exp);
                break;
            default:
                std::cout << "This shouldn't happen." << std::endl;
        }
        exp++;
    }

    return decimalResult;
}

std::string toBase5(int64_t inputNumber)
{
   std::string number;
   while(inputNumber > 0)
   {
       int64_t remainder = inputNumber % 5;
       number.push_back(remainder + '0');
       inputNumber = inputNumber / 5;
   }

   return number;
}

std::string numberToSnafu(std::string inputNumber)
{
   std::string result;
   for(int64_t i = inputNumber.size() - 1; i >= 0; i--) {
       int64_t remainder = (inputNumber[i] - '0') % 5;
       if (remainder == 0 || remainder == 1 || remainder == 2) {
           result.push_back(remainder + '0');
       } else if (remainder == 3) {
           result.push_back('=');
           if (i != 0) {
               inputNumber[i - 1] += 1;
           } else {
               inputNumber = "1";
           }
       } else {
           result.push_back('-');
           if (i != 0) {
               inputNumber[i - 1] += 1;
           } else {
               inputNumber = "1";
           }
       }


       auto j = i - 1;
       while (j >= 0 && inputNumber[j] == '5') {
           inputNumber[j] = '0';
           if (j >= 1) {
               inputNumber[j - 1] += 1;
           } else {
               inputNumber = '1' + inputNumber;
               inputNumber[1] = '0';
           }
           j--;
       }
   }

   std::reverse(result.begin(), result.end());
   return result;
}

std::string base10ToSnafu(int64_t inputNumber)
{
    std::string result = "";
    int64_t f = 1;
    int64_t i = 0;
    double fd = (double)inputNumber;
    for(; fd > 0; i++, f*=5)
    {
        fd = round(fd/5);
    }

    f/=5;

    for(int64_t j = 0; j < i; j++, f/=5)
    {
        int64_t num = (int64_t)round(inputNumber/(double)f);
        inputNumber -= (num * f);
        switch(num)
        {
            case -2:
                result += "=";
                break;
            case -1:
                result += "-";
                break;
            case 0:
                result += "0";
                break;
            case 1:
                result += "1";
                break;
            case 2:
                result += "2";
                break;
        }
    }

    return result;
}
int main() {
    std::string input = "input.txt";
    std::vector<std::string> snafuNumbers = readFile(input);
    std::vector<int64_t> convertedNumbers;
    for(auto& snafuNumber : snafuNumbers)
    {
        convertedNumbers.push_back(snafuToDecimal(snafuNumber));
    }


    int64_t sumBase10 = 0;

    for(auto& convertedNumber : convertedNumbers)
    {

        sumBase10 += convertedNumber;
    }
    std::cout << "Final Answer: " << base10ToSnafu(sumBase10) << std::endl;

    return 0;
}
