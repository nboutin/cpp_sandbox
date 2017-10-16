//

class Foo
{
public:
  Foo();                          // default constructor
  Foo(const Foo& foo);            // copy constructor
  Foo& operator=(const Foo& foo); // copy assignment
};
