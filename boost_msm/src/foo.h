
#pragma once

#include <memory>

namespace foo
{
class Foo
{
public:
    Foo();

    void do_open();
    void do_close();

private:
    std::shared_ptr<void> m_fsm;
};
}
