#ifndef EXAMPLE15_HPP
#define EXAMPLE15_HPP


#include "baseexample.hpp"



class Example15 : public BaseExample
{
public:
    Example15() : BaseExample("Example 15 / Hello") {}


public:
    int run() override;
    Info get_info() const override;

};



#endif // EXAMPLE15_HPP
