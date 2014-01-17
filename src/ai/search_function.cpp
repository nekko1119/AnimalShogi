#include "search_function.h"

#include <algorithm>
#include <limits>
#include <random>
#include <map>
#include <vector>
#include <boost/range/algorithm/find.hpp>
#include "../utility/logging.h"

namespace animal_shogi
{
    namespace
    {
        auto engine = std::mt19937{std::random_device{}()};
    }

    minimax::minimax(eval_func_type eval_func, std::size_t depth)
    : eval_func_{std::move(eval_func)},
    depth_{depth}
    {
        if (depth < 1)
        {
            ASHOGI_LOG_TRIVIAL(error) << "再帰深度は１以上でなければならない";
            throw std::invalid_argument{"depth should be at least 1"};
        }
    }

    int minimax::operator()(state st)
    {
        state_ = st;
        auto const moves = enumerate_movable_pieces(st, st.current_turn());
        auto const move = *(execute(st, depth_).second);
        auto const it = boost::find(moves, move);
        return std::distance(std::begin(moves), it);
    }

    minimax::result_type minimax::execute(state const& st, std::size_t depth) const
    {
        auto const moves = enumerate_movable_pieces(st, st.current_turn());
        if (depth == 0 || moves.empty())
        {
            return {eval_func_(st), boost::none};
        }

        std::multimap<int, movement> move_evals;
        for (auto const& it : moves)
        {
            state next;
            if (it.from)
            {
                next = st.update_from_board_copy(*(it.from), it.to);
            }
            else
            {
                next = st.update_from_cap_pc_copy(it.to, it.pc);
            }

            ASHOGI_LOG_TRIVIAL(debug) << "depth : " << depth_ - depth + 1 << ", move : " << it.str();
            auto score = execute(next, depth - 1);
            move_evals.emplace(-score.first, it);
        }
        // 評価値が最大の最初の要素の位置を得る
        using val_t = decltype(move_evals)::value_type;
        auto pos = std::max_element(std::begin(move_evals), std::end(move_evals), [](val_t const& l, val_t const& r)
        {
            return l.first < r.first;
        });

        // 評価値が最大の要素数を得、[0, count)の範囲の乱数分布器を作成
        std::uniform_int_distribution<> dist{0, static_cast<int>(move_evals.count((*pos).first)) - 1};

        // 最大評価値の指し手のうち、実際に指す手を一様乱数で決める
        result_type result{(*pos).first, (*std::next(pos, dist(engine))).second};
        return result;
    }

    alphabeta::alphabeta(eval_func_type eval_func, std::size_t depth)
    : eval_func_{std::move(eval_func)},
    depth_{depth}
    {
        if (depth < 1)
        {
            ASHOGI_LOG_TRIVIAL(error) << "再帰深度は１以上でなければならない";
            throw std::invalid_argument{"depth should be at least 1"};
        }
    }

    int alphabeta::operator()(state st)
    {
        state_ = st;
        auto const moves = enumerate_movable_pieces(st, st.current_turn());
        auto const move = *(execute(st, depth_, std::numeric_limits<int>::min() + 1, std::numeric_limits<int>::max()).second);
        auto const it = boost::find(moves, move);
        return std::distance(std::begin(moves), it);
    }

    alphabeta::result_type alphabeta::execute(state const& st, std::size_t depth, int alpha, int beta) const
    {
        auto const moves = enumerate_movable_pieces(st, st.current_turn());
        if (depth == 0 || moves.empty())
        {
            return {eval_func_(st), boost::none};
        }

        std::multimap<int, movement> move_evals;
        for (auto const& it : moves)
        {
            state next;
            if (it.from)
            {
                next = st.update_from_board_copy(*(it.from), it.to);
            }
            else
            {
                next = st.update_from_cap_pc_copy(it.to, it.pc);
            }

            ASHOGI_LOG_TRIVIAL(debug) << "depth : " << depth_ - depth + 1 << ", move : " << it.str();
            auto const score = execute(next, depth - 1, -beta, -alpha);
            if (beta <= alpha)
            {
                return {alpha, it};
            }

            move_evals.emplace(-score.first, it);
            alpha = std::max(alpha, -score.first);
        }
        // 評価値が最大の最初の要素の位置を得る
        using val_t = decltype(move_evals)::value_type;
        auto pos = std::max_element(std::begin(move_evals), std::end(move_evals), [](val_t const& l, val_t const& r)
        {
            return l.first < r.first;
        });


        if (pos->first < -90)
        {
            ASHOGI_LOG_TRIVIAL(info) << "負け確定 : " << pos->first;
        }

        // 評価値が最大の要素数を得、[0, count)の範囲の乱数分布器を作成
        std::uniform_int_distribution<> dist{0, static_cast<int>(move_evals.count(pos->first)) - 1};

        // 最大評価値の指し手のうち、実際に指す手を一様乱数で決める
        result_type result{pos->first, std::next(pos, dist(engine))->second};
        return result;
    }
}