#ifndef _SIMPLEROUTINGRECEIVERS_H_
#define _SIMPLEROUTINGRECEIVERS_H_

#include <list>

class SimpleRoutingReceivers
{
private:
    std::list<int> receivers;

public:
    void setReceivers(std::list<int>);
    std::list<int> getReceivers();
};

#endif