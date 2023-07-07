
#include "../include/OrderBookPrinter.hpp"
#include <iostream>
#include <string_view>
#include <iomanip>
#include <array>

namespace jvn 
{
    void OrderBookPrinter::print(const OrderBook& order_book) {
        printHeader();
        printOrders(order_book);
    }

    void OrderBookPrinter::printHeader() {
        std::cout << std::left;

        // +-----------------------------------------------------------------+
        printTextSection("", TOTAL_COLUMN_WIDTH, HORIZONTAL_EDGE_CHAR, CORNER_CHAR, CORNER_CHAR, true);

        // | BUY                            | SELL                           |
        printTextSection(" BUY ", HALF_COLUMN_WIDTH, SEPARATOR_CHAR, VERTICAL_EDGE_CHAR);
        printTextSection(" SELL ", HALF_COLUMN_WIDTH, SEPARATOR_CHAR, VERTICAL_EDGE_CHAR, VERTICAL_EDGE_CHAR, true);
        
        // | Id       | Volume      | Price | Price | Volume      | Id       |
        std::array<std::pair<std::string_view, int>, 3> sections = {{
            {" Id ", ID_COLUMN_WIDTH},
            {" Volume ", VOLUME_COLUMN_WIDTH},
            {" Price ", PRICE_COLUMN_WIDTH}
        }};
        for (size_t section_idx = 0; section_idx < 2 * sections.size(); ++section_idx) {
            auto [section_text, section_width] = sections[section_idx < sections.size() ? section_idx : 2 * sections.size() - section_idx - 1];
            printTextSection(section_text, section_width, SEPARATOR_CHAR, VERTICAL_EDGE_CHAR);
        }
        std::cout << VERTICAL_EDGE_CHAR << '\n';
    }

    void OrderBookPrinter::printOrders(const OrderBook& order_book) {
        std::cout << std::right;

        //
    }

    void OrderBookPrinter::printTextSection(std::string_view text, int width, char fill_char, char prefix, char suffix, bool end_line) {
        if (prefix != '\0')
            std::cout << prefix;

        std::cout << std::setfill(fill_char) << std::setw(width) << text;

        if (suffix != '\0')
            std::cout << suffix;

        if (end_line)
            std::cout << '\n';
    }
}