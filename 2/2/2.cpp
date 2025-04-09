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
        std::cout << "Weapon " << name << " created!\n";
    }

    ~Weapon() {
        std::cout << "Weapon " << name << " destroyed!\n";
    }

    void displayInfo() const {
        std::cout << "Weapon: " << name
            << "\nDamage: " << damage
            << "\nWeight: " << weight << " kg\n\n";
    }
};

int main() {
    Weapon sword("Dragon Slayer", 45, 8.2);
    Weapon bow("Shadow Bow", 32, 3.1);
    Weapon axe("Thunder Axe", 55, 10.5);

    std::cout << "\n=== Weapons Info ===\n";
    sword.displayInfo();
    bow.displayInfo();
    axe.displayInfo();

    {
        Weapon dagger("Poison Dagger", 25, 0.9);
        dagger.displayInfo();
    } 

    return 0;
}