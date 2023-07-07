#pragma once

#include "OrderBook.hpp"
#include <functional>
#include <string_view>
#include <array>
#include <locale>

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

        using order_member_getter = int(*)(const Order*);

        struct SectionInfo {
            std::string_view text;
            int width;
            order_member_getter getter;
            bool format_number;
        };

        static constexpr const std::array<SectionInfo, 3> table_sections = {{
            {" Id ", ID_COLUMN_WIDTH, [](const Order* order){ return int(order->id); }, false},
            {" Volume ", VOLUME_COLUMN_WIDTH, [](const Order* order){ return int(order->getVolume()); }, true},
            {" Price ", PRICE_COLUMN_WIDTH, [](const Order* order){ return int(order->limit); }, true}
        }};

        struct comma_three_digit_sep: public std::numpunct<char> {
            virtual std::string do_grouping() const { return "\003"; }
            virtual char do_thousands_sep() const { return ','; }
        };

        static void printHeader();
        static void printOrders(const OrderBook& order_book);

        template <bool Inverse>
        static void printOrder(const Order* order);
        template <bool Inverse>
        static void printEmptyOrder();
        static std::string intToText(int num, bool format);

        static void printTextSection(std::string_view text, int width, char fill_char, char prefix = '\0', char suffix = '\0', bool end_line = false);
    };
}