#include <iostream>
#include <deque>
#include <stdexcept>
#include <string>

template <typename T>
class Queue {
private:
    std::deque<T> items;

public:
    void push(const T& item) {
        items.push_back(item);
    }

    void pop() {
        if (items.empty()) {
            throw std::runtime_error("Cannot pop from empty queue");
        }
        items.pop_front();
    }

    T front() const {
        if (items.empty()) {
            throw std::runtime_error("Cannot get front of empty queue");
        }
        return items.front();
    }

    bool empty() const {
        return items.empty();
    }
};

int main() {
    try {
        Queue<std::string> stringQueue;
        stringQueue.push("First");
        stringQueue.push("Second");

        std::cout << "String Queue:\n";
        while (!stringQueue.empty()) {
            std::cout << stringQueue.front() << "\n";
            stringQueue.pop();
        }
        stringQueue.pop();

    }
    catch (const std::runtime_error& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    try {
        Queue<int> intQueue;
        intQueue.push(10);
        intQueue.pop();
        intQueue.pop();

    }
    catch (const std::runtime_error& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}