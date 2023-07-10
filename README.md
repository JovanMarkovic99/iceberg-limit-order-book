# Limit Order Book with Iceberg Order support

## Introduction

This project implements a Limit Order Book with support for Iceberg Orders. The Order Book efficiently manages and matches buy and sell orders based on specified limits and quantities. The design aims to optimize time complexity and efficiency, providing fast insertion, and matching operations.

## Building & Testing

To build and run the project, follow these steps:

1. Clone the repository to your local machine:
    ``` 
    git clone https://github.com/JovanMarkovic99/iceberg-limit-order-book 
    ```

2. Navigate to the project's root directory and create a build directory:
    ``` 
    cd iceberg-limit-order-book 
    mkdir build
    ```

3. Build the project by running `cmake` in the build directory:
    ```
    cd build
    cmake ..
    cmake --build .
    ```

4. (Optional) You can run any provided tests to ensure the project functions as expected:
    ```
    ctest -C Debug
    ```

5. Run the program `main.exe`.

## Order Design

The Order Book supports two types of orders: `Order` and `IcebergOrder`.

- The `Order` struct represents an individual order and includes the following properties:

```cpp
struct Order {
    OrderType order_type;
    int id;
    limit_type limit;
    quantity_type quantity;

    virtual quantity_type getVolume();
    ...
};
```

- The `IcebergOrder` struct represents an Iceberg Order and includes an additional property:

```cpp
struct IcebergOrder : Order {
    quantity_type peak_size;

    virtual quantity_type getVolume();
    ...
};
```
The `getVolume()` function calculates the volume of the order. For `Order`, the volume is equal to its current quantity. For `IcebergOrder`, the volume is the minimum value between its peak size and its current full quantity.

## Book Design

The design of the Order Book utilizes a Binary Search Tree (BST) (`std::map`) of limits, where each limit holds a `std::deque` of individual orders coorsponding to the given limit/price. This design optimizes time complexity for insertion, and matching operations. To achieve a O(1) worst-case time complexity for matching orders, the top-priority buy and sell limits are cached.

### Limit Class

The `Limit` class represents a limit in the Order Book. It maintains a deque of orders and provides efficient insertion and removal operations from the front and back.

```cpp
class Limit {
private:
    std::deque<std::unique_ptr<Order>> m_orders;
public:
    limit_type limit_price;

    std::unique_ptr<Order> popFront();
    std::unique_ptr<Order> popBack();

    void pushFront(std::unique_ptr<Order> order);
    void pushBack(std::unique_ptr<Order> order);

    iterator begin() noexcept;
    iterator end() noexcept;
    ...
};
```

### OrderBook Class

The `OrderBook` class represents the entire Order Book. It includes two maps, `m_sell_map` and `m_buy_map`, which store sell and buy limits, respectively. The `m_lowest_sell` and `m_highest_buy` iterators are used to cache the top-priority sell and buy limits needed for O(1) matching.

```cpp
class OrderBook {
private:
    std::map<limit_type, Limit> m_sell_map;
    std::map<limit_type, Limit, std::greater<limit_type>> m_buy_map;
    sell_map_type::iterator m_lowest_sell;
    buy_map_type::iterator m_highest_buy;
    ...
public:
    void processOrder(std::unique_ptr<Order> order);
    ...
};
```

## Design Considerations

While the design outlined above provides efficient time complexity for most operations, it does have certain limitations. Notably, random access and modification of orders, may suffer from suboptimal time complexity. To address this, a hashmap can be introduced to achieve O(1) lookup of order locations. However, modifying orders would still result in a worst-case O(n) time complexity due to the need for random position removal.

To achieve true O(1) time complexity for order modification, the deque in the `Limit` class can be substituted with a doubly-linked list.
