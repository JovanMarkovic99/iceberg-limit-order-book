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
        virtual ~Order() = default;

        // Returns the current quantity or the peak_size, whichever is smaller
        [[nodiscard]] virtual quantity_type getVolume() const noexcept;
        virtual void matchVolume(quantity_type volume) noexcept;
    };

    struct IcebergOrder : Order {
        quantity_type peak_size;
        quantity_type current_peak_size;

        IcebergOrder(OrderType order_type, int id, limit_type limit, quantity_type quantity, quantity_type peak_size);
        ~IcebergOrder() override = default;

        [[nodiscard]] quantity_type getVolume() const noexcept final override;
        virtual void matchVolume(quantity_type volume) noexcept final override;
    };
}