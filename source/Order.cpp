
#include "../include/Order.hpp"
#include <algorithm>

namespace jvn
{
    Order::Order(OrderType order_type, int id, limit_type limit, quantity_type quantity)
        :order_type(order_type),
        id(id),
        limit(limit),
        quantity(quantity)
    {}

    Order::quantity_type Order::getVolume() const noexcept {
        return quantity;
    }
    
    void Order::matchVolume(quantity_type volume) noexcept {
        quantity -= volume;
    }

    IcebergOrder::IcebergOrder(OrderType order_type, int id, limit_type limit, quantity_type quantity, quantity_type peak_size) :
        Order(order_type, id, limit, quantity),
        peak_size(peak_size),
        current_peak_size(peak_size)
    {}

    Order::quantity_type IcebergOrder::getVolume() const noexcept {
        return std::min(quantity, current_peak_size);
    }

    void IcebergOrder::matchVolume(quantity_type volume) noexcept {
        Order::matchVolume(volume);
        
        current_peak_size -= volume;
        if (current_peak_size == 0)
            current_peak_size = peak_size;
    }
}