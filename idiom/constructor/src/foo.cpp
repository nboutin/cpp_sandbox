//

#include "foo.hpp"

#include <iostream>
using namespace std;

Foo::Foo() { cout << __PRETTY_FUNCTION__ << endl; }

Foo::Foo(const Foo& foo) { cout << __PRETTY_FUNCTION__ << endl; }

Foo& Foo::operator=(const Foo& foo) { cout << __PRETTY_FUNCTION__ << endl; }
