#pragma once
#include <game_map.h>
#include <vector>
#define HP_KNIGHT 2000
#define HP_PRINCESS 1000
#define HP_ZOMBIE 250
#define HP_DRAGON 600
#define DMG_KNIGHT 100
#define DMG_PRINCESS 0
#define DMG_ZOMBIE 150
#define DMG_DRAGON 300

#define ZOMBIE_COUNT 7

class Character{
public:
	int hitPoint();
	int getDamage();
	void makeDamage(int dmg);
	point coord();
	virtual void move() = 0;
protected:
	PGameMap myMap;
	int hp;
	int damage;
	int x;
	int y;
};

typedef Character* PCharacter;

extern std::vector<PCharacter> monsters;
extern PCharacter player;
extern PCharacter princess;

class Princess: public Character{
public:
	Princess(PGameMap m);
	void move();
};

class Knight: public Character{
public:
	Knight(PGameMap m);
	void move();
};

class Monster: public Character{
public:
	void move();
};

class Zombie: public Monster{
public:
	Zombie(PGameMap m);
};

class Dragon: public Monster{
public:
	Dragon(PGameMap m);
};

