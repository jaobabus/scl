#ifndef EXAMPLE4_HPP
#define EXAMPLE4_HPP


#include "baseexample.hpp"



class Example4 : public BaseExample
{
public:
    Example4() : BaseExample("Example 4 / Colored console input") {}


public:
    int run() override;
    Info get_info() const override;

};



#endif // EXAMPLE4_HPP
