#pragma once
#include <game_map.h>
#include <vector>
#include <iostream>
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

class CHaracter;
class Princess;
class Knight;
class Monster;

class Character{
public:
	int hitPoint();
	int getDamage();
	void makeDamage(int dmg);
	virtual void colide(Character* chr, const Point& new_point)=0; 
	virtual void colide(Knight* chr, const Point& new_point)=0;
	virtual void colide(Princess* chr, const Point& new_point)=0;
	virtual void colide(Monster* chr, const Point& new_point)=0;
	Point coord();
	virtual ~Character();
	virtual void move() = 0;
protected:
	PGameMap myMap;
	int hp;
	int damage;
	Point position;
};

typedef Character* PCharacter;

extern std::vector<PCharacter> monsters;
extern PCharacter player;
extern PCharacter princess;

class Princess: public Character{
public:
	Princess(PGameMap m);
	void colide(Character* chr, const Point& new_point);
	void colide(Knight* chr, const Point& new_point){};
	void colide(Princess* chr, const Point& new_point){};
	void colide(Monster* chr, const Point& new_point){};
	void move();
};

class Knight: public Character{
public:
	Knight(PGameMap m);
	void colide(Character* chr, const Point& new_point);
	void colide(Knight* chr, const Point& new_point){};
	void colide(Princess* chr, const Point& new_point);
	void colide(Monster* chr, const Point& new_point);
	void move();
};

class Monster: public Character{
public:
	void colide(Character* chr, const Point& new_point);
	void colide(Knight* chr, const Point& new_point);
	void colide(Princess* chr, const Point& new_point){std::cout<<"*";};
	void colide(Monster* chr, const Point& new_point){std::cout<<"*";};
	void move();
};

// class Object: public Character{
// public:
// 	Object(PGameMap m);
// };

// class Medkit: public Character{
// public:
// 	Object(PGameMap m);
// };

class Zombie: public Monster{
public:
	Zombie(PGameMap m);
};

class Dragon: public Monster{
public:
	Dragon(PGameMap m);
};

