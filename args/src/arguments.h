
#pragma once

#include <string>
#include <optional>

struct arg_t
{
    std::string config_pathname;
};

std::optional<arg_t> parse_args(int argc, char* argv[]);
