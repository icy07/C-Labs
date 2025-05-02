#include <iostream>
#include <memory>
#include <string>

class Inventory {
private:
    std::unique_ptr<std::string[]> items; 
    size_t capacity;                     
    size_t count;                         

    void resize() {
        size_t new_capacity = capacity * 2;
        auto new_items = std::make_unique<std::string[]>(new_capacity);

        for (size_t i = 0; i < count; ++i) {
            new_items[i] = items[i];
        }

        items = std::move(new_items);
        capacity = new_capacity;
    }

public:
    Inventory() : capacity(2), count(0) {
        items = std::make_unique<std::string[]>(capacity);
    }

    void addItem(const std::string& item) {
        if (count >= capacity) {
            resize();
        }
        items[count++] = item;
    }

    void displayInventory() const {
        std::cout << "=== Inventory (" << count << "/" << capacity << ") ===\n";
        for (size_t i = 0; i < count; ++i) {
            std::cout << i + 1 << ". " << items[i] << "\n";
        }
        std::cout << "=====================\n";
    }
};

int main() {
    Inventory inv;
    inv.addItem("Steel Sword");
    inv.addItem("Wooden Shield");
    inv.addItem("Health Potion");
    inv.addItem("Magic Scroll");
    inv.addItem("Gold Key");

    inv.displayInventory();

    return 0;
}