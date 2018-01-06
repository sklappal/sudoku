#include <iostream>
#include <string>
#include "tests.cpp"

int main(int argc, char* argv[])
{
  if (argc == 2 && std::string(argv[1]) == "test")
  {
    tests::RunTests();
  }
  return 0;
}