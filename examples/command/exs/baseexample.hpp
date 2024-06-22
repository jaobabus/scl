#ifndef BASEEXAMPLE_HPP
#define BASEEXAMPLE_HPP


#include <string>




class BaseExample
{
public:
    struct Info
    {
        bool block_terminal;
    };

public:
    BaseExample(const char* name)
        : name(name) {}

public:
    virtual int run() = 0;


public:
    virtual Info get_info() const = 0;

public:
    const char* const name;

};



#endif // BASEEXAMPLE_HPP
