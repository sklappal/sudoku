#include <vector>

namespace sudoku
{
  class board
  {

    public:
      const int c_boardSize = 81;

      board();
      board(const board&);
      board(char[]);

      int unknownCount() const;

      // Does not contain contradiction
      bool isValid() const;

      // Rows and Cols are zero based. Region ordering is zero based row first.
      int getValue(int row, int col) const;

      void setValue(int row, int col, int value);

      void print() const;

    private:
      
      const char c_emptyValue = 0;

      std::vector<char> m_vals;
  };
}