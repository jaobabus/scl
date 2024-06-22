#ifndef EXAMPLE13_HPP
#define EXAMPLE13_HPP


#include "baseexample.hpp"



class Example13 : public BaseExample
{
public:
    Example13() : BaseExample("Example 13 / Hello") {}


public:
    int run() override;
    Info get_info() const override;

};



#endif // EXAMPLE13_HPP
