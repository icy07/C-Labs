#include <iostream>
#include <string>

class Weapon {
private:
    std::string name;
    int damage;
    float weight;

public:
    Weapon(const std::string& n, int d, float w)
        : name(n), damage(d), weight(w) {
    }

    Weapon operator+(const Weapon& other) const {
        return Weapon(name + "+" + other.name,
            damage + other.damage,
            weight + other.weight);
    }

    bool operator>(const Weapon& other) const {
        return damage > other.damage;
    }

    void displayInfo() const {
        std::cout << "Weapon: " << name
            << " | Damage: " << damage
            << " | Weight: " << weight << "kg\n";
    }
};

int main() {
    Weapon sword("Dragon Slayer", 45, 8.2);
    Weapon bow("Shadow Bow", 35, 3.1);
    Weapon axe("Thunder Axe", 60, 10.5);

    Weapon combined = sword + bow;
    std::cout << "Combined weapon:\n";
    combined.displayInfo();

    std::cout << "\nComparison:\n";
    std::cout << "Sword > Bow: " << (sword > bow) << std::endl; 
    std::cout << "Axe > Sword: " << (axe > sword) << std::endl;
    std::cout << "Bow > Axe: " << (bow > axe) << std::endl;   

    return 0;
}