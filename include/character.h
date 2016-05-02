#pragma once
#include <game_map.h>
#include <vector>
#include <iostream>
#include <fstream>
#define HP_KNIGHT 10000
#define HP_PRINCESS 5000
#define HP_ZOMBIE 1400
#define HP_DRAGON 2800
#define HP_OBJECT 1
#define HP_WITCH 700
#define DMG_KNIGHT 700
#define DMG_PRINCESS 10
#define DMG_ZOMBIE 450
#define DMG_DRAGON 800
#define DMG_MEDKIT -300
#define DMG_FIREBALL 500

#define ZOMBIE_COUNT 70
#define DRAGONS_COUNT 1

class Character;
class Princess;
class Knight;
class Monster;
class Object;
class FireBall;

typedef Character* PCharacter;

void spawn_all();

class Character{
public:
	int hitPoint();
	int getDamage();
	void makeDamage(int dmg);
	char getChr();
	virtual void colide(Character* chr, const Point& new_point)=0; 
	virtual void colide(Knight* chr, const Point& new_point)=0;
	virtual void colide(Princess* chr, const Point& new_point)=0;
	virtual void colide(Monster* chr, const Point& new_point)=0;
	virtual void colide(Object* chr, const Point& new_point)=0;
	Point coord();
	virtual ~Character();
	virtual void move()=0;
	friend void spawn_all();
protected:
	void spawn(PCharacter chr, const Point& pos);
	PGameMap myMap;
	int hp;
	int damage;
	int max_hp;
	char symbol;
	Point position;
};

extern std::vector<PCharacter> monsters;
extern PCharacter player;
extern PCharacter princess;
extern std::ofstream logi;

class Princess: public Character{
public:
	Princess(PGameMap m);
	void colide(Character* chr, const Point& new_point);
	void colide(Knight* chr, const Point& new_point){};
	void colide(Princess* chr, const Point& new_point){};
	void colide(Monster* chr, const Point& new_point){};
	void colide(Object* chr, const Point& new_point){};
	void move();
};

class Knight: public Character{
public:
	Knight(PGameMap m);
	void colide(Character* chr, const Point& new_point);
	void colide(Knight* chr, const Point& new_point){};
	void colide(Princess* chr, const Point& new_point);
	void colide(Monster* chr, const Point& new_point);
	void colide(Object* chr, const Point& new_point);
	void move();
};

class Monster: public Character{
public:
	void colide(Character* chr, const Point& new_point);
	void colide(Knight* chr, const Point& new_point);
	void colide(Princess* chr, const Point& new_point){};
	void colide(Monster* chr, const Point& new_point){};
	void colide(Object* chr, const Point& new_point);
	virtual void move();
};

class Object: public Character{
public:
	virtual void move(){};
	void colide(Character* chr, const Point& new_point);
	void colide(Knight* chr, const Point& new_point);
	void colide(Princess* chr, const Point& new_point);
	void colide(Monster* chr, const Point& new_point);
	void colide(Object* chr, const Point& new_point);
};

template<class T, char C = '+'>
class Spawner: public Character{
public:
	Spawner(PGameMap m){
		myMap = m;
		position = m->spawnerCoord(C);
		damage = 0;
		hp = HP_OBJECT;
		symbol = C;
		max_hp = HP_OBJECT;
		step = 0;	
	};

	void move(){
		step++;
		if ((step%5)){
			return;
		}
		Point dirs[] = {UP_POINT, RIGHT_POINT, DOWN_POINT, LEFT_POINT};
		int j = rand() % 4;
		for (int i = 0; i < 4; i++){
			char c = myMap->getCell(position + dirs[(j + i)%4]);
			if (c == CHR_NOTHING){
				logi << "spawn" << std::endl;
				spawn(new T(myMap, position + dirs[(j + i)%4]), position + dirs[(j + i)%4]);
				break;
			}
		}
	};

	void colide(Character* chr, const Point& new_point){};
	void colide(Knight* chr, const Point& new_point){};
	void colide(Princess* chr, const Point& new_point){};
	void colide(Monster* chr, const Point& new_point){};
	void colide(Object* chr, const Point& new_point){};
private:
	int step;
};

class FireBall: public Object{
public:
	FireBall(PGameMap m, Point dir);
	void move();
private:
	Point direction;
};

class Medkit: public Object{
public:
	Medkit(PGameMap m);
};

class Zombie: public Monster{
public:
	Zombie(PGameMap m, Point p = Point(-1, -1));
};

class Dragon: public Monster{
public:
	Dragon(PGameMap m, Point p = Point(-1, -1));
};

class Witch: public Monster{
public:
	Witch(PGameMap m);
	void move();
};

