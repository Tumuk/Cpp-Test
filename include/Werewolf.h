//
// Created by Joshua Sutton
// 24/09/2018.
//

#ifndef WEREWOLF_H
#define WEREWOLF_H

#include "Entity.h"
#include "Monster.h"

class Werewolf : public Monster
{
public:
    Werewolf(std::string name = "AAA", int closeDamage = 0, int distanceDamage = 0,
             std::string noise = "awoo", int loudness = 0);
    void howl(Entity &e);

private:
    int loudness;
};

#endif
