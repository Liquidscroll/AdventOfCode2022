#include <fstream>
#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include <numeric>
#include <sstream>

void printGrid(std::vector<std::vector<int>> const &gridMatrix)
{
    for (size_t i = 0; i < gridMatrix.size(); i++)
    {
        for (const auto &j : gridMatrix[i])
        {
            std::cout << j;
        }
        std::cout << std::endl;
    }
}

bool countVisible(std::vector<std::vector<int>> const &gridMatrix, size_t row, size_t col, int size)
{
 int gridSize = gridMatrix.size();
 bool treeVisible = false; 
  for(int i = row - 1; i >= 0; i--)
  {
    if(gridMatrix[i][col] >= size)
    {
        treeVisible = false;
        break;
    }
    else if (i == 0 && gridMatrix[i][col] < size)
    {
        return true;
    }
  }
  for(int i = row + 1; i < gridSize; i++)
  {
    if(gridMatrix[i][col] >= size)
    {
        treeVisible = false;
        break;
    } else if (i == gridSize - 1 && gridMatrix[i][col] < size)
    {
        return true;
    }  
  }
  for(int i = col - 1; i >= 0; i--)
  {
    if(gridMatrix[row][i] >= size)
    {
        treeVisible = false;
        break;
    } else if (i == 0 && gridMatrix[row][i] < size)
    {
        return true;
    }
  }
  for(int i = col + 1; i < gridSize; i++)
  {
    if(gridMatrix[row][i] >= size)
    {
        treeVisible = false;
        break;
    } else if (i== gridSize - 1 && gridMatrix[row][i] < size)
    {
        return true;
    }
  }
  return treeVisible;
}

int calcScenicScore(std::vector<std::vector<int>> const &gridMatrix, int col, int row, int size)
{
    int totalRows = gridMatrix.size();
    int totalCols = gridMatrix[0].size();
    int northScore = 0, southScore = 0, eastScore = 0, westScore = 0;

    for(int i = col - 1; i >= 0; i--)
    {
        if(gridMatrix[row][i] >= size)
        {
            northScore++;
            break;
        }
        if(gridMatrix[row][i] < size)
        {
            northScore++;
        }
    }

    for (int i = col + 1; i < totalCols; i++)
    {
        if(gridMatrix[row][i] >= size)
        {
            southScore++;
            break;
        }
        if(gridMatrix[row][i] < size)
        {
            southScore++;
        }
    }

    for (int i = row - 1; i >= 0; i --)
    {
        if(gridMatrix[i][col] >= size)
        {
            westScore++;
            break;
        }
        if(gridMatrix[i][col] < size)
        {
            westScore++;
        }
    }
    for(int i = row + 1; i < totalRows; i++)
    {
        if(gridMatrix[i][col] >= size)
        {
            eastScore++;
            break;
        }
        if(gridMatrix[i][col] < size)
        {
            eastScore++;
        }
    }

    return (northScore * southScore * eastScore * westScore);
}

int visibleTreesPart1(std::vector<std::vector<int>> const &gridMatrix)
{
    int treeCounter = 0;
    for(size_t i = 0; i < gridMatrix.size(); i++)
    {
        for(size_t j = 0; j < gridMatrix[i].size(); j++)
        {
            if(i == 0 || j == 0 || i == gridMatrix.size() - 1 || j == gridMatrix[i].size() - 1)
            {
               continue;
            }
            else
            {
                if(countVisible(gridMatrix, i, j, gridMatrix[i][j])) 
                { 
                    treeCounter++;
                }
            }

        }
    }
    return treeCounter;
}

int highestScenicScore(std::vector<std::vector<int>> const &gridMatrix)
{
    std::vector<int> scenicScoreArray;
    int highestScore = 0;
    int highestRow, highestCol;
    for(size_t i = 0; i < gridMatrix.size(); i++)
    {
        highestRow++;
        for(size_t j = 0; j < gridMatrix[i].size(); j++)
        {
            highestCol++;
            int scenicScore = calcScenicScore(gridMatrix, i, j, gridMatrix[i][j]);
            scenicScoreArray.push_back(scenicScore);
            if(scenicScore >= highestScore)
            {
                highestScore = scenicScore;
                std::cout << "Score: " << highestScore << "i: " << highestRow << "j: " << highestCol << std::endl;
            }
        }
    }
    return highestScore;
}

int main()
{
    std::ifstream input_file("input.txt");
    std::string line;
    std::vector<std::vector<int>> inputGrid;
    std::vector<int> row;
    int answer1 = 0, answer2 = 0;
    while(input_file >> line)
    {
        row.clear();
        for(auto i : line)
        {
            row.push_back(i - '0');;
        }
        inputGrid.push_back(row);
    }

    //printGrid(inputGrid);
    answer1 = visibleTreesPart1(inputGrid);
    answer1 += (inputGrid.size() * 4 - 4);
    answer2 = highestScenicScore(inputGrid);
    std::cout << "Answer Part 1: " << answer1 << std::endl;
    std::cout << "Answer Part 2: " << answer2 << std::endl;
    return 0;
}