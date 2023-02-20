#include <bits/stdc++.h>

std::vector<std::string> splitString(std::string s)
{
    s = s.substr(1, s.size() - 2);

    std::vector<std::string> ans;
    int depth = 0;
    std::string temp = "";

    for(int i = 0; i < (int)s.size(); i++)
    {
        if(s[i] == '[')
        {
            depth++;
        }
        else if (s[i] == ']')
        {
        depth--;
        }
        else if (s[i] == ',' && depth == 0)
        {
            ans.push_back(temp);
            temp.clear();
        }
        if(s[i] != ',' || (s[i] == ',' && depth != 0))
        {
            temp.push_back(s[i]);
        }
    }

    if(temp.size() != 0) { ans.push_back(temp); }

    return ans;

}




int compareLists(std::vector<std::string> lhs, std::vector<std::string> rhs)
{
    for(int i = 0; i < (int)std::min(lhs.size(), rhs.size()); i++)
    {
        if(lhs[i][0] != '[' && rhs[i][0] != '[')
        {
            if(stoi(lhs[i]) - stoi(rhs[i]) != 0)
            {
                return stoi(lhs[i]) - stoi(rhs[i]);
            }
        }
        else if (lhs[i][0] == '[' && rhs[i][0] != '[')
        {
            int x = compareLists(splitString(lhs[i]), splitString("[" + rhs[i] + "]"));

            if(x != 0) { return x; }
        }
        else if (lhs[i][0] != '[' && rhs[i][0] == '[')
        {
            int x = compareLists(splitString("[" + lhs[i] + "]"), splitString(rhs[i]));
            if (x != 0) { return x; }
        }
        else
        {
            int x  = compareLists(splitString(lhs[i]), splitString(rhs[i]));
            if(x != 0) { return x; }
        }
    }

    return lhs.size() - rhs.size();
}

int main()
{
    std::ifstream inputFile("input.txt");

    std::string line, line2;
    std::vector<std::string> packets = {"[[2]]", "[[6]]"};

    int index = 1, answerPart1 = 0, answerPart2 = 0;
    while(inputFile >> line >> line2)
    {
        packets.push_back(line);
        packets.push_back(line2);
        if(compareLists(splitString(line), splitString(line2)) < 0) answerPart1 += index;
        index++;
    }

    std::sort(packets.begin(), packets.end(), [](std::string s1, std::string s2)
    {
        return compareLists(splitString(s1), splitString(s2)) < 0;
    });

    auto decoderPos1 = std::find(packets.begin(), packets.end(), "[[2]]") - packets.begin() + 1;
    auto decoderPos2 = std::find(packets.begin(), packets.end(), "[[6]]") - packets.begin() + 1;

    answerPart2 = decoderPos1 * decoderPos2;

    std::cout << "\nAnswer to Part 1: " << answerPart1 << std::endl;
    std::cout << "Answer to Part 2: " << answerPart2 << std::endl;
    return 0;
}