//
// Created by Joshua Sutton
// 10/09/2018.
//

#include <cstdlib>
#include <iostream>
#include <random>

#include "Functions.h"
#include "GlobalVariables.h"
#include "Potion.h"

using namespace std;

int hour;
int numHours;
int probabilityOfEncounter;
string* names;

int main(int argc, char* argv[])
{
    static default_random_engine generator(time(0));
    static uniform_int_distribution<int> distribution(0, 99);

    if(argc != 3)
    {
        cout << "Not enough arguments provided." << endl;
        cout << "./RPA [n] [p]" << endl << "n: maximum number of hours adventure should run for (1-48)" << endl;
        cout << "p: percentage chance of an encounter per hour (5-95)" << endl;
        return 1;
    }

    // Set up running time
    numHours = atoi(argv[1]);
    if(numHours < 1)
    {
    	cout << "Running time too low. Setting to the minimum (1)." << endl << endl;
        numHours = 1;
    }
    if(numHours > 48)
    {
    	cout << "Running time too high. Setting to the maximum (48)." << endl << endl;
        numHours = 48;
    }

    // Set up hour
    hour = 0;

    // Set up probability of encounter
    probabilityOfEncounter = atoi(argv[2]);
    if(probabilityOfEncounter < 5)
    {
    	cout << "Encounter probability too low. Setting to the minimum (5)" << endl << endl;
        probabilityOfEncounter = 5;
    }
    if(probabilityOfEncounter > 95)
    {
    	cout << "Encounter probability too high. Setting to the maximum (95)" << endl << endl;
        probabilityOfEncounter = 95;
    }

    cout << "Running for " << numHours << " hours." << endl;

    // Grab the list of names
    getNames();

    // Generate adventurers
    cout << endl << "===Your Adventurers===" << endl;
    Adventurer** adventurers = generateAdventurers();
    int numOfAdventurers = 7;
    for(int i = 0; i < 7; i++)
    {
        cout << "==" << (*adventurers[i]).getType() << "==" << endl;
        cout << *adventurers[i] << endl;
        if(adventurers[i]->getType() == "Offensive Mage" || adventurers[i]->getType() == "Defensive Mage")
        {
            cout << "--Spell Info--" << endl;
            adventurers[i]->getSpellInfo();
        }
        cout << endl;
    }


    // Main program loop
    cout << endl << "===Main Loop===" << endl;
    for(int i = 0; i < numHours; i++)
    {
        hour++;

        cout << "==Hour " << hour << "==" << endl;

        // Randomly find a potion and apply it to an adventurer
        if(distribution(generator)%2 == 1)
        {
            // Generate potion and apply to random adventurer
            cout << "Potion has been found!" << endl;
            Potion p;
            distribution = std::uniform_int_distribution<int>(0, 6);
            int advToApply = distribution(generator);
            while(adventurers[advToApply]->getHealth() <= 0)
                advToApply = distribution(generator);
            p.applyPotion(*adventurers[advToApply]);
            cout << endl;
        }

        // Reset the random number distribution
        distribution = uniform_int_distribution<int>(0, 99);

        // Check if an encounter is to be had this hour
        if(distribution(generator) < probabilityOfEncounter)
        {
            cout << "Random Encounter!!" << endl;
            // Generate the monsters
            distribution = uniform_int_distribution<int>(1, hour);
            int numOfMonsters = distribution(generator);
            Monster** monsters = generateMonsters(numOfMonsters);
            cout << "--Monsters this Round--" << endl;
            for(int i = 0; i < numOfMonsters; i++)
            {
                cout << "=" << monsters[i]->getType() << "=" << endl;
                cout << *monsters[i] << endl << endl;
            }

            int numMonstersThisRound = numOfMonsters;

            // Continue until all adventurers or monsters are dead
            while(numOfAdventurers > 0 || numOfMonsters > 0)
            {
                cout << "=Adventurers' Turn==" << endl;
                // Have adventurers fight monsters
                for(int i = 0; i < 7; i++)
                {
                    // Go to the next adventurer if the ith one is dead
                    if(adventurers[i]->getHealth() <= 0)
                        continue;

                    if(numOfMonsters == 0)
                        break;

                    // Randomly pick a monster
                    distribution = uniform_int_distribution<int>(0, numMonstersThisRound - 1);
                    int monster = distribution(generator);

                    // Make sure the monster is not dead
                    while(monsters[monster]->getHealth() <= 0)
                        monster = distribution(generator);

                    // Randomly pick an action for the fighter based on their class
                    if(adventurers[i]->getType() == "Offensive Mage" || adventurers[i]->getType() == "Defensive Mage")
                    {
                        // Either do a close or ranged attack, or cast a spell
                        distribution = uniform_int_distribution<int>(0, 2);
                        switch(distribution(generator))
                        {
                            case 0: // Close attack
                            {
                                adventurers[i]->closeAttack(*monsters[monster]);
                                break;
                            }
                            case 1: // Ranged attack
                            {
                                adventurers[i]->rangedAttack(*monsters[monster]);
                                break;
                            }
                            case 2: // Cast a spell
                            {
                                // Cast a spell on an adventurer if the mage is defensive
                                if(adventurers[i]->getType() == "Defensive Mage")
                                    adventurers[i]->castSpell(*adventurers[monster%numOfAdventurers]);
                                else
                                    adventurers[i]->castSpell(*monsters[monster]);
                                break;
                            }
                            default:
                            {
                                cout << adventurers[i]->getName() << " got confused and hurt themselves.\nThey take ";
                                cout << hour << " damage.";
                                adventurers[i]->takeDamage(hour);
                                break;
                            }
                        }
                    }
                    else if(adventurers[i]->getType() == "Rogue")
                    {
                        // Either do a close or ranged attack, or steal
                        distribution = uniform_int_distribution<int>(0, 2);
                        switch(distribution(generator))
                        {
                            case 0: // Close attack
                            {
                                adventurers[i]->closeAttack(*monsters[monster]);
                                break;
                            }
                            case 1: // Ranged attack
                            {
                                adventurers[i]->rangedAttack(*monsters[monster]);
                                break;
                            }
                            case 2: // Steal
                            {
                                adventurers[i]->stealGold(*monsters[monster]);
                                break;
                            }
                            default:
                            {
                                cout << adventurers[i]->getName() << " got confused and hurt themselves.\nThey take ";
                                cout << hour << " damage.";
                                adventurers[i]->takeDamage(hour);
                                break;
                            }
                        }
                    }
                    else if(adventurers[i]->getType() == "Warrior")
                    {
                        // Either do a close attack or be chivalrous
                        distribution = uniform_int_distribution<int>(0, 1);
                        switch(distribution(generator))
                        {
                            case 0: // Close attack
                            {
                                if(adventurers[i]->getLocation() == "far")
                                    adventurers[i]->move("close");
                                else
                                    adventurers[i]->closeAttack(*monsters[monster]);
                                break;
                            }
                            case 1: // Be chivalrous
                            {
                                adventurers[i]->beChivalrous(*adventurers[monster > 6 ? 6 : monster]);
                                break;
                            }
                            default:
                            {
                                cout << adventurers[i]->getName() << " got confused and hurt themselves.\nThey take ";
                                cout << hour << " damage.";
                                adventurers[i]->takeDamage(hour);
                                break;
                            }
                        }
                    }

                    // Kill the monster if its health drops below 0 and distribute the gold
                    if(monsters[monster]->getHealth() <= 0)
                    {
                        int freeGold = adventurers[i]->kill(*monsters[monster]);

                        numOfMonsters--;

                        if(!(freeGold <= 0))
                        {
                            // Distribute the gold
                            if(freeGold%numOfAdventurers != 0)
                            {
                                // Randomly distribute leftover gold
                                int goldRemainder = freeGold%numOfAdventurers;
                                freeGold -= goldRemainder;
                                for(int i = 0; i < goldRemainder; i++)
                                {
                                    distribution = uniform_int_distribution<int>(0, 6);
                                    int adventurer = distribution(generator);
                                    while(adventurers[adventurer]->getHealth() <=0)
                                        adventurer = distribution(generator);
                                    adventurers[adventurer]->addGold(1);
                                    cout << adventurers[adventurer]->getType() << " " << adventurers[adventurer]->getName();
                                    cout << " got 1 gold!" << endl;
                                }
                                goldRemainder = 0;

                                // Give each adventurer the same amount of remaining gold
                                cout << freeGold/numOfAdventurers << " gold was given to each adventurer." << endl << endl;
                            }

                            for(int i = 0; i < 7; i++)
                            {
                                if(adventurers[i]->getHealth() > 0)
                                    adventurers[i]->addGold(freeGold/numOfAdventurers);
                            }
                        }
                    }

                    // If all the monsters have died, go to the next loop
                    if(numOfMonsters == 0)
                        break;
                }

                if(numOfMonsters == 0)
                    break;

                cout << "=Monsters' Turn=" << endl;
                // Have monsters fight adventurers
                for(int i = 0; i < numOfMonsters; i++)
                {
                    // Go to the next monster if the ith one is dead
                    if(monsters[i]->getHealth() <= 0)
                        continue;

                    // Randomly pick an adventurer
                    distribution = uniform_int_distribution<int>(0, 6);
                    int adventurer = distribution(generator);

                    // Make sure the adventurer is not dead
                    while(adventurers[adventurer]->getHealth() <= 0)
                        adventurer = distribution(generator);

                    // Randomly pick an action for the fighter based on their class
                    if(monsters[i]->getType() == "Tree" || monsters[i]->getType() == "Witch")
                    {
                        // Either do a close or ranged attack, or cast a spell
                        distribution = std::uniform_int_distribution<int>(0, 2);
                        switch(distribution(generator))
                        {
                            case 0: // Close attack
                            {
                                monsters[i]->closeAttack(*adventurers[adventurer]);
                                break;
                            }
                            case 1: // Ranged attack
                            {
                                monsters[i]->rangedAttack(*adventurers[adventurer]);
                                break;
                            }
                            case 2: // Cast a spell
                            {
                                // Cast a spell on an adventurer if the mage is defensive
                                if(monsters[i]->getType() == "Tree")
                                    monsters[i]->castSpell(*monsters[distribution(generator)%numOfMonsters]);
                                else
                                    monsters[i]->castSpell(*adventurers[adventurer]);
                                break;
                            }
                            default:
                            {
                                cout << monsters[i]->getName() << " got confused and hurt themselves.\nThey take ";
                                cout << hour << " damage.";
                                monsters[i]->takeDamage(hour);
                                break;
                            }
                        }
                    }
                    else if(monsters[i]->getType() == "Skeleton")
                    {
                        // Either do a close or ranged attack, or doot
                        distribution = uniform_int_distribution<int>(0, 2);
                        switch(distribution(generator))
                        {
                            case 0: // Close attack
                            {
                                monsters[i]->closeAttack(*adventurers[adventurer]);
                                break;
                            }
                            case 1: // Ranged attack
                            {
                                monsters[i]->rangedAttack(*adventurers[adventurer]);
                                break;
                            }
                            case 2: // Doot
                            {
                                monsters[i]->doot(*adventurers[adventurer]);
                                break;
                            }
                            default:
                            {
                                cout << monsters[i]->getName() << " got confused and hurt themselves.\nThey take ";
                                cout << hour << " damage.";
                                monsters[i]->takeDamage(hour);
                                break;
                            }
                        }
                    }
                    else if(monsters[i]->getType() == "Vampire")
                    {
                        // Either do a close or ranged attack, or suck blood
                        distribution = uniform_int_distribution<int>(0, 2);
                        switch(distribution(generator))
                        {
                            case 0: // Close attack
                            {
                                monsters[i]->closeAttack(*adventurers[adventurer]);
                                break;
                            }
                            case 1: // Ranged attack
                            {
                                monsters[i]->rangedAttack(*adventurers[adventurer]);
                                break;
                            }
                            case 2: // Suck blood
                            {
                                monsters[i]->stealLife(*adventurers[adventurer]);
                                break;
                            }
                            default:
                            {
                                cout << monsters[i]->getName() << " got confused and hurt themselves.\nThey take ";
                                cout << hour << " damage.";
                                monsters[i]->takeDamage(hour);
                                break;
                            }
                        }
                    }
                    else if(monsters[i]->getType() == "Werewolf")
                    {
                        // Either do a close attack or howl
                        distribution = uniform_int_distribution<int>(0, 1);
                        switch(distribution(generator))
                        {
                            case 0: // Close attack
                            {
                                if(monsters[i]->getLocation() == "far")
                                    monsters[i]->move("close");
                                else
                                    monsters[i]->closeAttack(*adventurers[adventurer]);
                                break;
                            }
                            case 1: // Howl
                            {
                                monsters[i]->howl(*adventurers[adventurer]);
                                break;
                            }
                            default:
                            {
                                cout << monsters[i]->getName() << " got confused and hurt themselves.\nThey take ";
                                cout << hour << " damage.";
                                monsters[i]->takeDamage(hour);
                                break;
                            }
                        }
                    }

                    // Kill the adventurer if its health drops below 0 and distribute the gold
                    if(adventurers[adventurer]->getHealth() <= 0)
                    {
                        int freeGold = monsters[i]->kill(*adventurers[adventurer]);

                        numOfAdventurers--;

                        if(!(freeGold <= 0) && numOfAdventurers > 0)
                        {
                            // Distribute the gold
                            if(freeGold%numOfAdventurers != 0)
                            {
                                // Randomly distribute leftover gold
                                int goldRemainder = freeGold%numOfAdventurers;
                                freeGold -= goldRemainder;
                                for(int i = 0; i < goldRemainder; i++)
                                {
                                    distribution = uniform_int_distribution<int>(0, 6);
                                    int adventurer = distribution(generator);
                                    if(adventurers[adventurer]->getHealth() <=0)
                                        i--;
                                    else
                                    {
                                        adventurers[adventurer]->addGold(1);
                                        cout << adventurers[adventurer]->getType() << " " << adventurers[adventurer]->getName();
                                        cout << " got 1 gold!" << endl;
                                    }
                                    adventurers[adventurer]->addGold(1);
                                    cout << adventurers[adventurer]->getType() << " " << adventurers[adventurer]->getName();
                                    cout << " got 1 gold!" << endl;
                                }
                                goldRemainder = 0;

                                // Give each adventurer the same amount of remaining gold
                                cout << freeGold/numOfAdventurers << " gold was given to each adventurer." << endl << endl;
                            }

                            for(int i = 0; i < 7; i++)
                            {
                                if(adventurers[i]->getHealth() > 0)
                                    adventurers[i]->addGold(freeGold/numOfAdventurers);
                            }
                        }
                    }

                    // If all the adventurers have died, go to the next loop
                    if(numOfAdventurers == 0)
                        break;
                }

                if(numOfMonsters == 0)
                    break;

                if(numOfAdventurers == 0)
                    break;
            }

            for(int i = 0; i < numMonstersThisRound; i++)
                delete monsters[i];

            delete [] monsters;
        }
        else
        {
            cout << "No monsters were encountered..." << endl;
        }
        cout << endl;

        if(numOfAdventurers == 0)
            break;
        else
        {
            // Fully heal each still alive adventurer
            for(int i = 0; i < 7; i++)
            {
                if(adventurers[i]->getHealth() > 0)
                {
                    adventurers[i]->addHealth(1000);
                    // Restore a mage's mana
                    if(adventurers[i]->getType() == "Offensive Mage" || adventurers[i]->getType() == "Defensive Mage")
                        adventurers[i]->addMana(100);
                }
            }
            cout << "The remaining adventurers were fully healed, and mages had their mana restored.\n" << endl;
        }
    }

    // Print the adventure summary
    cout << endl << "===Adventure Summary===" << endl;
    for(int i = 0; i < 7; i++)
    {
        cout << "==Adventurer " << i+1 << "==" << endl;
        cout << "Name: " << adventurers[i]->getName() << endl;
        cout << "Type: " << adventurers[i]->getType() << endl;
        cout << "Gold: " << adventurers[i]->getGold() << endl;
        if(adventurers[i]->getHealth() <= 0)
        {
            cout << "Killed By: " << adventurers[i]->getKiller() << endl;
            cout << "Hour Killed: " << adventurers[i]->getHourKilled() << endl << endl;
        }
        else
            cout << "Adventurer was not killed." << endl << endl;

    }

    // Free up memory
    for(int i = 0; i < 7; i++)
    	delete adventurers[i];

    delete [] adventurers;

    delete [] names;

    return 0;
}
