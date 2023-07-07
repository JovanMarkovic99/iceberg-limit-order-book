#pragma once

#include "OrderBook.hpp"
#include <string_view>

namespace jvn 
{
    class OrderBookPrinter {
    public:
        static void print(const OrderBook& order_book);
    private:
        static constexpr const int TOTAL_COLUMN_WIDTH = 65;
        static constexpr const int HALF_COLUMN_WIDTH = 32;
        static constexpr const int ID_COLUMN_WIDTH = 10;
        static constexpr const int VOLUME_COLUMN_WIDTH = 13;
        static constexpr const int PRICE_COLUMN_WIDTH = 7;

        static constexpr const char CORNER_CHAR = '+';
        static constexpr const char HORIZONTAL_EDGE_CHAR = '-';
        static constexpr const char VERTICAL_EDGE_CHAR = '|';
        static constexpr const char SEPARATOR_CHAR = ' ';

        static void printHeader();
        static void printOrders(const OrderBook& order_book);

        static void printTextSection(std::string_view text, int width, char fill_char, char prefix = '\0', char suffix = '\0', bool end_line = false);
    };
}