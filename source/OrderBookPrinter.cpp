
#include "../include/OrderBookPrinter.hpp"
#include <iostream>
#include <iomanip>

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
        for (size_t section_idx = 0; section_idx < 2 * table_sections.size(); ++section_idx) {
            SectionInfo section_info = table_sections[section_idx < table_sections.size() ? section_idx : 2 * table_sections.size() - section_idx - 1];
            printTextSection(section_info.text, section_info.width, SEPARATOR_CHAR, VERTICAL_EDGE_CHAR);
        }
        std::cout << VERTICAL_EDGE_CHAR << '\n';
    }

    // TODO: Further refactor
    void OrderBookPrinter::printOrders(const OrderBook& order_book) {
        for (size_t section_idx = 0; section_idx < 2 * table_sections.size(); ++section_idx) {
            SectionInfo section_info = table_sections[section_idx < table_sections.size() ? section_idx : 2 * table_sections.size() - section_idx - 1];
            printTextSection("", section_info.width, HORIZONTAL_EDGE_CHAR, CORNER_CHAR);
        }
        std::cout << CORNER_CHAR << '\n';

        std::cout << std::right;

        auto buy_map_iter = order_book.m_buy_map.begin();
        auto sell_map_iter = order_book.m_sell_map.begin();

        // All dual buy-sell lambdas in this function can be merged with C++20 templated lambdas
        auto done_buy = [&]() { return buy_map_iter == order_book.m_buy_map.end(); };
        auto done_sell = [&]() { return sell_map_iter == order_book.m_sell_map.end(); };

        Limit::const_iterator buy_limit_iter;
        if (!done_buy())
            buy_limit_iter = buy_map_iter->second.begin();
        Limit::const_iterator sell_limit_iter;
        if (!done_sell())
            sell_limit_iter = sell_map_iter->second.begin();

        auto advance_buy_iter_pair = [&]() {
            ++buy_limit_iter;
            if (buy_limit_iter == buy_map_iter->second.end()) {
                ++buy_map_iter;
                if (buy_map_iter != order_book.m_buy_map.end())
                    buy_limit_iter = buy_map_iter->second.begin();
            }
        };

        auto advance_sell_iter_pair = [&]() {
            ++sell_limit_iter;
            if (sell_limit_iter == sell_map_iter->second.end()) {
                ++sell_map_iter;
                if (sell_map_iter != order_book.m_sell_map.end())
                    sell_limit_iter = sell_map_iter->second.begin();
            }
        };

        while (!done_buy() || !done_sell()) {
            if (!done_buy()) {
                printOrder<false>(buy_limit_iter->get());
                advance_buy_iter_pair();
            } else 
                printEmptyOrder<false>();

            if (!done_sell()) {
                printOrder<true>(sell_limit_iter->get());
                advance_sell_iter_pair();
            } else
                printEmptyOrder<true>();

            std::cout << VERTICAL_EDGE_CHAR << '\n';
        }

        // +-----------------------------------------------------------------+
        printTextSection("", TOTAL_COLUMN_WIDTH, HORIZONTAL_EDGE_CHAR, CORNER_CHAR, CORNER_CHAR, true);
    }

    template <bool Inverse>
    void OrderBookPrinter::printOrder(const Order* order) {
        if constexpr (Inverse)
            for (auto section_info_iter = table_sections.rbegin(); section_info_iter != table_sections.rend(); ++section_info_iter)
                printTextSection(intToText(section_info_iter->getter(order), section_info_iter->format_number),
                                section_info_iter->width,
                                SEPARATOR_CHAR,
                                VERTICAL_EDGE_CHAR);
        else
            for (auto section_info_iter = table_sections.begin(); section_info_iter != table_sections.end(); ++section_info_iter)
                printTextSection(intToText(section_info_iter->getter(order), section_info_iter->format_number),
                                section_info_iter->width,
                                SEPARATOR_CHAR,
                                VERTICAL_EDGE_CHAR);
    }

    template <bool Inverse>
    void OrderBookPrinter::printEmptyOrder() {
        if constexpr (Inverse)
            for (auto section_info_iter = table_sections.rbegin(); section_info_iter != table_sections.rend(); ++section_info_iter)
                printTextSection("", section_info_iter->width, SEPARATOR_CHAR, VERTICAL_EDGE_CHAR);
        else
            for (auto section_info_iter = table_sections.begin(); section_info_iter != table_sections.end(); ++section_info_iter)
                printTextSection("", section_info_iter->width, SEPARATOR_CHAR, VERTICAL_EDGE_CHAR);
    }

    std::string OrderBookPrinter::intToText(int num, bool format) {
        std::stringstream ss;
        if (format)
            ss << std::put_money(num);
        else
            ss << num;
        return ss.str();
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