
#include <gtest/gtest.h>
#include "../include/Order.hpp"
#include <memory>

using namespace jvn;

TEST(OrderTest, VirtualFunctionallity) {
  std::unique_ptr<Order> iceberg_order(new IcebergOrder(OrderType::BUY, 0, 0, 100, 30));
  EXPECT_EQ(iceberg_order->getVolume(), 30);
  iceberg_order->quantity -= 80;
  EXPECT_EQ(iceberg_order->getVolume(), 20);
}