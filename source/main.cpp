
#include "../include/OrderBook.hpp"
#include "../include/OrderBookPrinter.hpp"

using namespace jvn;

int main() {
    OrderBook book;
    book.processOrder(std::unique_ptr<Order>(new IcebergOrder(OrderType::BUY, 1, 100, 50, 10)));
    OrderBookPrinter::print(book);
    book.processOrder(std::unique_ptr<Order>(new IcebergOrder(OrderType::SELL, 2, 10, 100, 10)));
    OrderBookPrinter::print(book);
    book.processOrder(std::unique_ptr<Order>(new IcebergOrder(OrderType::BUY, 3, 20, 100, 10)));
    OrderBookPrinter::print(book);

    /*
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
    */

    return 0;
}
