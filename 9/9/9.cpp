#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <stdexcept>
#include <sstream>
#include <ctime>

template <typename T>
class Logger {
private:
    std::string filename;

public:
    Logger(const std::string& fname) : filename(fname) {
        std::ofstream file(filename, std::ios::app);
        if (!file) {
            throw std::runtime_error("Failed to open log file: " + filename);
        }
        file << "Log started at " << std::time(nullptr) << "\n";
    }

    void log(const T& event) {
        std::ofstream file(filename, std::ios::app);
        if (!file) {
            throw std::runtime_error("Failed to append to log file: " + filename);
        }
        file << event << "\n";
    }
};

class Inventory {
private:
    std::vector<std::string> items;

public:
    void addItem(const std::string& item) {
        if (item.empty()) {
            throw std::invalid_argument("Item name cannot be empty.");
        }
        items.push_back(item);
        std::cout << "Added " << item << " to inventory.\n";
    }

    void removeItem(const std::string& item) {
        for (auto it = items.begin(); it != items.end(); ++it) {
            if (*it == item) {
                items.erase(it);
                std::cout << "Removed " << item << " from inventory.\n";
                return;
            }
        }
        throw std::invalid_argument("Item " + item + " not found in inventory.");
    }

    void displayInventory() const {
        if (items.empty()) {
            std::cout << "Inventory is empty.\n";
            return;
        }
        std::cout << "Inventory:\n";
        for (const auto& item : items) {
            std::cout << "- " << item << "\n";
        }
    }

    std::string serialize() const {
        std::stringstream ss;
        ss << items.size();
        for (const auto& item : items) {
            ss << " " << item;
        }
        return ss.str();
    }

    void deserialize(const std::string& data) {
        std::istringstream iss(data);
        size_t count;
        iss >> count;
        items.clear();
        std::string item;
        std::getline(iss, item); 
        for (size_t i = 0; i < count; ++i) {
            std::getline(iss, item, ' ');
            if (!item.empty()) {
                items.push_back(item);
            }
        }
    }
};

class Monster {
protected:
    std::string name;
    int health;
    int attack;
    int defense;

public:
    Monster(const std::string& n, int h, int a, int d)
        : name(n), health(h), attack(a), defense(d) {
        validate();
    }

    virtual void displayInfo() const {
        std::cout << "Monster: " << name << ", HP: " << health
            << ", Attack: " << attack << ", Defense: " << defense << std::endl;
    }

    virtual std::string getType() const = 0;
    virtual std::string serialize() const {
        return getType() + "," + name + "," + std::to_string(health) + "," +
            std::to_string(attack) + "," + std::to_string(defense);
    }

    virtual ~Monster() = default;

    std::string getName() const { return name; }
    int getHealth() const { return health; }
    int getAttack() const { return attack; }
    int getDefense() const { return defense; }

    void takeDamage(int damage, Logger<std::string>& logger) {
        if (damage < 0) {
            throw std::invalid_argument("Damage cannot be negative.");
        }
        health -= damage;
        if (health < 0) {
            health = 0;
            logger.log(name + " has been defeated!");
            throw std::runtime_error("Health dropped below 0 for " + name);
        }
        logger.log(name + " takes " + std::to_string(damage) + " damage, HP now " + std::to_string(health));
    }

protected:
    void validate() const {
        if (health <= 0) throw std::invalid_argument("Health must be positive.");
        if (name.empty()) throw std::invalid_argument("Name cannot be empty.");
        if (attack < 0 || defense < 0) throw std::invalid_argument("Attack and defense must be non-negative.");
    }
};

class Goblin : public Monster {
public:
    Goblin(const std::string& n, int h = 50, int a = 15, int d = 5)
        : Monster(n, h, a, d) {
    }

    std::string getType() const override { return "Goblin"; }
};

class Dragon : public Monster {
public:
    Dragon(const std::string& n, int h = 200, int a = 30, int d = 20)
        : Monster(n, h, a, d) {
    }

    std::string getType() const override { return "Dragon"; }
};

class Zombie : public Monster {
public:
    Zombie(const std::string& n, int h = 40, int a = 10, int d = 15)
        : Monster(n, h, a, d) {
    } 

    std::string getType() const override { return "Zombie"; }
};

class Character {
private:
    std::string name;
    int health;
    int attack;
    int defense;
    int level;
    int experience;
    Inventory inventory;

public:
    Character(const std::string& n, int h, int a, int d)
        : name(n), health(h), attack(a), defense(d), level(1), experience(0) {
        validate();
    }

    void attackEnemy(Monster& enemy, Logger<std::string>& logger) {
        int damage = attack - enemy.getDefense();
        if (damage > 0) {
            enemy.takeDamage(damage, logger);
            logger.log(name + " attacks " + enemy.getName() + " for " + std::to_string(damage) + " damage!");
        }
        else {
            logger.log(name + " attacks " + enemy.getName() + ", but it has no effect!");
        }
    }

    void takeDamage(int damage, Logger<std::string>& logger) {
        if (damage < 0) {
            throw std::invalid_argument("Damage cannot be negative.");
        }
        health -= damage;
        if (health < 0) {
            health = 0;
            logger.log(name + " has been defeated!");
            throw std::runtime_error("Health dropped below 0 for " + name);
        }
        logger.log(name + " takes " + std::to_string(damage) + " damage, HP now " + std::to_string(health));
    }

    void heal(int amount, Logger<std::string>& logger) {
        if (amount < 0) {
            throw std::invalid_argument("Heal amount cannot be negative.");
        }
        health += amount;
        if (health > 100) health = 100;
        logger.log(name + " heals for " + std::to_string(amount) + " HP!");
    }

    void gainExperience(int exp, Logger<std::string>& logger) {
        if (exp < 0) {
            throw std::invalid_argument("Experience cannot be negative.");
        }
        experience += exp;
        logger.log(name + " gains " + std::to_string(exp) + " experience!");
        while (experience >= 100) {
            level++;
            experience -= 100;
            logger.log(name + " leveled up to level " + std::to_string(level) + "!");
        }
    }

    void addItem(const std::string& item, Logger<std::string>& logger) {
        inventory.addItem(item);
        logger.log(name + " added " + item + " to inventory.");
    }

    void removeItem(const std::string& item, Logger<std::string>& logger) {
        inventory.removeItem(item);
        logger.log(name + " removed " + item + " from inventory.");
    }

    void displayInfo() const {
        std::cout << "Name: " << name << ", HP: " << health
            << ", Attack: " << attack << ", Defense: " << defense
            << ", Level: " << level << ", Experience: " << experience << std::endl;
        inventory.displayInventory();
    }

    std::string serialize() const {
        return name + "," + std::to_string(health) + "," + std::to_string(attack) + "," +
            std::to_string(defense) + "," + std::to_string(level) + "," +
            std::to_string(experience) + "," + inventory.serialize();
    }

    void deserialize(const std::string& data, Logger<std::string>& logger) {
        std::istringstream iss(data);
        std::string token;
        std::vector<std::string> tokens;
        while (std::getline(iss, token, ',')) {
            tokens.push_back(token);
        }
        if (tokens.size() < 7) {
            throw std::runtime_error("Invalid character data format.");
        }
        name = tokens[0];
        try {
            health = std::stoi(tokens[1]);
            attack = std::stoi(tokens[2]);
            defense = std::stoi(tokens[3]);
            level = std::stoi(tokens[4]);
            experience = std::stoi(tokens[5]);
            inventory.deserialize(tokens[6]);
            validate();
        }
        catch (const std::exception& e) {
            throw std::runtime_error("Invalid numeric data in character: " + std::string(e.what()));
        }
        logger.log("Loaded character: " + name);
    }

    std::string getName() const { return name; }
    int getHealth() const { return health; }
    int getAttack() const { return attack; }
    int getDefense() const { return defense; }

private:
    void validate() const {
        if (health <= 0) throw std::invalid_argument("Health must be positive.");
        if (name.empty()) throw std::invalid_argument("Name cannot be empty.");
        if (attack < 0 || defense < 0) throw std::invalid_argument("Attack and defense must be non-negative.");
        if (level < 1) throw std::invalid_argument("Level must be at least 1.");
        if (experience < 0) throw std::invalid_argument("Experience cannot be negative.");
    }
};

class Game {
private:
    Character player;
    std::vector<std::unique_ptr<Monster>> monsters;
    Logger<std::string> logger;
    bool running;

public:
    Game(const std::string& playerName)
        : player(playerName, 100, 20, 10), logger("game.log"), running(true) {
    }

    void addMonster(std::unique_ptr<Monster> monster) {
        monsters.push_back(std::move(monster));
        logger.log("Added monster: " + monsters.back()->getName());
    }

    void combat() {
        if (monsters.empty()) {
            std::cout << "No monsters to fight!\n";
            return;
        }
        auto& monster = *monsters.front();
        std::cout << "\nFighting " << monster.getName() << "!\n";

        try {
            while (player.getHealth() > 0 && monster.getHealth() > 0) {
                player.attackEnemy(monster, logger);
                if (monster.getHealth() > 0) {
                    player.takeDamage(monster.getAttack() - player.getDefense(), logger);
                }
            }
            if (monster.getHealth() <= 0) {
                player.gainExperience(50, logger);
                monsters.erase(monsters.begin());
            }
        }
        catch (const std::runtime_error& e) {
            std::cout << "Combat ended: " << e.what() << "\n";
            if (player.getHealth() <= 0) {
                running = false;
            }
        }
    }

    void saveProgress(const std::string& filename) {
        std::ofstream file(filename);
        if (!file) {
            throw std::runtime_error("Failed to open file for writing: " + filename);
        }
        file << player.serialize() << "\n";
        file << monsters.size() << "\n";
        for (const auto& monster : monsters) {
            file << monster->serialize() << "\n";
        }
        logger.log("Game progress saved to " + filename);
    }

    void loadProgress(const std::string& filename) {
        std::ifstream file(filename);
        if (!file) {
            throw std::runtime_error("Failed to open file for reading: " + filename);
        }

        std::string line;
        std::getline(file, line);
        player.deserialize(line, logger);

        size_t monsterCount;
        file >> monsterCount;
        std::getline(file, line);
        monsters.clear();
        for (size_t i = 0; i < monsterCount; ++i) {
            std::getline(file, line);
            std::istringstream iss(line);
            std::string type, name;
            int health, attack, defense;
            char comma;
            std::getline(iss, type, ',');
            std::getline(iss, name, ',');
            iss >> health >> comma >> attack >> comma >> defense;
            try {
                if (type == "Goblin") {
                    monsters.push_back(std::make_unique<Goblin>(name, health, attack, defense));
                }
                else if (type == "Dragon") {
                    monsters.push_back(std::make_unique<Dragon>(name, health, attack, defense));
                }
                else if (type == "Skeleton") {
                    monsters.push_back(std::make_unique<Zombie>(name, health, attack, defense));
                }
                else {
                    throw std::runtime_error("Unknown monster type: " + type);
                }
                logger.log("Loaded monster: " + name);
            }
            catch (const std::exception& e) {
                std::cerr << "Warning: Failed to load monster: " << e.what() << "\n";
            }
        }
        logger.log("Game progress loaded from " + filename);
    }

    void play() {
        while (running) {
            std::cout << "\nPlayer Info:\n";
            player.displayInfo();
            std::cout << "\nMonsters:\n";
            for (const auto& monster : monsters) {
                monster->displayInfo();
            }
            std::cout << "\nOptions: (1) Fight, (2) Heal, (3) Add Item, (4) Remove Item, (5) Save, (6) Load, (7) Exit\n";
            int choice;
            std::cin >> choice;
            try {
                switch (choice) {
                case 1:
                    combat();
                    break;
                case 2:
                    player.heal(20, logger);
                    break;
                case 3: {
                    std::string item;
                    std::cout << "Enter item name: ";
                    std::cin >> item;
                    player.addItem(item, logger);
                    break;
                }
                case 4: {
                    std::string item;
                    std::cout << "Enter item name: ";
                    std::cin >> item;
                    player.removeItem(item, logger);
                    break;
                }
                case 5:
                    saveProgress("game_save.txt");
                    break;
                case 6:
                    loadProgress("game_save.txt");
                    break;
                case 7:
                    running = false;
                    break;
                default:
                    std::cout << "Invalid option.\n";
                }
            }
            catch (const std::exception& e) {
                std::cerr << "Error: " << e.what() << "\n";
            }
        }
        logger.log("Game ended.");
    }
};

int main() {
    try {
        Game game("Hero");
        game.addMonster(std::make_unique<Goblin>("Goblin"));
        game.addMonster(std::make_unique<Dragon>("Dragon"));
        game.addMonster(std::make_unique<Zombie>("Zombie"));
        game.play();
    }
    catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << "\n";
        return 1;
    }
    return 0;
}