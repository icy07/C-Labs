#include <iostream>
#include <deque>
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
        if (!items.empty()) {
            items.pop_front();
        }
    }

    T front() const {
        return items.front();
    }

    bool empty() const {
        return items.empty();
    }
};

int main() {
    Queue<std::string> stringQueue;
    stringQueue.push("First");
    stringQueue.push("Second");
    stringQueue.push("Third");

    std::cout << "String Queue:\n";
    while (!stringQueue.empty()) {
        std::cout << stringQueue.front() << "\n";
        stringQueue.pop();
    }

    Queue<int> intQueue;
    intQueue.push(10);
    intQueue.push(20);
    intQueue.push(30);

    std::cout << "\nInteger Queue:\n";
    while (!intQueue.empty()) {
        std::cout << intQueue.front() << "\n";
        intQueue.pop();
    }

    return 0;
}