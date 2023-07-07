
#include "../include/OrderBook.hpp"
#include "../include/OrderBookPrinter.hpp"

using namespace jvn;

int main(int argc, char **argv) {
    OrderBook book;
    
    book.processOrder(std::make_unique<IcebergOrder>(OrderType::BUY, 1, 1, 100, 10));
    book.processOrder(std::make_unique<Order>(OrderType::SELL, 2, 1, 100));

    OrderBookPrinter::print(book);

    return 0;
}
