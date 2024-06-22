#ifndef EXAMPLE14_HPP
#define EXAMPLE14_HPP


#include "baseexample.hpp"



class Example14 : public BaseExample
{
public:
    Example14() : BaseExample("Example 14 / Hello") {}


public:
    int run() override;
    Info get_info() const override;

};



#endif // EXAMPLE14_HPP
