#ifndef EXAMPLE10_HPP
#define EXAMPLE10_HPP


#include "baseexample.hpp"



class Example10 : public BaseExample
{
public:
    Example10() : BaseExample("Example 10 / Hello") {}


public:
    int run() override;
    Info get_info() const override;

};



#endif // EXAMPLE10_HPP
