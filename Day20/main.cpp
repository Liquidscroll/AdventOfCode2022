#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <numeric>
#include <functional>
#include <queue>

struct inputNumber
{
    int64_t index, value;

    bool operator==(const inputNumber& otherNum)
    {
        return std::tuple(this->index, this->value) == std::tuple(otherNum.index, otherNum.value);
    }

    void print() const
    {
        std::cout << "(Idx: " << this->index
                  << ", Value: " << this->value << ")" << std::endl;
    }
};

int64_t mod(int64_t num1, int64_t num2)
{
    return (num1 % num2 + num2) % num2;
}

std::vector<inputNumber> moveNumbers(std::vector<inputNumber>& numbers, const inputNumber number)
{
    auto itr = std::find(numbers.begin(), numbers.end(), number);
    auto pos = mod(std::distance(numbers.begin(), itr) + number.value, numbers.size() - 1ULL);

    numbers.erase(itr);
    numbers.insert(numbers.begin() + pos, number);
    return numbers;
}

std::vector<inputNumber> mixNumbers(const std::vector<inputNumber>& codedNumbers, const std::vector<inputNumber>& origNumbers)
{
    std::vector<inputNumber> decodedNumbers = codedNumbers;
    for(auto& number : origNumbers)
    {
        decodedNumbers = moveNumbers(decodedNumbers, number);
    }

    return decodedNumbers;
}

auto interval(const std::vector<inputNumber>& numbers, int64_t zeroPos, int64_t interval)
{

    return numbers[(interval + zeroPos) % numbers.size()].value;
}

auto intervalSum(const std::vector<inputNumber>& numbers)
{
    auto zeroPos = std::distance(numbers.begin(), std::find_if(numbers.begin(), numbers.end(),
                                                             [](auto& number)
                                                             {
                                                                return number.value == 0;
                                                             }));

    return (interval(numbers, zeroPos, 1000)
            + interval(numbers, zeroPos, 2000)
            + interval(numbers, zeroPos, 3000));
}

void answerPart1(std::vector<inputNumber>& inputNumbers)
{
    std::vector<inputNumber> ans = inputNumbers;
    ans = mixNumbers(ans, ans);

    auto answer = intervalSum(ans);

    std::cout << "Answer Part 1: " << answer << std::endl;

}

void answerPart2(std::vector<inputNumber>& inputNumbers)
{
    std::vector<inputNumber> keyCoded = inputNumbers;
    int key = 811589153;
    for(auto& num : keyCoded)
    {
        num.value *= key;
    }

    int numRounds = 10;

    std::vector<inputNumber> decoded = keyCoded;

    for(int i = 0; i < numRounds; i++)
    {
        decoded = mixNumbers(decoded, keyCoded);
    }

    auto ans = intervalSum(decoded);

    std::cout << "Answer Part 2: " << ans << std::endl;

}
int main()
{
    std::ifstream inputFile("input.txt");

    std::vector<inputNumber> numbers, numbers2;
    std::string line;
    int64_t idx = 0, idx2 = 0;
    while(inputFile >> line)
    {
        numbers.push_back({idx++, stoi(line)});
        numbers2.push_back({idx2++, stoi(line)});
    }
    answerPart1(numbers);
    answerPart2(numbers2);
    return 0;
}

