#include <character.h>
#include <iostream>

#define UP 'u'
#define DOWN 'd'
#define LEFT 'l'
#define RIGHT 'r'

using namespace std;

extern vector<PCharacter> monsters;
extern PCharacter player;
extern PCharacter princess;

//Процедуры
PCharacter find_monster(const Point& p){
	for (vector<PCharacter>::iterator i = monsters.begin(); i != monsters.end(); i++){
		if ((*i)->coord() == p){
			return *i;
		}
	}
	return NULL;
}

///////////////////////////////////////////////////////////////////////////////////////////
//Character
//Возвращает значение HP
int Character::hitPoint(){
	return hp;
}

//Возвращает значения урона
int Character::getDamage(){
	return damage;
}

//Наносит урон dmg данному персонажу
void Character::makeDamage(int dmg){
	hp -= dmg;
	if (hp <= 0){
		myMap->setCharacter(CHR_NOTHING, x, y);
		x = -1;
		y = -1;
	}
}

//Ввозвращает координаты персонажа
Point Character::coord(){
	return Point(x, y);
}

//Деструктор
Character::~Character(){

}

///////////////////////////////////////////////////////////////////////////////////////////
//Princess
//Стандартный конструктор для принцессы
Princess::Princess(PGameMap m){
	myMap = m;
	Point p = myMap->princessCoord();
	x = p.x;
	y = p.y;
	hp = HP_PRINCESS;
}

//Перемещение
void Princess::move(){
	return;
}

///////////////////////////////////////////////////////////////////////////////////////////
//Knight
//Стандартный конструктор
Knight::Knight(PGameMap m){
	myMap = m;
	Point p = myMap->playerCoord();
	x = p.x;
	y = p.y;
	hp = HP_KNIGHT;
	damage = DMG_KNIGHT;
}

//Перемещение
void Knight::move(){
	char comand[15];
	cin >> comand;
	Point p = Point(x, y);
	Point np;
	switch (comand[0]){
		case UP:
			np = p + UP_POINT;
			break;
		case DOWN:
			np = p + DOWN_POINT;
			break;
		case RIGHT:
			np = p + RIGHT_POINT;
			break;
		case LEFT:
			np = p + LEFT_POINT;
			break;
		default:
			move();
			return;
	}
	if (myMap->getCell(np) == CHR_WALL){
		move();
		return;
	}
	if (myMap->getCell(np) == CHR_NOTHING || myMap->getCell(np) == CHR_PRINCESS){
		if (myMap->getCell(np) == CHR_PRINCESS){
			myMap->setCharacter(CHR_NOTHING, np.x, np.y);
		}
		myMap->moveCharacter(np.x, np.y, p.x, p.y);
		x = np.x;
		y = np.y;
		return;
	}
	PCharacter target = find_monster(np);
	if (target != NULL){
		target->makeDamage(damage);
		if (target->hitPoint() <= 0){
			myMap->moveCharacter(np.x, np.y, p.x, p.y);
			x = np.x;
			y = np.y;
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////
//Monster
//перемещение
void Monster::move(){
	if (hp <= 0){
		return;
	}
	int dir = rand() % 4;
	Point p = Point(x, y);
	Point np;
	Point a[4] = {UP_POINT, LEFT_POINT, RIGHT_POINT, DOWN_POINT};
	for (int i = 0; i < 4; i++){
		cell t = myMap->getCell(p + a[i]);
		if (t == CHR_KNIGHT){
			dir = -1;
			np = p + a[i];
			break;
		}
	}
	switch(dir){
		case UP_CODE:
			np = p + UP_POINT;
			break;
		case LEFT_CODE:
			np = p + LEFT_POINT;
			break;
		case DOWN_CODE:
			np = p + DOWN_POINT;
			break;
		case RIGHT_CODE:
			np = p + RIGHT_POINT;
			break;
	}
	cell c = myMap->getCell(np);
	if (c == CHR_NOTHING){
		myMap->moveCharacter(np.x, np.y, p.x, p.y);
		x = np.x;
		y = np.y;
		return;
	}
	PCharacter target = find_monster(np);
	if (target != NULL){
		target->makeDamage(damage);
		if (target->hitPoint() <= 0){
			myMap->moveCharacter(np.x, np.y, p.x, p.y);
			x = np.x;
			y = np.y;
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////
//Zombie

Zombie::Zombie(PGameMap m){
	myMap = m;
	Point p = random_point();
	while (myMap->getCell(p) != CHR_NOTHING){
		p = random_point();
	}
	x = p.x;
	y = p.y;
	myMap->setCharacter(CHR_ZOMBIE, p.x, p.y);
	damage = DMG_ZOMBIE;
	hp = HP_ZOMBIE;
}

///////////////////////////////////////////////////////////////////////////////////////////
//Dragon

Dragon::Dragon(PGameMap m){
	myMap = m;
	Point p = myMap->dragonCoord();
	x = p.x;
	y = p.y;
	damage = DMG_DRAGON;
	hp = HP_DRAGON;
}