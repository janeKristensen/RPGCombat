#include <vector>
#include <iostream>

class Attack {
public:
    Attack(std::string name, int attackStat) 
        : m_name(name), m_attackStat(attackStat) {}

    std::string getName() const     { return m_name; }
    int getAttackStat() const       { return m_attackStat; }
    void setAttackStat(int value)   { m_attackStat = value; }
   
private:
    std::string m_name = "";
    int m_attackStat;
};

class Item {
public:
    Item(std::string name, int healStat) 
        : m_name(name), m_healStat(healStat) {}

    std::string getName() const     { return m_name; }
    int getHealStat() const         { return m_healStat; }

private:
    std::string m_name;
    int m_healStat;
};

class DetailPrinter {
public:
    template <typename T>
    void printDetail(const std::string& name, const T& detail)
    {
        std::cout << name << detail << std::endl;
    }
private:

};

#define ATTACK_ARRAY std::vector<std::shared_ptr<Attack>>
#define ITEM_ARRAY std::vector<std::shared_ptr<Item>>
#define MAX_HEALTH 100
#define MAX_DEFENSE 10
#define MAX_STRENGTH 10

class Character {
public:
    Character(std::string name, ATTACK_ARRAY& attacks, ITEM_ARRAY& items)
    {
        m_name = name;
        m_attacks = attacks;
        m_items = items;
    }

    int getHealth()             { return m_health; }
    int getDefense()            { return m_defenseMultiplier; }
    int getStrength()           { return m_strengthMultiplier; }
    ATTACK_ARRAY& getAttacks()  { return m_attacks; }
    ITEM_ARRAY& getItems()      { return m_items; }

    void setHealth(int value) 
    { 
        if (value > MAX_HEALTH) m_health = MAX_HEALTH;
        else if (value < 0) return;
        else m_health = value; 
    }

    void setDefense(int value) 
    { 
        if (value > MAX_DEFENSE) m_defenseMultiplier = MAX_DEFENSE;
        else if (value < 0) return;
        else m_defenseMultiplier = value; 
    }

    void setStrength(int value) 
    { 
        if (value > MAX_STRENGTH) m_strengthMultiplier = MAX_STRENGTH;
        else if (value < 0) return;
        else m_strengthMultiplier = value; 
    }
    
    void printStats() 
    { 
        printer->printDetail("Health: ", m_health);
        printer->printDetail("Defense: ", m_defenseMultiplier);
        printer->printDetail("Strength: ", m_strengthMultiplier);
    }
    void printName() 
    { 
        printer->printDetail("Name: ", m_name); 
    }
    
    
private:
    std::string m_name;
    ATTACK_ARRAY m_attacks;
    ITEM_ARRAY m_items;
    DetailPrinter* printer;
    int m_health                = MAX_HEALTH;
    int m_strengthMultiplier    = 1;
    int m_defenseMultiplier     = 1;
};



template <typename T>
std::shared_ptr<T> lookUp(std::vector<std::shared_ptr<T>>& arr, std::string itemName)
{
    for (const auto& p : arr) {
        if (p->getName() == itemName) return p;
    }
    return nullptr;
};

void onAttack(Character& character, int damage)
{
    int newHealth = character.getHealth() - (damage * character.getDefense());
    character.setHealth(newHealth);
}

void attackCharacter(Character& character, std::string attackName, Character& other) {

    auto attack = lookUp(character.getAttacks(), attackName);
    int damage = character.getStrength() * attack->getAttackStat();
    onAttack(other, damage);
}

void useItem(Character& character, std::string name)
{
    auto usedItem = lookUp(character.getItems(), name);
    if (usedItem != nullptr) {
        int newHealth = character.getHealth() + usedItem->getHealStat();
        character.setHealth(newHealth);
    }
}

int main()
{
    ATTACK_ARRAY attacks = {
        std::make_shared<Attack>(Attack("Slash", 10)),
        std::make_shared<Attack>(Attack("Hack", 5))
    };

    ITEM_ARRAY items = {
        std::make_shared<Item>(Item("MedPack", 8)),
        std::make_shared<Item>(Item("SuperMedPack", 15))
    };

    Character p1{ "Player 1", attacks, items };
    Character p2{ "Player 2", attacks, items };

    p1.printName();
    p1.printStats();
    p2.printName();
    p2.printStats();

    attackCharacter(p1, "Slash", p2);
    attackCharacter(p2, "Hack", p1);
    p1.printName();
    p1.printStats();
    p2.printName();
    p2.printStats();

    useItem(p2, "SuperMedPack");
    p2.printName();
    p2.printStats();
}

