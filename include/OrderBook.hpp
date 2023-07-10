#pragma once

#include "Limit.hpp"
#include <memory>
#include <map>
#include <vector>
#include <iostream>

namespace jvn 
{
    class OrderBook {
    public:
        OrderBook();

        using limit_type = Order::limit_type;
        using quantity_type = Order::quantity_type;

        void processOrder(std::unique_ptr<Order> order);

        friend class OrderBookPrinter;
    private:
        using sell_map_type = std::map<limit_type, Limit>;
        using buy_map_type = std::map<limit_type, Limit, std::greater<limit_type>>;
        sell_map_type m_sell_map;
        buy_map_type m_buy_map;
        sell_map_type::iterator m_lowest_sell;
        buy_map_type::iterator m_highest_buy;

        struct Match {
            int buy_id;
            int sell_id;
            limit_type limit;
            quantity_type quantity;
        };
        inline friend std::ostream& operator<<(std::ostream& os, const Match& match);

        template <class Map, class Iter>
        [[nodiscard]] Match match(Order* order, Map& map, Iter& iter);
        template <OrderType Type>
        void mergeIcebergMatches(std::vector<Match>& matches) const;

        template <class Map, class Iter>
        void add(std::unique_ptr<Order> order, Map&, Iter& iter);

        void sendMessages(const std::vector<Match>& matches) const;
    };
}