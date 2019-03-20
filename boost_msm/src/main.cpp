
#include <iostream>

#include "foo.h"

int main()
{
    foo::Foo fsm;
    fsm.do_close();
    fsm.do_open();

    return 0;
}
