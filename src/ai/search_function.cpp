#include "search_function.h"

#include <algorithm>
#include <limits>
#include <random>
#include <map>
#include <vector>
#include <boost/range/algorithm/find.hpp>
#include <boost/range/algorithm/max_element.hpp>
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

    int minimax::operator()(state st) const
    {
        std::multimap<double, movement> move_evals;
        auto const moves = enumerate_movable_pieces(st, st.current_turn());
        auto const move = execute(st, depth_, move_evals);
        auto const target = move.second.get();
        auto const it = boost::find(moves, target);
        if (it == moves.end())
        {
            throw std::runtime_error{"movement is not found"};
        }
        return std::distance(std::begin(moves), it);
    }

    minimax::result_type minimax::execute(state const& st, std::size_t depth, std::multimap<double, movement>& move_evals) const
    {
        auto const moves = enumerate_movable_pieces(st, st.current_turn());
        if (depth == 0 || moves.empty())
        {
            return {static_cast<double>(eval_func_(st, !st.current_turn())), boost::none};
        }

        ASHOGI_LOG_TRIVIAL(debug) << "depth : " << depth_ - depth + 1;
        double max_score = static_cast<int>(std::numeric_limits<int>::min());
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

            auto score = execute(next, depth - 1, move_evals);
            score.first *= -1.0;

            ASHOGI_LOG_TRIVIAL(debug) << "move : " << it.str() << ", eval : " << score.first;
            
            if (max_score < score.first)
            {
                max_score = score.first;
            }
            move_evals.emplace(max_score, it);
        }
        return result_type{max_score, move_evals.lower_bound(max_score)->second};
        //// 評価値が最大の最初の要素の位置を得る
        //using val_t = decltype(move_evals)::value_type;
        //auto pos = boost::max_element(move_evals, [](val_t const& l, val_t const& r)
        //{
        //    return l.first < r.first;
        //});

        //// 評価値が最大の要素数を得、[0, count)の範囲の乱数分布器を作成
        //std::uniform_int_distribution<> dist{0, static_cast<int>(move_evals.count(pos->first)) - 1};

        //// 最大評価値の指し手のうち、実際に指す手を一様乱数で決める
        //result_type result{pos->first, std::next(pos, dist(engine))->second};
        //return result;
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

    int alphabeta::operator()(state st) const
    {
        auto const moves = enumerate_movable_pieces(st, st.current_turn());
        // 最も高い評価値とその指し手を記録する
        std::multimap<double, boost::optional<movement>> move_evals;
        ASHOGI_LOG_TRIVIAL(debug) << "depth : " << 1 << "@ --------";
        for (auto const& it : moves)
        {
            auto const eval = execute(
                st,
                st.current_turn(),
                it,
                depth_ - 1,
                static_cast<double>(std::numeric_limits<int>::min() + 1),
                static_cast<double>(std::numeric_limits<int>::max()));

            move_evals.emplace(eval, it);
        }

        // 評価値が最大の最初の要素の位置を得る
        using val_t = decltype(move_evals)::value_type;
        auto pos = boost::max_element(move_evals, [](val_t const& l, val_t const& r)
        {
            return l.first < r.first;
        });

        // 評価値が最大の要素数を得、[0, count)の範囲の乱数分布器を作成
        std::uniform_int_distribution<> dist{0, static_cast<int>(move_evals.count(pos->first)) - 1};

        // 最大評価値の指し手のうち、実際に指す手を一様乱数で決める
        std::advance(pos, dist(engine));

        ASHOGI_LOG_TRIVIAL(debug) << "select : " << pos->second->str() << ", eval : " << pos->first;
        return std::distance(std::begin(moves), boost::find(moves, pos->second.get()));
    }

    alphabeta::result_type alphabeta::execute(state const& st, turn trn, movement const& move, std::size_t depth, double alpha, double beta) const
    {
        state next;
        if (move.from)
        {
            next = st.update_from_board_copy(move.from.get(), move.to);
        }
        else
        {
            next = st.update_from_cap_pc_copy(move.to, move.pc);
        }
        auto const next_moves = enumerate_movable_pieces(next, next.current_turn());

        if (depth == 0)
        {
            auto const eval = eval_func_(next, trn);
            ASHOGI_LOG_TRIVIAL(debug) << "move : " << move.str() << ", eval : " << eval;
            return static_cast<double>(eval);
        }

        ASHOGI_LOG_TRIVIAL(debug) << "depth : " << depth_ - depth + 1 << "@ " << move.str();
        if (trn == next.current_turn())
        {
            for (auto const& it : next_moves)
            {
                auto const score = std::max(alpha, execute(next, trn, it, depth - 1, alpha, beta));            
                if (beta <= alpha)
                {
                    return beta;
                }
            }
            return alpha;
        }
        else
        {
            for (auto const& it : next_moves)
            {
                auto const score = std::min(alpha, execute(next, trn, it, depth - 1, alpha, beta));
                if (beta <= alpha)
                {
                    return alpha;
                }
            }
            return beta;
        }
    }
}