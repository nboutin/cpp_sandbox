
#include <iostream>

// back-end
#include <boost/msm/back/state_machine.hpp>
// front-end
#include <boost/msm/front/state_machine_def.hpp>
// functors
#include <boost/msm/front/euml/common.hpp>
#include <boost/msm/front/functor_row.hpp>
// for And_ operator
#include <boost/msm/front/euml/operator.hpp>
// for func_state and func_state_machine
#include <boost/msm/front/euml/state_grammar.hpp>

namespace msm = boost::msm;
using namespace msm::front;
namespace mpl = boost::mpl;

#include "foo.h"

using namespace foo;

namespace foo
{
namespace
{
// Events
struct close
{};
struct open
{};

struct fsm_test_ : public msm::front::state_machine_def<fsm_test_>
{
    template<class Event, class FSM>
    void on_entry(Event const&, FSM&)
    {
        std::cout << "entering: fsm_test" << std::endl;
    }
    template<class Event, class FSM>
    void on_exit(Event const&, FSM&)
    {
        std::cout << "leaving: fsm_test" << std::endl;
    }

    struct Open_tag
    {};
    using Open = msm::front::euml::func_state<Open_tag>;

    //    struct Close_tag
    //    {};
    //    using Close = msm::front::euml::func_state<Close_tag>;

    struct Close : public msm::front::state<>
    {};

    using initial_state = Open;

    // clang-format off
	struct transition_table : mpl::vector<
		Row < Open, close, Close, none, none >,
		Row < Close, open, Open, none, none>
	>{};
    // clang-format on

    // Replaces the default no-transition response.
    template<class FSM, class Event>
    void no_transition(Event const& e, FSM&, int state)
    {
        std::cout << "no transition from state " << state << " on event " << typeid(e).name()
                  << std::endl;
    }
};

// Pick a back-end
using fsm_test = msm::back::state_machine<fsm_test_>;
}
}

Foo::Foo() : m_fsm(std::make_shared<fsm_test>())
{
    std::static_pointer_cast<fsm_test>(m_fsm)->start();
}

void Foo::do_open() { std::static_pointer_cast<fsm_test>(m_fsm)->process_event(open()); }
void Foo::do_close() { std::static_pointer_cast<fsm_test>(m_fsm)->process_event(close()); }
