#ifndef ANIMAL_SHOGI_IO_CSV_HPP
#define ANIMAL_SHOGI_IO_CSV_HPP

#include <sstream>
#include <vector>
#include "exchange.hpp"

namespace animal_shogi
{
    template <typename Stream, typename Row>
    void write_csv(Stream& out, Row const& values)
    {
        bool is_first = true;
        for (auto const& it : values)
        {
            // 最初だけカンマを出力しない
            if (!animal_shogi::exchange(is_first, false))
            {
                out << ",";
            }
            out << it;
        }
        out << "\n";
    }

    template <typename Stream>
    std::vector<std::vector<std::string>> read_csv(Stream& in)
    {
        std::vector<std::vector<std::string>> column;
        std::string row_str;
        while (std::getline(in, row_str))
        {
            std::vector<std::string> row;
            std::string token;
            std::istringstream iss{row_str};
            while (std::getline(iss, token, ','))
            {
                row.push_back(token);
            }
            column.push_back(row);
        }
        return column;
    }
}

#endif