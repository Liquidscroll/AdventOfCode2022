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
   /*Function that will take the position of a tree and see if all other trees between it and an edge of the grid
   are shorter than it and thus if it is visible from the edge of the grid.*/
 int gridSize = gridMatrix.size();
 bool treeVisible = false; 
 //std::cout << "Here at: " << row << "," << col << std::endl;
  //iterate over row
  for(int i = row - 1; i >= 0; i--)
  {
   // std::cout << "current: " << gridMatrix[i][col] << std::endl;
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

    //std::cout << "current: " << gridMatrix[i][col] << std::endl;
    if(gridMatrix[i][col] >= size)
    {
        treeVisible = false;
        break;
    } else if (i == gridSize - 1 && gridMatrix[i][col] < size)
    {
        return true;
    }  
  }

  //iterate over column
  for(int i = col - 1; i >= 0; i--)
  {
   // std::cout << "current: " << gridMatrix[i][col] << std::endl;
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
    //std::cout << "current: " << gridMatrix[i][col] << std::endl;
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

int visibleTreesPart1(std::vector<std::vector<int>> const &gridMatrix)
{
    int treeCounter;
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
                std::cout << std::boolalpha << "countVisible return: " << countVisible(gridMatrix, i, j, gridMatrix[i][j]) << " at position: " << i << "," << j << std::endl;
                if(countVisible(gridMatrix, i, j, gridMatrix[i][j])) 
                { 
                    std::cout << "here1" << std::endl; 
                    treeCounter++;
                }
            }

        }
    }
    return treeCounter;
}

int main()
{
    std::ifstream input_file("inputexample.txt");
    std::string line;
    std::vector<std::vector<int>> inputGrid;
    std::vector<int> row;
    int counter = 0;
    while(input_file >> line)
    {
        row.clear();
        for(auto i : line)
        {
            row.push_back(i - '0');;
        }
        inputGrid.push_back(row);
    }

    printGrid(inputGrid);
    counter = visibleTreesPart1(inputGrid);
    //counter += (inputGrid.size() * 4 - 4);
    std::cout << "Answer Part 1: " << counter << std::endl;

  return 0;
}