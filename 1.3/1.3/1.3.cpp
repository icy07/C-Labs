#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>

class Entity {
protected:
    std::string name;
    int health;
    int maxHealth;
    int attackPower;
    int defense;

public:
    Entity(const std::string& n, int h, int a, int d)
        : name(n), health(h), maxHealth(h), attackPower(a), defense(d) {
    }

    const std::string& getName() const { return name; }
    int getHealth() const { return health; }
    int getAttack() const { return attackPower; }
    int getDefense() const { return defense; }

    void setHealth(int value) {
        health = value;
        if (health < 0) health = 0;
        if (health > maxHealth) health = maxHealth;
    }

    virtual void attackEntity(Entity& target) {
        int damage = getAttack() - target.getDefense();
        if (damage > 0) {
            target.setHealth(target.getHealth() - damage);
            std::cout << getName() << " attacks " << target.getName()
                << " for " << damage << " damage!\n";
        }
        else {
            std::cout << getName() << " attacks " << target.getName()
                << ", but it has no effect!\n";
        }
    }

    virtual void displayInfo() const {
        std::cout << "Entity: " << getName()
            << ", HP: " << getHealth() << "/" << maxHealth
            << ", Attack: " << getAttack()
            << ", Defense: " << getDefense() << std::endl;
    }

    virtual void heal(int amount) {
        int newHealth = getHealth() + amount;
        setHealth(newHealth);
        std::cout << getName() << " heals " << amount
            << " HP. Now HP: " << getHealth() << std::endl;
    }

    virtual ~Entity() = default;
};

class Character : public Entity {
public:
    Character(const std::string& n, int h, int a, int d)
        : Entity(n, h, a, d) {
    }

    void attackEntity(Entity& target) override {
        int damage = getAttack() - target.getDefense();
        if (damage > 0) {
            if (rand() % 100 < 20) { // 20% шанс крита
                damage *= 2;
                std::cout << "Critical hit! ";
            }
            target.setHealth(target.getHealth() - damage);
            std::cout << getName() << " attacks " << target.getName()
                << " for " << damage << " damage!\n";
        }
        else {
            std::cout << getName() << " attacks " << target.getName()
                << ", but it has no effect!\n";
        }
    }

    void displayInfo() const override {
        std::cout << "Character: " << getName()
            << ", HP: " << getHealth() << "/" << maxHealth
            << ", Attack: " << getAttack()
            << ", Defense: " << getDefense() << std::endl;
    }

    void heal(int amount) override {
        int effectiveHeal = static_cast<int>(amount * 1.5);
        setHealth(getHealth() + effectiveHeal);
        std::cout << getName() << " uses healing skill! Restores "
            << effectiveHeal << " HP. Now HP: " << getHealth() << std::endl;
    }
};

class Monster : public Entity {
public:
    Monster(const std::string& n, int h, int a, int d)
        : Entity(n, h, a, d) {
    }

    void attackEntity(Entity& target) override {
        int damage = getAttack() - target.getDefense();
        if (damage > 0) {
            if (rand() % 100 < 30) { // 30% шанс яда
                damage += 5;
                std::cout << "Poisonous attack! ";
            }
            target.setHealth(target.getHealth() - damage);
            std::cout << getName() << " attacks " << target.getName()
                << " for " << damage << " damage!\n";
        }
        else {
            std::cout << getName() << " attacks " << target.getName()
                << ", but it has no effect!\n";
        }
    }

    void displayInfo() const override {
        std::cout << "Monster: " << getName()
            << ", HP: " << getHealth() << "/" << maxHealth
            << ", Attack: " << getAttack()
            << ", Defense: " << getDefense() << std::endl;
    }
};

class Boss : public Monster {
    std::string specialAbility;

public:
    Boss(const std::string& n, int h, int a, int d, const std::string& ability)
        : Monster(n, h, a, d), specialAbility(ability) {
    }

    void attackEntity(Entity& target) override {
        int damage = getAttack() - target.getDefense();
        if (damage > 0) {
            if (rand() % 100 < 25) { // 25% шанс особой атаки
                damage += 15;
                std::cout << specialAbility << "! ";
            }
            target.setHealth(target.getHealth() - damage);
            std::cout << getName() << " attacks " << target.getName()
                << " for " << damage << " damage!\n";
        }
        else {
            std::cout << getName() << " attacks " << target.getName()
                << ", but it has no effect!\n";
        }
    }

    void displayInfo() const override {
        std::cout << "Boss: " << getName()
            << ", HP: " << getHealth() << "/" << maxHealth
            << ", Attack: " << getAttack()
            << ", Defense: " << getDefense()
            << ", Ability: " << specialAbility << std::endl;
    }
};

int main() {
    srand(static_cast<unsigned>(time(nullptr)));

    Character hero("Hero", 100, 20, 10);
    Monster goblin("Goblin", 50, 15, 5);
    Boss dragon("Dragon Lord", 300, 40, 25, "Inferno Breath");

    Entity* entities[] = { &hero, &goblin, &dragon };

    for (Entity* entity : entities) {
        entity->displayInfo();
    }

    hero.attackEntity(goblin);
    dragon.attackEntity(hero);
    goblin.attackEntity(hero);

    hero.heal(20);
    dragon.heal(50);

    std::cout << "\nFinal status:\n";
    for (Entity* entity : entities) {
        entity->displayInfo();
    }

    return 0;
}