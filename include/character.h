#pragma once
#include <game_map.h>
#include <vector>
#define HP_KNIGHT 10000
#define HP_PRINCESS 5000
#define HP_ZOMBIE 2000
#define HP_DRAGON 5000
#define DMG_KNIGHT 700
#define DMG_PRINCESS 0
#define DMG_ZOMBIE 450
#define DMG_DRAGON 800

#define ZOMBIE_COUNT 70
#define DRAGONS_COUNT 1

class Character{
public:
	int hitPoint();
	int getDamage();
	void makeDamage(int dmg);
	Point coord();
	virtual ~Character();
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

