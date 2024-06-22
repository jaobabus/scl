
#include <iostream>

#include <textmap.hpp>
#include <consolewidget.hpp>

#include <boost/lexical_cast.hpp>
#include <boost/program_options.hpp>

#include "allexamples.hpp"



int main(int argc, const char** argv)
{
    using namespace boost::program_options;
    options_description options("Options");

    options.add_options()
        ("help,h", "Show this message")
        ("example,e", value<std::string>()->required(), "Example to run");

    variables_map args;
    try {
        store(parse_command_line(argc, argv, options), args);
        notify(args);
        if (args.count("help"))
            throw std::runtime_error("Call help");
    }
    catch (std::exception &e) {
        std::cerr << "Error: " << e.what() << '\n';
        std::cerr << options << '\n';
        std::cout << "Examples:\n";
        for (auto& ex : examples) {
            std::cout << ex->name << "\n";
        }
        return 1;
    }

    // Try by index
    int example_index = -1;
    try {
        example_index = boost::lexical_cast<int>(args.at("example").as<std::string>());
        if (example_index < 1 || example_index > examples.size()) {
            std::cerr << "Error out of range: " << example_index << " not in [1, " << examples.size() << "]" << std::endl;
            return 1;
        }
    }
    catch (boost::bad_lexical_cast const& ex) {
        std::cerr << "Error cast argument 'example' to int" << std::endl;
        std::cerr << ex.what() << std::endl;
        return 1;
    }

    auto& example = examples[example_index - 1];
    return example->run();
}
