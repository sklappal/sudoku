#include <chrono>
#include <iostream>

#include "board.h"
#include "timer.h"

namespace {
  int passCount = 0;
  int failCount = 0;
}
namespace tests
{
  void testAssert(bool b)
  {
    if (b)
    {
      passCount++;
    } 
    else
    {
      failCount++;
    }  
  }

  void Board_is_initialized_empty()
  {
    auto board = sudoku::board();
    testAssert(board.unknownCount() == board.c_boardSize);
  }

  void Empty_board_is_valid()
  {
    auto board = sudoku::board();
    testAssert(board.isValid());
  }

  void Valid_board_is_valid()
  {
    auto runTest = [](int row, int col, int val)
    {
      auto board = sudoku::board();
      board.setValue(row, col, val);
      testAssert(board.isValid());
    };

    runTest(0, 1, 2);
    runTest(0, 2, 3);
    runTest(0, 3, 4);
    runTest(0, 4, 5);
    runTest(0, 5, 6);
    runTest(0, 6, 7); 
  }

  void Valid_complete_board_is_valid()
  {
    char vals[] = 
      {1,2,3,4,5,6,7,8,9,
      4,5,6,7,8,9,1,2,3,
      7,8,9,1,2,3,4,5,6,
      2,3,4,5,6,7,8,9,1,
      5,6,7,8,9,1,2,3,4,
      8,9,1,2,3,4,5,6,7,
      3,4,5,6,7,8,9,1,2,
      6,7,8,9,1,2,3,4,5,
      9,1,2,3,4,5,6,7,8};
    auto board = sudoku::board(vals);
    testAssert(board.isValid());
  }

  void Performance_valid_board()
  {
    char vals[] = 
      {1,2,3,4,5,6,7,8,9,
      4,5,6,7,8,9,1,2,3,
      7,8,9,1,2,3,4,5,6,
      2,3,4,5,6,7,8,9,1,
      5,6,7,8,9,1,2,3,4,
      8,9,1,2,3,4,5,6,7,
      3,4,5,6,7,8,9,1,2,
      6,7,8,9,1,2,3,4,5,
      9,1,2,3,4,5,6,7,8};
    auto board = sudoku::board(vals);
    auto t = common::timer("1e5 iterations");
    for (int i = 0; i < 1e5; i++)
      board.isValid();
    
  }

  void Row_invalid_board_is_invalid()
  {

    auto runTest = [](int col1, int col2, int row, int val)
    {
      auto board = sudoku::board();
      board.setValue(row, col1, val);
      board.setValue(row, col2, val);  
      testAssert(!board.isValid());      
    };

    runTest(0, 1, 0, 1);
    runTest(0, 1, 0, 2);
    runTest(0, 1, 3, 7);
    runTest(0, 1, 8, 6);
    runTest(0, 1, 0, 3);
    runTest(6, 7, 0, 8);
    runTest(4, 8, 8, 2);
    
  }

  void Col_invalid_board_is_invalid()
  {

    auto runTest = [](int row1, int row2, int col, int val)
    {
      auto board = sudoku::board();
      board.setValue(row1, col, val);
      board.setValue(row2, col, val);  
      testAssert(!board.isValid());      
    };

    runTest(0, 1, 0, 1);
    runTest(7, 8, 0, 2);
    runTest(2, 4, 3, 7);
    runTest(4, 1, 8, 6);
    runTest(3, 2, 0, 3);
    runTest(6, 7, 0, 8);
    runTest(4, 8, 8, 2);    
  }

  void Region_invalid_board_is_invalid()
  {

    auto runTest = [](int row1, int col1, int row2, int col2, int val)
    {
      auto board = sudoku::board();
      board.setValue(row1, col1, val);
      board.setValue(row2, col2, val);  
      testAssert(!board.isValid());      
    };

    runTest(0, 0, 1, 1, 7);
    runTest(8, 8, 6, 6, 2);
    runTest(3, 3, 4, 4, 7);
    runTest(4, 4, 5, 5, 1);
    runTest(6, 6, 7, 7, 3);
  }

  void RunTests()
  {
    Board_is_initialized_empty();
    Empty_board_is_valid();
    Valid_board_is_valid();
    Valid_complete_board_is_valid();
    Performance_valid_board();
    Row_invalid_board_is_invalid();
    Col_invalid_board_is_invalid();
    Region_invalid_board_is_invalid();
  
    std::cout << "Tests succesfully run. Pass ratio: " << (100.0 * passCount / double(passCount + failCount));
    std::cout << "% (total: " <<  passCount + failCount << " pass: " << passCount << " fail: " << failCount << ")." << std::endl;
  }
}