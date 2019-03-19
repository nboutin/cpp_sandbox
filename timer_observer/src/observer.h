
#pragma once

#include <vector>

template<typename T>
class Observer
{
public:
    Observer()          = default;
    virtual ~Observer() = default;

    virtual void update(const T& data) const = 0;
};

template<typename T>
class Observable
{
public:
    Observable() = default;

    void subscribe(const Observer<T>& observer) { listeners.push_back(&observer); }

    void notify(const T& data) const
    {
        for(const auto l : listeners)
        {
            l->update(data);
        }
    }

private:
    std::vector<const Observer<T>*> listeners;
};
