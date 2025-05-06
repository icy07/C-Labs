#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>
#include <random>

std::mutex battleMutex;

class Character {
private:
    std::string name;
    int health;
    int attack;
    int defense;

public:
    Character(const std::string& n, int h, int a, int d)
        : name(n), health(h), attack(a), defense(d) {
    }

    void takeDamage(int damage) {
        std::lock_guard<std::mutex> lock(battleMutex);
        health -= damage;
        if (health < 0) health = 0;
    }

    bool isAlive() const {
        std::lock_guard<std::mutex> lock(battleMutex);
        return health > 0;
    }

    void attackTarget(Character& target) {
        int damage = attack - target.defense;
        if (damage > 0) {
            target.takeDamage(damage);
            std::lock_guard<std::mutex> lock(battleMutex);
            std::cout << name << " attacks for " << damage << " damage!\n";
        }
    }

    void displayStatus() const {
        std::lock_guard<std::mutex> lock(battleMutex);
        std::cout << name << " HP: " << health << "\n";
    }
};

void battleLoop(Character& hero, Character& monster) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> delay(500, 1500);

    while (hero.isAlive() && monster.isAlive()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(delay(gen)));
        hero.attackTarget(monster);
        if (monster.isAlive()) {
            monster.attackTarget(hero);
        }
    }

    std::lock_guard<std::mutex> lock(battleMutex);
    if (hero.isAlive()) {
        std::cout << "Hero wins!\n";
    }
    else {
        std::cout << "Monster wins!\n";
    }
}

int main() {
    Character hero("Hero", 100, 20, 10);
    Character monster("Dragon", 150, 25, 5);

    std::thread battleThread(battleLoop, std::ref(hero), std::ref(monster));

    while (hero.isAlive() && monster.isAlive()) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        hero.displayStatus();
        monster.displayStatus();
        std::cout << "-------------\n";
    }

    battleThread.join();
    return 0;
}