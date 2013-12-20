#include <record.h>
#include <gtest/gtest.h>
#include <algorithm>
using namespace animal_shogi;

namespace
{
    struct compare_t
    {
        template <typename T>
        bool operator()(T const& lhs, T const& rhs) const
        {
            return lhs.from == rhs.from && lhs.to == rhs.to && lhs.pc == rhs.pc;
        };
    } compare;
}

TEST(record_test, example)
{
    record r;
    r.push(point{2, 3}, {2, 2}, {turn::BLACK, ptype::CHICK});
    r.push(point{1, 1}, {2, 2}, {turn::WHITE, ptype::ELEPHANT});
    r.push(boost::none, {2, 3}, {turn::BLACK, ptype::CHICK});
    std::cout << r[0].str() << std::endl;
    std::cout << r[1].str() << std::endl;
    std::cout << r[2].str() << std::endl;
    for (auto const& it : r)
    {
        std::cout << it.str() << std::endl;
    }
    r.pop();
    auto const& m = r.top();
    EXPECT_TRUE(*m.from == point(1, 1));
}

TEST(record_test, construct)
{
    record r;
    EXPECT_THROW(r.pop(), std::runtime_error);
    //r[0].str(); // assert failure.
    r.begin();
    //r.end(); // assert failure
    //auto const& m = r.top(); // assert failure
    EXPECT_EQ(0, r.size());
}

TEST(record_test, push)
{
    record r;
    // recordクラスで棋譜自体の整合性のチェックは行わない
    r.push(point{2, 3}, {2, 2}, {turn::BLACK, ptype::CHICK});
    r.push(point{1, 1}, {2, 2}, {turn::WHITE, ptype::ELEPHANT});
    r.push(boost::none, {2, 3}, piece{turn::BLACK, ptype::CHICK});
    std::vector<movement> expected =
    {
        {point{2, 3}, {2, 2}, {turn::BLACK, ptype::CHICK}},
        {point{1, 1}, {2, 2}, {turn::WHITE, ptype::ELEPHANT}},
        {boost::none, {2, 3}, {turn::BLACK, ptype::CHICK}}
    };
    EXPECT_EQ(3, r.size());
    bool cond = std::equal(std::begin(r), std::end(r), std::begin(expected), compare);
    EXPECT_TRUE(cond);
}

TEST(record_test, pop)
{
    record r;
    r.push(point{2, 3}, {2, 2}, {turn::BLACK, ptype::CHICK});
    r.push(point{1, 1}, {2, 2}, {turn::WHITE, ptype::ELEPHANT});
    r.push(boost::none, {2, 3}, {turn::BLACK, ptype::CHICK});
    std::vector<movement> expected =
    {
        {point{2, 3}, {2, 2}, {turn::BLACK, ptype::CHICK}},
        {point{1, 1}, {2, 2}, {turn::WHITE, ptype::ELEPHANT}},
        {boost::none, {2, 3}, {turn::BLACK, ptype::CHICK}}
    };
    EXPECT_EQ(3, r.size());

    r.pop();
    EXPECT_EQ(2, r.size());
    expected.pop_back();
    bool cond = std::equal(std::begin(r), std::end(r), std::begin(expected), compare);
    EXPECT_TRUE(cond);

    r.push(point{3, 1}, {3, 2}, {turn::WHITE, ptype::GIRAFFE});
    EXPECT_EQ(3, r.size());
    expected.emplace_back(point{3, 1}, point{3, 2}, piece{turn::WHITE, ptype::GIRAFFE});
    cond = std::equal(std::begin(r), std::end(r), std::begin(expected), compare);
    EXPECT_TRUE(cond);
}

TEST(record_test, size)
{
    record r;
    EXPECT_EQ(0, r.size());
    r.push(point{0, 0}, {0, 0}, {turn::BLACK, ptype::CHICK});
    EXPECT_EQ(1, r.size());
    r.pop();
    EXPECT_EQ(0, r.size());
}

TEST(record_test, top)
{
    record r;
    //auto const& m = r.top(); // assert failure
    movement m{point{0, 0}, {0, 0}, {turn::WHITE, ptype::LION}};
    movement m2{point{3, 3}, {3, 4}, {turn::BLACK, ptype::ELEPHANT}};
    r.push(m.from, m.to, m.pc);
    EXPECT_TRUE(compare(r.top(), m));
    r.push(m2.from, m2.to, m2.pc);
    EXPECT_TRUE(compare(r.top(), m2));
}

TEST(record_test, iterator)
{
    record r;
    movement m[] =
    {
        {point{2, 3}, {2, 2}, {turn::BLACK, ptype::CHICK}},
        {point{1, 1}, {2, 2}, {turn::WHITE, ptype::ELEPHANT}},
        {boost::none, {2, 3}, {turn::BLACK, ptype::CHICK}}
    };
    r.push(point{2, 3}, {2, 2}, {turn::BLACK, ptype::CHICK});
    r.push(point{1, 1}, {2, 2}, {turn::WHITE, ptype::ELEPHANT});
    r.push(boost::none, {2, 3}, {turn::BLACK, ptype::CHICK});
    int i = 0;
    for (auto it = r.begin(), last = r.end(); it != last; ++it, ++i)
    {
        EXPECT_TRUE(compare(m[i], *it));
    }
}

TEST(record_test, ramdom_access)
{
    record r;
    movement m[] =
    {
        {point{2, 3}, {2, 2}, {turn::BLACK, ptype::CHICK}},
        {point{1, 1}, {2, 2}, {turn::WHITE, ptype::ELEPHANT}},
        {boost::none, {2, 3}, {turn::BLACK, ptype::CHICK}}
    };
    r.push(point{2, 3}, {2, 2}, {turn::BLACK, ptype::CHICK});
    r.push(point{1, 1}, {2, 2}, {turn::WHITE, ptype::ELEPHANT});
    r.push(boost::none, {2, 3}, {turn::BLACK, ptype::CHICK});
    for (record::size_type i = 0; i < r.size(); ++i)
    {
        EXPECT_TRUE(compare(m[i], r[i]));
    }
}