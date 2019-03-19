
#include "arguments.h"

#include <iostream>

int main(int argc, char* argv[])
{
    using namespace std;

    auto args = parse_args(argc, argv);

    if(args)
    {
        cout << "config_pathname:" << args.value().config_pathname << endl;
    }
}
