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
    r.emplace_back(point{2, 3}, {2, 2}, {turn::black, ptype::chick});
    r.emplace_back(point{1, 1}, {2, 2}, {turn::white, ptype::elephant});
    r.emplace_back(boost::none, {2, 3}, {turn::black, ptype::chick});
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
    r.emplace_back(point{2, 3}, {2, 2}, {turn::black, ptype::chick});
    r.emplace_back(point{1, 1}, {2, 2}, {turn::white, ptype::elephant});
    r.emplace_back(boost::none, {2, 3}, piece{turn::black, ptype::chick});
    std::vector<movement> expected =
    {
        {point{2, 3}, {2, 2}, {turn::black, ptype::chick}},
        {point{1, 1}, {2, 2}, {turn::white, ptype::elephant}},
        {boost::none, {2, 3}, {turn::black, ptype::chick}}
    };
    EXPECT_EQ(3, r.size());
    bool cond = std::equal(std::begin(r), std::end(r), std::begin(expected), compare);
    EXPECT_TRUE(cond);
}

TEST(record_test, pop)
{
    record r;
    r.emplace_back(point{2, 3}, {2, 2}, {turn::black, ptype::chick});
    r.emplace_back(point{1, 1}, {2, 2}, {turn::white, ptype::elephant});
    r.emplace_back(boost::none, {2, 3}, {turn::black, ptype::chick});
    std::vector<movement> expected =
    {
        {point{2, 3}, {2, 2}, {turn::black, ptype::chick}},
        {point{1, 1}, {2, 2}, {turn::white, ptype::elephant}},
        {boost::none, {2, 3}, {turn::black, ptype::chick}}
    };
    EXPECT_EQ(3, r.size());

    r.pop();
    EXPECT_EQ(2, r.size());
    expected.pop_back();
    bool cond = std::equal(std::begin(r), std::end(r), std::begin(expected), compare);
    EXPECT_TRUE(cond);

    r.emplace_back(point{3, 1}, {3, 2}, {turn::white, ptype::giraffe});
    EXPECT_EQ(3, r.size());
    expected.emplace_back(point{3, 1}, point{3, 2}, piece{turn::white, ptype::giraffe});
    cond = std::equal(std::begin(r), std::end(r), std::begin(expected), compare);
    EXPECT_TRUE(cond);
}

TEST(record_test, size)
{
    record r;
    EXPECT_EQ(0, r.size());
    r.emplace_back(point{0, 0}, {0, 0}, {turn::black, ptype::chick});
    EXPECT_EQ(1, r.size());
    r.pop();
    EXPECT_EQ(0, r.size());
}

TEST(record_test, top)
{
    record r;
    //auto const& m = r.top(); // assert failure
    movement m{point{0, 0}, {0, 0}, {turn::white, ptype::lion}};
    movement m2{point{3, 3}, {3, 4}, {turn::black, ptype::elephant}};
    r.emplace_back(m.from, m.to, m.pc);
    EXPECT_TRUE(compare(r.top(), m));
    r.emplace_back(m2.from, m2.to, m2.pc);
    EXPECT_TRUE(compare(r.top(), m2));
}

TEST(record_test, iterator)
{
    record r;
    movement m[] =
    {
        {point{2, 3}, {2, 2}, {turn::black, ptype::chick}},
        {point{1, 1}, {2, 2}, {turn::white, ptype::elephant}},
        {boost::none, {2, 3}, {turn::black, ptype::chick}}
    };
    r.emplace_back(point{2, 3}, {2, 2}, {turn::black, ptype::chick});
    r.emplace_back(point{1, 1}, {2, 2}, {turn::white, ptype::elephant});
    r.emplace_back(boost::none, {2, 3}, {turn::black, ptype::chick});
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
        {point{2, 3}, {2, 2}, {turn::black, ptype::chick}},
        {point{1, 1}, {2, 2}, {turn::white, ptype::elephant}},
        {boost::none, {2, 3}, {turn::black, ptype::chick}}
    };
    r.emplace_back(point{2, 3}, {2, 2}, {turn::black, ptype::chick});
    r.emplace_back(point{1, 1}, {2, 2}, {turn::white, ptype::elephant});
    r.emplace_back(boost::none, {2, 3}, {turn::black, ptype::chick});
    for (record::size_type i = 0; i < r.size(); ++i)
    {
        EXPECT_TRUE(compare(m[i], r[i]));
    }
}