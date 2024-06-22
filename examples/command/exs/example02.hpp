#ifndef EXAMPLE2_HPP
#define EXAMPLE2_HPP


#include "baseexample.hpp"



class Example2 : public BaseExample
{
public:
    Example2() : BaseExample("Example 2 / Colorize simple string") {}


public:
    int run() override;
    Info get_info() const override;

};



#endif // EXAMPLE2_HPP
