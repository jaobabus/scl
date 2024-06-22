#ifndef EXAMPLE3_HPP
#define EXAMPLE3_HPP


#include "baseexample.hpp"



class Example3 : public BaseExample
{
public:
    Example3() : BaseExample("Example 3 / Hello") {}


public:
    int run() override;
    Info get_info() const override;

};



#endif // EXAMPLE3_HPP
