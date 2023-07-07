
#include "../include/OrderBook.hpp"
#include "../include/OrderBookPrinter.hpp"

using namespace jvn;

int main(int argc, char **argv) {
    OrderBook book;
    book.processOrder(std::unique_ptr<Order>(new Order(OrderType::BUY, 100322, 5103, 7500)));
    book.processOrder(std::unique_ptr<Order>(new IcebergOrder(OrderType::SELL, 100345, 5103, 100000, 10000)));
    OrderBookPrinter::print(book);

    return 0;
}
