#include "command_line_parser.h"

#include <iostream>

namespace animal_shogi
{
    command_line_parser::command_line_parser(int argc, char** argv, bool is_learn)
    {
        program_options::options_description option{"Usage "};

        if (!is_learn)
        {
            option.add_options()
                ("help,h", "Lists commands")
                ("black,b", program_options::value<std::string>(), "First player : \"human\" or \"ai\"")
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
        else
        {
            option.add_options()
                ("help,h", "Lists commands")
                ("loop,l", program_options::value<int>()->default_value(1), "Loop count")
                ("generation,g", program_options::value<int>()->default_value(1), "Generation count")
                ("depth,d", program_options::value<std::size_t>()->default_value(6), "First AI's depth limit of search (standard 1 ~ 9) ")
                ("depth2,D", program_options::value<std::size_t>()->default_value(6), "Second AI's depth limit of search (standard 1 ~ 9) ");

            program_options::store(program_options::parse_command_line(argc, argv, option), variable_map_);
            program_options::notify(variable_map_);

            if (variable_map_.count("help"))
            {
                std::cout << option << std::endl;
            }
        }
    }
}