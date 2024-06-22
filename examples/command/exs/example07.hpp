#ifndef EXAMPLE7_HPP
#define EXAMPLE7_HPP


#include "baseexample.hpp"



class Example7 : public BaseExample
{
public:
    Example7() : BaseExample("Example 7 / Hello") {}


public:
    int run() override;
    Info get_info() const override;

};



#endif // EXAMPLE7_HPP
