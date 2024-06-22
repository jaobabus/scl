#ifndef EXAMPLE9_HPP
#define EXAMPLE9_HPP


#include "baseexample.hpp"



class Example9 : public BaseExample
{
public:
    Example9() : BaseExample("Example 9 / Hello") {}


public:
    int run() override;
    Info get_info() const override;

};



#endif // EXAMPLE9_HPP
