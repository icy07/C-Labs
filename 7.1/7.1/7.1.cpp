#include <iostream>
#include <fstream>
#include <vector>
#include <memory>
#include <string>
#include <stdexcept>
#include <limits>

class Entity {
protected:
    std::string name;
    int health;
    int level;

public:
    Entity(const std::string& n, int h, int lvl) : name(n), health(h), level(lvl) {}
    virtual void writeToFile(std::ofstream& file) const = 0;
    virtual void displayInfo() const = 0;
    virtual void edit() = 0;
    virtual ~Entity() = default;
    static std::unique_ptr<Entity> createFromFile(std::ifstream& file);
};

class Player : public Entity {
    int experience;
public:
    Player(const std::string& n, int h, int lvl, int exp) : Entity(n, h, lvl), experience(exp) {}
    void writeToFile(std::ofstream& file) const override {
        file << "Player\n" << name << "\n" << health << "\n" << level << "\n" << experience << "\n";
    }
    void displayInfo() const override {
        std::cout << "Player: " << name << " | HP: " << health << " | Level: " << level << " | XP: " << experience << "\n";
    }
    void edit() override {
        std::cout << "New name (" << name << "): ";
        std::string newName;
        std::getline(std::cin, newName);
        if (!newName.empty()) name = newName;
        std::cout << "New health (" << health << "): ";
        std::cin >> health;
        std::cout << "New level (" << level << "): ";
        std::cin >> level;
        std::cout << "New experience (" << experience << "): ";
        std::cin >> experience;
        std::cin.ignore();
    }
};

class Enemy : public Entity {
    std::string type;
public:
    Enemy(const std::string& n, int h, int lvl, const std::string& t) : Entity(n, h, lvl), type(t) {}
    void writeToFile(std::ofstream& file) const override {
        file << "Enemy\n" << name << "\n" << health << "\n" << level << "\n" << type << "\n";
    }
    void displayInfo() const override {
        std::cout << "Enemy: " << name << " | HP: " << health << " | Level: " << level << " | Type: " << type << "\n";
    }
    void edit() override {
        std::cout << "New name (" << name << "): ";
        std::string newName;
        std::getline(std::cin, newName);
        if (!newName.empty()) name = newName;
        std::cout << "New health (" << health << "): ";
        std::cin >> health;
        std::cout << "New level (" << level << "): ";
        std::cin >> level;
        std::cin.ignore();
    }
};

std::unique_ptr<Entity> Entity::createFromFile(std::ifstream& file) {
    std::string type;
    std::getline(file, type);
    std::string name;
    std::getline(file, name);
    int health, level;
    file >> health >> level;
    file.ignore();
    if (type == "Player") {
        int exp;
        file >> exp;
        file.ignore();
        return std::make_unique<Player>(name, health, level, exp);
    }
    else if (type == "Enemy") {
        std::string enemyType;
        std::getline(file, enemyType);
        return std::make_unique<Enemy>(name, health, level, enemyType);
    }
    throw std::runtime_error("Unknown entity type");
}

template <typename T>
class GameManager {
    std::vector<std::unique_ptr<T>> entities;
public:
    void addEntity(std::unique_ptr<T> entity) {
        entities.push_back(std::move(entity));
    }
    void displayAll() const {
        for (size_t i = 0; i < entities.size(); ++i) {
            std::cout << i + 1 << ". ";
            entities[i]->displayInfo();
        }
    }
    void editEntity(size_t index) {
        if (index >= entities.size()) throw std::out_of_range("Invalid index");
        entities[index]->edit();
    }
    void saveToFile(const std::string& filename) const {
        std::ofstream file(filename);
        if (!file) throw std::runtime_error("File write error");
        for (const auto& entity : entities) entity->writeToFile(file);
    }
    void loadFromFile(const std::string& filename) {
        std::ifstream file(filename);
        if (!file) throw std::runtime_error("File read error");
        while (file.peek() != EOF) entities.push_back(T::createFromFile(file));
    }
};

int main() {
    try {
        GameManager<Entity> manager;
        manager.addEntity(std::make_unique<Player>("Hero", 100, 10, 1500));
        manager.addEntity(std::make_unique<Enemy>("Dragon", 500, 20, "Boss"));
        manager.displayAll();
        manager.editEntity(0);
        manager.saveToFile("save.txt");
        GameManager<Entity> loadedManager;
        loadedManager.loadFromFile("save.txt");
        loadedManager.displayAll();
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
    }
    return 0;
}