
#include "../include/OrderBookPrinter.hpp"
#include <sstream>

using namespace jvn;

std::vector<std::string> delimString(const std::string& str, char delimiter) {
    std::vector<std::string> tokens;
    std::istringstream iss(str);
    std::string token;
    while (std::getline(iss, token, ','))
        tokens.push_back(token);

    return tokens;
}

std::unique_ptr<Order> generateOrder() {
    std::string input;
    std::getline(std::cin, input);
    if (!input.size() || (input[0] != 'B' && input[0] != 'S'))
        return nullptr;

    auto tokens = delimString(input, ',');
    // Add input checking if necessary

    OrderType order_type = (tokens[0] == "B") ? OrderType::BUY : OrderType::SELL;
    int id = std::stoi(tokens[1]);
    Order::limit_type limit = static_cast<Order::limit_type>(std::stoi(tokens[2]));
    Order::quantity_type quantity = std::stoi(tokens[3]);

    if  (tokens.size() == 4)
        return std::make_unique<Order>(order_type, id, limit, quantity);

    Order::quantity_type peak_size = std::stoi(tokens[4]);
    return std::make_unique<IcebergOrder>(order_type, id, limit, quantity, peak_size);
}

int main() {
    OrderBook book;
    for (std::unique_ptr<Order> order = nullptr;;order = generateOrder())
        if (order) {
            book.processOrder(std::move(order));
            OrderBookPrinter::print(book);
        }

    return 0;
}
