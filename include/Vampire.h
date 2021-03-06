//
// Created by Joshua Sutton
// 24/09/2018.
//

#ifndef VAMPIRE_H
#define VAMPIRE_H

#include "Entity.h"
#include "Monster.h"

class Vampire : public Monster
{
public:
    Vampire(std::string name = "AAA", int closeDamage = 0, int distanceDamage = 0,
            int health = 100, std::string noise = "I vant to suck your blood!", int fullness = 0, int gold = 0);
    void stealLife(Entity &e);
    std::string getType();
    void doot(Entity &e);
    void castSpell(Entity &e);
    void howl(Entity &e);

private:
    int fullness;
};

#endif
