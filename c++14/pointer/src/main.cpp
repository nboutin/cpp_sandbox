// main.cpp

#include <iostream>
using namespace std;

#include <memory>

class Foo
{
public:
  void print()
  {
    cout << __PRETTY_FUNCTION__ << endl;
  }
};

int main(int argc, char** argv)
{
  unique_ptr<Foo> foo = make_unique<Foo>();

  foo->print();
}
