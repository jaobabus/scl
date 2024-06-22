#ifndef EXAMPLE6_HPP
#define EXAMPLE6_HPP


#include "baseexample.hpp"



class Example6 : public BaseExample
{
public:
    Example6() : BaseExample("Example 6 / Hello") {}


public:
    int run() override;
    Info get_info() const override;

};



#endif // EXAMPLE6_HPP
