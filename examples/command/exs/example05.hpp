#ifndef EXAMPLE5_HPP
#define EXAMPLE5_HPP


#include "baseexample.hpp"



class Example5 : public BaseExample
{
public:
    Example5() : BaseExample("Example 5 / Merge tokens") {}


public:
    int run() override;
    Info get_info() const override;

};



#endif // EXAMPLE5_HPP
