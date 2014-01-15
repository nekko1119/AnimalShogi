#include "command_line_parser.h"

#include <iostream>

namespace animal_shogi
{
    command_line_parser::command_line_parser(int argc, char** argv)
    {
        program_options::options_description option{"Usage "};

        option.add_options()
            ("help,h", "Lists commands")
            ("black,b", program_options::value<std::string>(), "Firtst player : \"human\" or \"ai\"")
            ("white,w", program_options::value<std::string>(), "Second player : \"human\" or \"ai\"")
            ("loop,l", program_options::value<int>()->default_value(1), "Loop count")
            ("depth,d", program_options::value<std::size_t>()->default_value(6), "AI's depth limit of search (standard 1 ~ 9) ")
            ("depth2,D", program_options::value<std::size_t>()->default_value(6), "Another AI's depth limit of search (standard 1 ~ 9) ");

        program_options::store(program_options::parse_command_line(argc, argv, option), variable_map_);
        program_options::notify(variable_map_);

        if (variable_map_.count("help") || !variable_map_.count("black") || !variable_map_.count("white"))
        {
            std::cout << option << std::endl;
        }
    }
}