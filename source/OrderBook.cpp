
#include "../include/OrderBook.hpp"
#include <vector>
#include <utility>
#include <iostream>
#include <sstream>

namespace jvn
{
    OrderBook::OrderBook()
        :m_sell_map(), 
        m_buy_map(), 
        m_lowest_sell(m_sell_map.end()),
        m_highest_buy(m_buy_map.end())
        {}

    void OrderBook::processOrder(std::unique_ptr<Order> order) {
        if (!order) {
            std::cerr << "Invalid processOrder(order) call! Invalid new Order.\n";
            return;
        }

        std::vector<Match> matches;
        if (order->order_type == OrderType::BUY) {
            while (order->quantity > 0 && m_lowest_sell != m_sell_map.end() && m_lowest_sell->first >= order->limit) 
                matches.emplace_back(match(order.get(), m_sell_map, m_lowest_sell));
            
            mergeIcebergMatches<OrderType::BUY>(matches);

            if (order->quantity > 0)
                add(std::move(order), m_buy_map, m_highest_buy);
        } else {
            while (order->quantity > 0 && m_highest_buy != m_buy_map.end() && m_highest_buy->first <= order->limit)
                matches.emplace_back(match(order.get(), m_buy_map, m_highest_buy));

            mergeIcebergMatches<OrderType::SELL>(matches);

            if (order->quantity > 0)
                add(std::move(order), m_sell_map, m_lowest_sell);
        }

        sendMessages(matches);
    }

    template <class Map, class Iter>
    OrderBook::Match OrderBook::match(Order* order, Map& map, Iter& match_iter) {
        std::unique_ptr<Order> matching_order = match_iter->second.popFront();
        int matching_order_id = matching_order->id;
        quantity_type matching_order_volume = matching_order->getVolume();

        // Exhausted match
        if (map.key_comp()(matching_order_volume, order->quantity) || matching_order_volume == order->quantity) {
            order->quantity -= matching_order_volume;
            matching_order->quantity -= matching_order_volume;

            // Not-exhausted Iceberg-order
            if (matching_order->quantity)
                match_iter->second.pushBack(std::move(matching_order));

            // Exhausted Limit
            if (match_iter->second.isEmpty())
                map.erase(std::exchange(match_iter, std::next(match_iter)));

            if constexpr(std::is_same_v<Map, OrderBook::buy_map_type>)
                return Match{matching_order_id, order->id, order->limit, matching_order_volume};
            else
                return Match{order->id, matching_order_id, match_iter->first, matching_order_volume};

        }

        matching_order->quantity -= order->quantity;

        match_iter->second.pushFront(std::move(matching_order));

        if constexpr(std::is_same_v<Map, OrderBook::buy_map_type>)
            return Match{matching_order_id, order->id, order->limit, std::exchange(order->quantity, 0)};
        else
            return Match{order->id, matching_order_id, match_iter->first, std::exchange(order->quantity, 0)};
    };

    template <OrderType Ty>
    void OrderBook::mergeIcebergMatches(std::vector<OrderBook::Match>& matches) const {
        if (!matches.size())
            return;

        size_t new_end = 0;
        for (size_t idx = 1; idx < matches.size(); ++idx)
            // Can perhaps be reduced into one if-else. I'm unsure if the compiler will optimize it in that case
            if constexpr (Ty == OrderType::BUY) {
                if (matches[idx].sell_id == matches[new_end].sell_id)
                    matches[new_end].quantity += matches[idx].quantity;
                else
                    matches[++new_end] = matches[idx];
            } else {
                if (matches[idx].buy_id == matches[new_end].buy_id)
                    matches[new_end].quantity += matches[idx].quantity;
                else
                    matches[++new_end] = matches[idx];
            }

        matches.resize(new_end + 1);
    }

    template <class Map, class Iter>
    void OrderBook::add(std::unique_ptr<Order> order, Map& map, Iter& match_iter) {
        auto [map_iter, inserted] = map.emplace(order->limit, order->limit);

        if (match_iter == map.end() || map.key_comp()(order->limit, match_iter->first))
            match_iter = map_iter;

        map_iter->second.pushBack(std::move(order));
    }

    void OrderBook::sendMessages(const std::vector<OrderBook::Match>& matches) const {
        for (Match match: matches)
            std::cout << match << '\n';
    }

    std::ostream& operator<<(std::ostream& os, const OrderBook::Match& match) {
        return os << match.buy_id << ',' << match.sell_id << ',' << match.limit << ',' << match.quantity;
    }
}