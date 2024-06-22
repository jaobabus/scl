#ifndef EXAMPLE8_HPP
#define EXAMPLE8_HPP


#include "baseexample.hpp"



class Example8 : public BaseExample
{
public:
    Example8() : BaseExample("Example 8 / Hello") {}


public:
    int run() override;
    Info get_info() const override;

};



#endif // EXAMPLE8_HPP
