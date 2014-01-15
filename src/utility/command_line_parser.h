#ifndef ANIMAL_SHOGI_UTILITY_COMMAND_LINE_PARSER_H
#define ANIMAL_SHOGI_UTILITY_COMMAND_LINE_PARSER_H

#include <boost/program_options.hpp>
#include <boost/utility/string_ref.hpp>

namespace animal_shogi
{
    namespace program_options = boost::program_options;

    class command_line_parser
    {
    public:
        command_line_parser(int argc, char** argv);

        template <typename T>
        T get(boost::string_ref const& ref) const
        {
            return variable_map_[ref.data()].as<T>();
        }

    private:
        program_options::variables_map variable_map_;
    };
}

#endif