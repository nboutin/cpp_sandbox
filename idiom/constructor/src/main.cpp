// main.cpp

#include <iostream>
using namespace std;

#include "foo.hpp"

int main(int argc, char** argv)
{
  cout << "default constructor" << endl;
  Foo f;

  cout << "copy constructor" << endl;
  Foo f1(f);

  Foo a, b;
  cout << "copy assignment" << endl;
  a = b;
}
