// foo.cpp

#include "foo.hpp"

#include <iostream>
using namespace std;

void Foo::print() { cout << __PRETTY_FUNCTION__ << endl; }
