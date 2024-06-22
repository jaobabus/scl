#ifndef EXAMPLE12_HPP
#define EXAMPLE12_HPP


#include "baseexample.hpp"



class Example12 : public BaseExample
{
public:
    Example12() : BaseExample("Example 12 / Hello") {}


public:
    int run() override;
    Info get_info() const override;

};



#endif // EXAMPLE12_HPP
