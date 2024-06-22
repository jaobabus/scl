#ifndef EXAMPLE11_HPP
#define EXAMPLE11_HPP


#include "baseexample.hpp"



class Example11 : public BaseExample
{
public:
    Example11() : BaseExample("Example 11 / Hello") {}


public:
    int run() override;
    Info get_info() const override;

};



#endif // EXAMPLE11_HPP
