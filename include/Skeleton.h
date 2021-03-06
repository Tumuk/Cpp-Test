//
// Created by Joshua Sutton
// 24/09/2018.
//

#ifndef SKELETON_H
#define SKELETON_H

#include "Entity.h"
#include "Monster.h"

class Skeleton : public Monster
{
public:
    Skeleton(std::string name = "AAA", int closeDamage = 0, int distanceDamage = 0,
             int health = 100, std::string noise = "doot", int dootAbility = 0, int gold = 0);
    void doot(Entity &e);
    std::string getType();
    void castSpell(Entity &e);
    void howl(Entity &e);
    void stealLife(Entity &e);

private:
    int dootAbility;
};

#endif
