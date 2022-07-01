#include "Character.h"
#include <iostream>
#include <cstring>

Character::Character(string _name, string _type, int _attack, int _defense, int _remainingHealth, int _nMaxRounds) {
	this->name = _name;
	this->type = _type;
	this->attack = _attack;
	this->defense = _defense;
	this->remainingHealth = _remainingHealth;
	this->nMaxRounds = _nMaxRounds;
	this->nRoundsSinceSpecial = 0;

}

Character::Character(const Character& character) {
	this->name = character.name;
	this->type = character.type;
	this->attack = character.attack;
	this->defense = character.defense;
	this->remainingHealth = character.remainingHealth;
	this->nMaxRounds = character.nMaxRounds;
	this->nRoundsSinceSpecial = character.nRoundsSinceSpecial;
	this->isAlive = character.isAlive;

}

Character& Character::operator=(const Character& character) {
	if(this != &character){
		delete[] healthHistory;
		this->name = character.name;
		this->type = character.type;
		this->attack = character.attack;
		this->defense = character.defense;
		this->remainingHealth = character.remainingHealth;
		this->nMaxRounds = character.nMaxRounds;
		this->nRoundsSinceSpecial = character.nRoundsSinceSpecial;
		this->isAlive = character.isAlive;

	}

	return *this;
}

bool Character::operator<(const Character& other) {        
    return name < other.name;
}

Character::~Character() {
	delete[] healthHistory;
}
