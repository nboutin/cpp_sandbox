
#pragma once

#include "observer.h"

#include <iostream>

class Measure : public Observer<bool>
{
public:
    Measure(const std::string& name) : name(name) {}

private:
    void update(const bool& data) const final
    {
        using namespace std;
        cout << name << endl;
    }

    std::string name;
};
