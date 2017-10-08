// main.cpp

#include <iostream>
using namespace std;

#include "foo.hpp"

int main(int argc, char** argv)
{
  cout << "cmake hello world" << endl;

  Foo foo;
  foo.print();
}
