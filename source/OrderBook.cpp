
#include "../include/OrderBook.hpp"
#include <utility>
#include <sstream>
#include <unordered_map>

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
            while (order->quantity > 0 && m_lowest_sell != m_sell_map.end() && order->limit >= m_lowest_sell->first)
                matches.emplace_back(match(order.get(), m_sell_map, m_lowest_sell));
            
            mergeIcebergMatches<OrderType::BUY>(matches);

            if (order->quantity > 0)
                add(std::move(order), m_buy_map, m_highest_buy);
        } else {
            while (order->quantity > 0 && m_highest_buy != m_buy_map.end() && order->limit <= m_highest_buy->first)
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
        limit_type matching_order_limit = matching_order->limit;
        quantity_type matching_order_volume = matching_order->getVolume();

        // Exhausted match
        if (matching_order_volume <= order->quantity) {
            order->quantity -= matching_order_volume;
            matching_order->matchVolume(matching_order_volume);

            // Not-exhausted Iceberg-order
            if (matching_order->quantity)
                match_iter->second.pushBack(std::move(matching_order));

            // Exhausted Limit
            if (match_iter->second.isEmpty())
                map.erase(std::exchange(match_iter, std::next(match_iter)));

            if constexpr(std::is_same_v<Map, OrderBook::buy_map_type>)
                return Match{matching_order_id, order->id, order->limit, matching_order_volume};
            else
                return Match{order->id, matching_order_id, matching_order_limit, matching_order_volume};

        }

        quantity_type old_order_quantity = std::exchange(order->quantity, 0);
        matching_order->matchVolume(old_order_quantity);

        match_iter->second.pushFront(std::move(matching_order));

        if constexpr(std::is_same_v<Map, OrderBook::buy_map_type>)
            return Match{matching_order_id, order->id, order->limit, old_order_quantity};
        else
            return Match{order->id, matching_order_id, matching_order_limit, old_order_quantity};
    };

    template <OrderType Ty>
    void OrderBook::mergeIcebergMatches(std::vector<OrderBook::Match>& matches) const {
        if (!matches.size())
            return;

        std::unordered_map<int, size_t> seen_match_indices;
        auto id_match = [](const Match& m) {
            if constexpr (Ty == OrderType::BUY)
                return m.sell_id;
            else
                return m.buy_id;
        };

        size_t new_end = 0;
        for (size_t idx = 0; idx < matches.size(); ++idx) {
            auto [iter, not_found] = seen_match_indices.emplace(id_match(matches[idx]), new_end);
            if (not_found)
                matches[new_end++] = matches[idx];
            else
                matches[iter->second].quantity += matches[idx].quantity;
        }

        matches.resize(new_end);
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