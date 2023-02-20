#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <vector>
#include <algorithm>

struct point
{
    int x;
    int y;

    inline bool operator==(const point &rhs)
    {
        return (this->x == rhs.x) && (this->y == rhs.y);
    }
};

class Cave {
    public:

    point stringToPoint(std::string str)
    {
        point result;

        result.x = stoi(str.substr(0, str.find(',')));
        result.y = stoi(str.substr(str.find(',') + 1));


        return result;
    }
    int getMaxY()
    {
        using mapPair = decltype(caveLayout)::value_type;
        auto maxY = std::max_element(caveLayout.begin(), caveLayout.end(),
                                     [] (const mapPair &y1, const mapPair &y2)
                                     { return y1.first < y2.first; } );
        return maxY->first;
    }
        void readRock(std::string line)
        {
            std::stringstream ss(line);
            std::string s, s2;
            std::vector<std::string> result;
            while(std::getline(ss, s, ' '))
            {
                if(s == "->") { continue;}
                else
                {
                    result.push_back(s);
                }
                
            }
            auto start = stringToPoint(result[0]);
            for(size_t i = 1; i < result.size(); i++)
            {
                
                auto end = stringToPoint(result[i]);
                //send coordinates to addRockLine
                addRockLine(start, end);
                start = end;
            }

        }

        void addRockLine(point startCoord, point endCoord)
        {
            auto fromX = startCoord.x;
            auto fromY = startCoord.y;
            auto toX = endCoord.x;
            auto toY = endCoord.y;

            if(fromY == toY)
            {
                const auto minX =  std::min(fromX, toX);
                const auto maxX =  std::max(fromX, toX);

                for(auto i = minX; i <= maxX; ++i)
                {
                    caveLayout[fromY][i] = material::Rock;
                }
            }
            else if (fromX == toX)
            {
                const auto minY = std::min(fromY, toY);
                const auto maxY = std::max(fromY, toY);

                for(auto i = minY; i <= maxY; ++i)
                {
                    caveLayout[i][fromX] = material::Rock;
                }
            }
            else
            {
                std::cout << "Nothing added for point: " << fromX << " " << fromY << " -> " << toX << " " << toY << std::endl;
            }


        }

        void printCaveLayout()
        {
            int maxY = getMaxY();
            int minX = caveLayout.begin()->second.begin()->first;
            int maxX = 0;
            for(auto row : caveLayout)
            {
                for(auto xPos : row.second)
                {
                    if(xPos.first < minX && xPos.second == material::Rock)
                    {
                        minX = xPos.first;
                    }
                    if(xPos.first > maxX)
                    {
                        maxX = xPos.first;
                    }
                }
            }
            for(int i = 0; i <= maxY; i++)
            {
                for(int j = minX; j < maxX; j++)
                {
                    if(caveLayout[i][j] == material::Rock)
                    {
                        std::cout << "#";
                    }
                    if(caveLayout[i][j] == material::Sand)
                    {
                        std::cout << "S";
                    }
                    if(caveLayout[i][j] == material::Air)
                    {
                        std::cout << ".";
                    }
                }
                std::cout << std::endl;
            }
           
        }

        bool isCaveEmpty(const point &position)
        {
            auto row = caveLayout.find(position.y);
            if(row != caveLayout.end())
            {
                const auto col = row->second.find(position.x);
                if(col != row->second.end())
                {
                    return (col->second == material::Air);
                }
            }
            return true;
        }

        void settleSand(const point position)
        {
            caveLayout[position.y][position.x] = material::Sand;

        }



    private:

    enum class material
    {
        Air,
        Rock,
        Sand
    };


    std::map<int, std::map<int, material>> caveLayout; //[Y, X, Material]
};

point flow(Cave &cave, const point& position)
{
    const auto& xPos = position.x;
    const auto& yPos = position.y;

    if((yPos > 1000) || (!cave.isCaveEmpty(position)))
    {
        throw std::out_of_range("");
    }

    if(const auto testPos = point{xPos, (yPos + 1)}; cave.isCaveEmpty((testPos)))
    {
        return testPos;
    }
    else if (const auto testPos = point {xPos - 1, (yPos + 1)}; cave.isCaveEmpty(testPos))
    {
        return testPos;
    }
    else if (const auto testPos = point {xPos + 1, (yPos + 1)}; cave.isCaveEmpty(testPos))
    {
        return testPos;
    }
    else
    {
        return position;
    }
}

void sandMovement(Cave &cave)
{
    point currentPos = point{500, 0};
    while(true)
    {
        point nextPos = flow(cave, currentPos);
        if(currentPos == nextPos)
        {
            cave.settleSand(currentPos);
            break;
        }

        currentPos = nextPos;
    }


}

int sandFlow(Cave &cave, int startStep)
{
    try
    {
        while(true)
        {
            sandMovement(cave);
            startStep++;
        }
    }
    catch(const std::out_of_range&)
    {

    }

    return startStep;
}

int main()
{
    std::ifstream inputFile("input.txt");
    std::string line;

    Cave cave{};
    while(std::getline(inputFile, line))
    {
        cave.readRock(line);
        
    }

    auto answerPart1 = sandFlow(cave, 0);

    std::cout << "\nAnswer to Part 1: " << answerPart1 << std::endl;

    cave.addRockLine(point { (498 - cave.getMaxY()), (cave.getMaxY() + 2)},
                       point {(502 + cave.getMaxY()), (cave.getMaxY() + 2)} );

    auto answerPart2 = sandFlow(cave, answerPart1);
    std::cout << "Answer to Part 2: " << answerPart2 << std::endl;

    //cave.printCaveLayout();
    return 0;
}