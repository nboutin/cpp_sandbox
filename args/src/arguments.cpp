
#include "arguments.h"

#include <iostream>

#include <getopt.h>

constexpr auto ARGUMENTS = "c:hv";

constexpr option long_opts[] = {
    {"config", required_argument, 0, 'c'},
    {"version", no_argument, 0, 'v'},
    {"help", no_argument, 0, 'h'},
    {nullptr, 0, 0, 0},
};

std::string construct_version();
void print_help();

std::optional<arg_t> parse_args(int argc, char* argv[])
{
    using namespace std;

    int c = 0;
    arg_t args;

    while((c = getopt_long(argc, argv, ARGUMENTS, long_opts, nullptr)) != EOF)
    {
        switch(c)
        {
        case 'c': args.config_pathname = optarg; continue;
        case 'v': cout << construct_version() << endl; return {};

        case '?':
        case 'h': print_help(); return {};
        }
    }
    return {args};
}

// Add carriage return without flushing cout buffer as oppose to endl
std::ostream& nl(std::ostream& oss)
{
    oss << "\n";
    return oss;
}

std::ostream& nt(std::ostream& oss)
{
    oss << "\t";
    return oss;
}

std::string construct_version() { return "NAME VERSION by xxx"; }

void print_help()
{
    using namespace std;

    cout << nl << construct_version() << nl;
    cout << "Usage:" << nl;
    cout << nt << "-c, --config=" << nt << ": Configuration file pathname" << nl;
    cout << nt << "-v, --version" << nt << ": Display program version and exit" << nl;
    cout << nt << "-h, --help" << nt << ": Display this help message and exit" << nl;
    cout << flush;
}
