#ifndef _RECEIVERSCONTAINER_H_
#define _RECEIVERSCONTAINER_H_

#include <list>

class ReceiversContainer
{
private:
    std::list<int> receivers;

public:
    void setReceivers(std::list<int>);
    std::list<int> getReceivers();
};

#endif