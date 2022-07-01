#include "ReceiversContainer.h"

std::list<int> ReceiversContainer::getReceivers()
{
    return receivers;
}

void ReceiversContainer::setReceivers(std::list<int> receivers)
{
    this->receivers = receivers;
}