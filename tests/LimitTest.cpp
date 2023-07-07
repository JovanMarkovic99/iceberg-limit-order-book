
#include <gtest/gtest.h>
#include "../include/Limit.hpp"
#include <vector>

using namespace jvn;

constexpr size_t NUM_ORDERS = 10;
std::unique_ptr<Order> createIdxOrder(size_t idx) {
    return std::make_unique<Order>(OrderType(idx % 2), 2 * idx, 0, 3 * idx);
}

TEST(LimitTest, IterationFunctionality) {
    Limit limit(0);
    EXPECT_TRUE(limit.isEmpty());

    Order* orders[NUM_ORDERS];
    for (size_t idx = 0; idx < NUM_ORDERS; ++idx) {
        orders[idx] = createIdxOrder(idx).release();
        limit.pushBack(std::unique_ptr<Order>(orders[idx]));
    }

    size_t idx = 0;
    for (auto& ptr: limit) {
        EXPECT_EQ(orders[idx], ptr.get());
        ++idx;
    }
}


TEST(LimitTest, FullFunctionality) {
    Limit limit(0);
    for (size_t idx = 0; idx < NUM_ORDERS; ++idx)
        limit.pushBack(createIdxOrder(idx));

    for (size_t idx = 0; idx < NUM_ORDERS; ++idx)
        limit.pushBack(limit.popFront());

    size_t idx = 0;
    for (auto iter = limit.begin(); iter != limit.end(); ++iter) {
        auto tmp_order = createIdxOrder(idx++);
        EXPECT_EQ(tmp_order->order_type, (*iter)->order_type);
        EXPECT_EQ(tmp_order->id, (*iter)->id);
        EXPECT_EQ(tmp_order->limit, (*iter)->limit);
        EXPECT_EQ(tmp_order->quantity, (*iter)->quantity);
    }

    for (size_t idx = 0; idx < NUM_ORDERS; ++idx) {
        EXPECT_FALSE(limit.isEmpty());
        if (idx % 2)
            EXPECT_NO_THROW(limit.popFront());
        else
            EXPECT_NO_THROW(limit.popBack());
    }
    
    EXPECT_TRUE(limit.isEmpty());
}