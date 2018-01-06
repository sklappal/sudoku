#include <algorithm>
#include <cassert>
#include <iostream>
#include <string>

#include "board.h"

namespace sudoku
{
  const int log = false;
  board::board()
  {
    m_vals = std::vector<char>(c_boardSize, c_emptyValue);
  }

  board::board(const board& b)
  {
    m_vals = std::vector<char>(b.m_vals.begin(), b.m_vals.end());
  }

  board::board(char vals[])
  {
    m_vals = std::vector<char>(c_boardSize, c_emptyValue);
    for (int i = 0; i < 9; i++)
    {
      for (int j = 0; j < 9; j++)
      {
        setValue(i, j, vals[i * 9 + j]);
      }
    }
  }

  int board::unknownCount() const
  {
    return std::count (m_vals.begin(), m_vals.end(), c_emptyValue);
  }

  int board::getValue(int row, int col) const
  {
    assert(row >= 0 && row <= 8);
    assert(col >= 0 && col <= 8);

    return m_vals[row * 9 + col];
  }

  void board::setValue(int row, int col, int val)
  {
    assert(row >= 0 && row <= 8);
    assert(col >= 0 && col <= 8);
    assert(val >= 1 && val <= 9);

    m_vals[row * 9 + col] = val;
  }

  namespace
  {
    bool sectionIsValid(const char vals[])
    {
      bool found[9] = {
        false, false, false, 
        false, false, false, 
        false, false, false};
      for(int i = 0; i < 9; i++)
      {
        if (vals[i] != 0)
        {
          if (found[vals[i]-1])
          {
            return false;
          }
          found[vals[i]-1] = true;
        }
      }
      return true;
    }

    bool rowIsValid(const std::vector<char> board, int row)
    {
      assert(row >= 0 && row <= 8);
      char section[9] = {};
      for (int i = 0; i < 9; i++)
      {
        section[i] = board[row * 9 + i];
      }
      return sectionIsValid(section);
    }
    
    bool colIsValid(const std::vector<char> board, int col)
    {
      assert(col >= 0 && col <= 8);
      char section[9] = {};
      for (int i = 0; i < 9; i++)
      {
        section[i] = board[i * 9 + col];
      }
      return sectionIsValid(section);
    }
    
    bool regionIsValid(const std::vector<char> board, int region)
    {
      assert(region >= 0 && region <= 8);
      int colStart = (region % 3) * 3;
      int rowStart = region / 3;
      
      char section[9] = {
        board[(3 * rowStart + 0) * 9 + colStart + 0],
        board[(3 * rowStart + 0) * 9 + colStart + 1],
        board[(3 * rowStart + 0) * 9 + colStart + 2],

        board[(3 * rowStart + 1) * 9 + colStart + 0],
        board[(3 * rowStart + 1) * 9 + colStart + 1],
        board[(3 * rowStart + 1) * 9 + colStart + 2],

        board[(3 * rowStart + 2) * 9 + colStart + 0],
        board[(3 * rowStart + 2) * 9 + colStart + 1],
        board[(3 * rowStart + 2) * 9 + colStart + 2],
      };
      
      return sectionIsValid(section);
    }

    void Log(const std::string& s)
    {
      if (log)
        std::cout << s << std::endl;
    }
  }


  bool board::isValid() const
  {
    for (int i = 0; i < 9; i++)
    {
      Log("hoo");
      if (!rowIsValid(m_vals, i))
      {
        Log("not valid row");
        return false;
      }
      
      if (!colIsValid(m_vals, i))
      {
        Log("not valid col");
        return false;
      }
      
      if (!regionIsValid(m_vals, i))
      {
        Log("not valid region");
        return false;
      }
    }

    return true;
  }

  void board::print() const
  {
    for (int i = 0; i < 9; i++)
    {
      for (int j = 0; j < 9; j++)
      {
        auto val = m_vals[i*9 + j];
        std::cout << (val != 0 ? char(48 + val) : '_') << " ";
      }
      std::cout << std::endl;
    }
  }
}