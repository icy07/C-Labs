#include <iostream>
#include <string>
using namespace std;

class Character {
private:
    string name;
    int health;
    int attack;
    int defense;

public:
    Character(string n, int h, int a, int d)
        : name(n), health(h), attack(a), defense(d) {
    }

    int getHealth() const {
        return health;
    }

    void displayInfo() const {
        cout << "Name: " << name << endl;
        cout << "Health: " << health << endl;
        cout << "Attack: " << attack << endl;
        cout << "Defense: " << defense << endl << endl;
    }

    void attackEnemy(Character& enemy) {
        int damage = attack - enemy.defense;
        if (damage < 0) damage = 0;
        enemy.takeDamage(damage);
    }

    void heal(int amount) {
        health += amount;
        if (health > 100) {
            health = 100;
        }
    }

    void takeDamage(int amount) {
        health -= amount;
        if (health < 0) {
            health = 0;
        }
    }
};

int main() {
    Character hero("Hero", 100, 20, 10);
    Character enemy("Enemy", 50, 15, 5);

    cout << "Initial stats:" << endl;
    hero.displayInfo();
    enemy.displayInfo();

    hero.attackEnemy(enemy);
    cout << "After hero attacks enemy:" << endl;
    enemy.displayInfo();

    enemy.attackEnemy(hero);
    cout << "After enemy attacks hero:" << endl;
    hero.displayInfo();

    hero.heal(10);
    cout << "After healing hero:" << endl;
    hero.displayInfo();

    enemy.takeDamage(40);
    cout << "Enemy takes 40 damage:" << endl;
    enemy.displayInfo();

    return 0;
}