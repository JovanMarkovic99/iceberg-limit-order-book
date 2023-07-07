
#include <gtest/gtest.h>
#include "../include/OrderBook.hpp"
#include "../include/OrderBookPrinter.hpp"
#include <iostream>
#include <sstream>
#include <streambuf>

using namespace jvn;

class OrderBookFixture: public ::testing::Test {
protected:
    void SetUp() override {
        org_cout = std::cout.rdbuf(buffer.rdbuf());
    }

    void TearDown() override {
        std::cout.rdbuf(org_cout);
    }

    std::stringstream buffer;
    std::streambuf *org_cout;
    OrderBook book;
};

TEST_F(OrderBookFixture, BasicFunctionality) {
    book.processOrder(std::unique_ptr<Order>(new Order(OrderType::BUY, 100322, 5103, 7500)));
    EXPECT_TRUE(buffer.str() == "");
    OrderBookPrinter::print(book);
    EXPECT_TRUE(buffer.str() == 
    "+-----------------------------------------------------------------+\n"
    "| BUY                            | SELL                           |\n"
    "| Id       | Volume      | Price | Price | Volume      | Id       |\n"
    "+----------+-------------+-------+-------+-------------+----------+\n"
    "|    100322|        7,500|  5,103|       |             |          |\n"
    "+-----------------------------------------------------------------+\n");
    buffer.str("");

    book.processOrder(std::unique_ptr<Order>(new IcebergOrder(OrderType::SELL, 100345, 5103, 100000, 10000)));
    EXPECT_TRUE(buffer.str() == "100322,100345,5103,7500\n");
    buffer.str("");

    OrderBookPrinter::print(book);
    EXPECT_TRUE(buffer.str() == 
    "+-----------------------------------------------------------------+\n"
    "| BUY                            | SELL                           |\n"
    "| Id       | Volume      | Price | Price | Volume      | Id       |\n"
    "+----------+-------------+-------+-------+-------------+----------+\n"
    "|          |             |       |  5,103|       10,000|    100345|\n"
    "+-----------------------------------------------------------------+\n");
    buffer.str("");
}