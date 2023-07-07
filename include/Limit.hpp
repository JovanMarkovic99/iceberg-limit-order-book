#pragma once

#include "Order.hpp"
#include <deque>
#include <memory>

namespace jvn 
{
    class Limit {
    public:
        using limit_type = Order::limit_type;
        using list_type = std::deque<std::unique_ptr<Order>>;
        using iterator = list_type::iterator;

        limit_type limit_price;

        Limit(limit_type limit_price);

        std::unique_ptr<Order> popFront() noexcept;
        std::unique_ptr<Order> popBack() noexcept;

        void pushFront(std::unique_ptr<Order> order) noexcept;
        void pushBack(std::unique_ptr<Order> order) noexcept;

        [[nodiscard]] bool isEmpty() const noexcept;
    
        iterator begin() noexcept;
        iterator end() noexcept;
    private:
        list_type m_orders;
    };
}