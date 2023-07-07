
#include "../include/Limit.hpp"
#include <iostream>
#include <utility>

namespace jvn
{
    Limit::Limit(limit_type limit_price)
        :limit_price(limit_price)
    {}

    std::unique_ptr<Order> Limit::popFront() noexcept {
        if (isEmpty()) {
            std::cerr << "Invalid popFront() call for Limit with price: " << limit_price << "! Limit has no front to pop.\n";
            return nullptr;
        }

        std::unique_ptr<Order> front_order = std::move(m_orders.front());
        m_orders.pop_front();
        return front_order;
    }

    std::unique_ptr<Order> Limit::popBack() noexcept {
        if (isEmpty()) {
            std::cerr << "Invalid popBack() call for Limit with price: " << limit_price << "! Limit has no back to pop.\n";
            return nullptr;
        }

        std::unique_ptr<Order> back_order = std::move(m_orders.back());
        m_orders.pop_back();
        return back_order;
    }

    void Limit::pushFront(std::unique_ptr<Order> order) noexcept {
        if (!order) {
            std::cerr << "Invalid pushFront(order) call for Limit with price: " << limit_price << "! Invalid new Order.\n";
            return;
        }

        m_orders.emplace_front(std::move(order));
    }

    void Limit::pushBack(std::unique_ptr<Order> order) noexcept {
        if (!order) {
            std::cerr << "Invalid pushBack(order) call for Limit with price: " << limit_price << "! Invalid new Order.\n";
            return;
        }

        m_orders.emplace_back(std::move(order));
    }

    bool Limit::isEmpty() const noexcept {
        return !m_orders.size();
    }

    Limit::iterator Limit::begin() noexcept {
        return m_orders.begin();
    }

    Limit::iterator Limit::end() noexcept {
        return m_orders.end();
    }

    Limit::const_iterator Limit::begin() const noexcept {
        return m_orders.begin();
    }

    Limit::const_iterator Limit::end() const noexcept {
        return m_orders.end();
    }
}