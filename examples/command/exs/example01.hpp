#ifndef EXAMPLE1_HPP
#define EXAMPLE1_HPP


#include "baseexample.hpp"



class Example1 : public BaseExample
{
public:
    Example1() : BaseExample("Example 1 / Hello") {}


public:
    int run() override;
    Info get_info() const override;

};



#endif // EXAMPLE1_HPP
