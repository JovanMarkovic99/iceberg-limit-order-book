#pragma once

namespace jvn 
{
    enum class OrderType : bool {
        BUY,
        SELL
    };

    struct Order {
        using limit_type = short;
        using quantity_type = int;

        OrderType order_type;
        int id;
        limit_type limit;
        quantity_type quantity;

        Order(OrderType order_type, int id, limit_type limit, quantity_type quantity);

        // Returns the current quantity or the peak_size, whichever is smaller
        [[nodiscard]] virtual quantity_type getVolume() const noexcept;
    };

    struct IcebergOrder : Order {
        quantity_type peak_size;

        IcebergOrder(OrderType order_type, int id, limit_type limit, quantity_type quantity, quantity_type peak_size);

        [[nodiscard]] quantity_type getVolume() const noexcept final override;
    };
}