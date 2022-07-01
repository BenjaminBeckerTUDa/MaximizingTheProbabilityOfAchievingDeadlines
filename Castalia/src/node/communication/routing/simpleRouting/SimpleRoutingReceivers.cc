#include "SimpleRoutingReceivers.h"

std::list<int> SimpleRoutingReceivers::getReceivers()
{
    return receivers;
}

void SimpleRoutingReceivers::setReceivers(std::list<int> receivers)
{
    this->receivers = receivers;
}