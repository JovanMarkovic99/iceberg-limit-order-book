
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

TEST_F(OrderBookFixture, SimpleExample1) {
    book.processOrder(std::unique_ptr<Order>(new IcebergOrder(OrderType::BUY, 1, 100, 50, 10)));
    OrderBookPrinter::print(book);
    book.processOrder(std::unique_ptr<Order>(new IcebergOrder(OrderType::SELL, 2, 10, 100, 10)));
    OrderBookPrinter::print(book);
    book.processOrder(std::unique_ptr<Order>(new IcebergOrder(OrderType::BUY, 3, 20, 100, 10)));
    OrderBookPrinter::print(book);

    EXPECT_TRUE(buffer.str() == 
    "+-----------------------------------------------------------------+\n"
    "| BUY                            | SELL                           |\n"
    "| Id       | Volume      | Price | Price | Volume      | Id       |\n"
    "+----------+-------------+-------+-------+-------------+----------+\n"
    "|         1|           10|    100|       |             |          |\n"
    "+-----------------------------------------------------------------+\n"
    "1,2,10,50\n"
    "+-----------------------------------------------------------------+\n"
    "| BUY                            | SELL                           |\n"
    "| Id       | Volume      | Price | Price | Volume      | Id       |\n"
    "+----------+-------------+-------+-------+-------------+----------+\n"
    "|          |             |       |     10|           10|         2|\n"
    "+-----------------------------------------------------------------+\n"
    "3,2,10,50\n"
    "+-----------------------------------------------------------------+\n"
    "| BUY                            | SELL                           |\n"
    "| Id       | Volume      | Price | Price | Volume      | Id       |\n"
    "+----------+-------------+-------+-------+-------------+----------+\n"
    "|         3|           10|     20|       |             |          |\n"
    "+-----------------------------------------------------------------+\n"
    );
}

TEST_F(OrderBookFixture, ComplexExampl1) {
    book.processOrder(std::unique_ptr<Order>(new IcebergOrder(OrderType::SELL, 1, 100, 50, 10)));
    OrderBookPrinter::print(book);
    book.processOrder(std::unique_ptr<Order>(new Order(OrderType::SELL, 2, 100, 10)));
    OrderBookPrinter::print(book);
    book.processOrder(std::unique_ptr<Order>(new Order(OrderType::SELL, 3, 101, 5)));
    OrderBookPrinter::print(book);
    book.processOrder(std::unique_ptr<Order>(new Order(OrderType::SELL, 4, 99, 3)));
    OrderBookPrinter::print(book);

    book.processOrder(std::unique_ptr<Order>(new Order(OrderType::BUY, 5, 105, 2)));
    OrderBookPrinter::print(book);
    book.processOrder(std::unique_ptr<Order>(new Order(OrderType::BUY, 6, 100, 1 + 10 + 10 + 10)));
    OrderBookPrinter::print(book);
    book.processOrder(std::unique_ptr<Order>(new Order(OrderType::BUY, 7, 100, 31)));
    OrderBookPrinter::print(book);

    EXPECT_TRUE(buffer.str() == 
    "+-----------------------------------------------------------------+\n"
    "| BUY                            | SELL                           |\n"
    "| Id       | Volume      | Price | Price | Volume      | Id       |\n"
    "+----------+-------------+-------+-------+-------------+----------+\n"
    "|          |             |       |    100|           10|         1|\n"
    "+-----------------------------------------------------------------+\n"
    "+-----------------------------------------------------------------+\n"
    "| BUY                            | SELL                           |\n"
    "| Id       | Volume      | Price | Price | Volume      | Id       |\n"
    "+----------+-------------+-------+-------+-------------+----------+\n"
    "|          |             |       |    100|           10|         1|\n"
    "|          |             |       |    100|           10|         2|\n"
    "+-----------------------------------------------------------------+\n"
    "+-----------------------------------------------------------------+\n"
    "| BUY                            | SELL                           |\n"
    "| Id       | Volume      | Price | Price | Volume      | Id       |\n"
    "+----------+-------------+-------+-------+-------------+----------+\n"
    "|          |             |       |    100|           10|         1|\n"
    "|          |             |       |    100|           10|         2|\n"
    "|          |             |       |    101|            5|         3|\n"
    "+-----------------------------------------------------------------+\n"
    "+-----------------------------------------------------------------+\n"
    "| BUY                            | SELL                           |\n"
    "| Id       | Volume      | Price | Price | Volume      | Id       |\n"
    "+----------+-------------+-------+-------+-------------+----------+\n"
    "|          |             |       |     99|            3|         4|\n"
    "|          |             |       |    100|           10|         1|\n"
    "|          |             |       |    100|           10|         2|\n"
    "|          |             |       |    101|            5|         3|\n"
    "+-----------------------------------------------------------------+\n"
    "5,4,99,2\n"
    "+-----------------------------------------------------------------+\n"
    "| BUY                            | SELL                           |\n"
    "| Id       | Volume      | Price | Price | Volume      | Id       |\n"
    "+----------+-------------+-------+-------+-------------+----------+\n"
    "|          |             |       |     99|            1|         4|\n"
    "|          |             |       |    100|           10|         1|\n"
    "|          |             |       |    100|           10|         2|\n"
    "|          |             |       |    101|            5|         3|\n"
    "+-----------------------------------------------------------------+\n"
    "6,4,100,1\n"
    "6,1,100,10\n"
    "6,2,100,10\n"
    "6,1,100,10\n"
    "+-----------------------------------------------------------------+\n"
    "| BUY                            | SELL                           |\n"
    "| Id       | Volume      | Price | Price | Volume      | Id       |\n"
    "+----------+-------------+-------+-------+-------------+----------+\n"
    "|          |             |       |    100|           10|         1|\n"
    "|          |             |       |    101|            5|         3|\n"
    "+-----------------------------------------------------------------+\n"
    "7,1,100,30\n"
    "+-----------------------------------------------------------------+\n"
    "| BUY                            | SELL                           |\n"
    "| Id       | Volume      | Price | Price | Volume      | Id       |\n"
    "+----------+-------------+-------+-------+-------------+----------+\n"
    "|         7|            1|    100|    101|            5|         3|\n"
    "+-----------------------------------------------------------------+\n"
    );
}