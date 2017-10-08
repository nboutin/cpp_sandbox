// foo.cpp

#include "foo.hpp"

#include <iostream>
using namespace std;

Foo::Foo()
{

}

void Foo::print()
{
	cout << __PRETTY_FUNCTION__ << endl;
}
