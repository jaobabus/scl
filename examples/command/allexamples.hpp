#ifndef ALLEXAMPLES_HPP
#define ALLEXAMPLES_HPP


#include <vector>

#include "exs/example01.hpp"
#include "exs/example02.hpp"
#include "exs/example03.hpp"
#include "exs/example04.hpp"
#include "exs/example05.hpp"
#include "exs/example06.hpp"
#include "exs/example07.hpp"
#include "exs/example08.hpp"
#include "exs/example09.hpp"
#include "exs/example10.hpp"
#include "exs/example11.hpp"
#include "exs/example12.hpp"
#include "exs/example13.hpp"
#include "exs/example14.hpp"
#include "exs/example15.hpp"


inline
std::vector<BaseExample*> examples = {
    new Example1(),
    new Example2(),
    new Example3(),
    new Example4(),
    new Example5(),
    new Example6(),
    new Example7(),
    new Example8(),
    new Example9(),
    new Example10(),
    new Example11(),
    new Example12(),
    new Example13(),
    new Example14(),
    new Example15(),
};



#endif // ALLEXAMPLES_HPP
