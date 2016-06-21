#include <character.h>
#include <fstream>
#include <iostream>

#define UP 'w'
#define DOWN 's'
#define LEFT 'a'
#define RIGHT 'd'

using namespace std;

extern vector<PCharacter> monsters;
extern PCharacter player;
extern PCharacter princess;
extern ofstream logi;
vector<PCharacter> new_monsters;

//Процедуры
PCharacter colide_all(PCharacter carrent_character, const Point& new_point){
	// vector <vector<PCharacter>::iterator> cemetery;
	// for (vector<PCharacter>::iterator i = monsters.begin(); i != monsters.end(); i++){
	// 	if ((*i)->hitPoint() <= 0){
	// 		cemetery.push_back(i);
	// 	}
	// }
	// for (int i = 0; i < cemetery.size(); i++){
	// 	monsters.erase(cemetery[i]);
	// 	delete *cemetery[i];
	// }
	for (vector<PCharacter>::iterator i = monsters.begin(); i != monsters.end(); i++){
		if ((*i)->coord() == new_point){
			(*i)->colide(carrent_character, new_point);
			return *i;
		}
	}
	return NULL;
}

void spawn_all(){
	for (unsigned int i = 0; i < new_monsters.size(); i++){
		monsters.push_back(new_monsters[i]);
		Point &pos = new_monsters	[i]->position;
		new_monsters[i]->myMap->setCharacter(new_monsters[i]->getChr(), pos.x, pos.y);
	}
	new_monsters.clear();
}

void status(PCharacter ch, int max_hp, const char a[]){
	printw("%s: %g%%", a, (double)ch->hitPoint() / max_hp * 100.0);
}

void create_character(char c, int x, int y, GameMap* gm){
	// logi << c << endl;
	switch (c){
	case CHR_KNIGHT:
		player = new Knight(gm, Point(x, y));
		monsters.push_back(player);
		break;
	case CHR_PRINCESS:
		princess = new Princess(gm, Point(x, y));
		monsters.push_back(princess);
		break;
	case CHR_ZOMBIE:
		monsters.push_back(new Zombie(gm, Point(x, y)));
		break;
	case CHR_DRAGON:
		monsters.push_back(new Dragon(gm, Point(x, y)));
		break;
	case CHR_MEDKIT:
		monsters.push_back(new Medkit(gm, Point(x, y)));
		break;
	case CHR_WITCH:
		monsters.push_back(new Witch(gm, Point(x, y)));
		break;
	case '+':
		monsters.push_back(new Spawner<Zombie, '+'>(gm, Point(x, y)));
		break;
	case '&':
		monsters.push_back(new Spawner<Dragon, '&'>(gm, Point(x, y)));
		break;
	}
}

void start_game(int n, int m, char** a){
	int drag_count = max(m * n / 200, DRAGONS_COUNT);
	int witch_count = max(m * n / 200, 4);
	int zomb_count = m * n / 25;
	GameMap myMap = GameMap(n, m, a);

	init_pair(1, COLOR_RED, COLOR_WHITE);
	init_pair(2, COLOR_GREEN, COLOR_WHITE);
	init_pair(3, COLOR_YELLOW, COLOR_BLACK);
	init_pair(4, COLOR_BLUE, COLOR_BLACK);
	init_pair(5, COLOR_GREEN, COLOR_BLACK);

	if (a == NULL){
		player = new Knight(&myMap);
		princess = new Princess(&myMap);
		monsters.push_back(player);
		monsters.push_back(princess);
		monsters.push_back(new Spawner<Zombie, '+'>(&myMap));
		monsters.push_back(new Spawner<Dragon, '&'>(&myMap));
		for (int i = 0; i < drag_count; i++){
			monsters.push_back(new Dragon(&myMap));
		}
		for(int i = 0; i < zomb_count; i++){
			monsters.push_back(new Zombie(&myMap));
		}
		for(int i = 0; i < witch_count; i++){
			monsters.push_back(new Witch(&myMap));
		}
	}
	else{
		logi << "gogogogo" << endl;
		for (int x = 0; x < m; x++){
			for (int y = 0; y < n; y++){
				create_character(a[y][x], x, y, &myMap);
			}
		}
	}
	logi << monsters.size() << endl;
	bool game = false;
	long step_count = 1;
	while(!game){
		logi << "next" << endl;
		clear();
		myMap.reDraw();
		move(0, 0);
		status(player, HP_KNIGHT, "Knight");
		move(1, 0);
		status(princess, HP_PRINCESS, "Princess");
		if (player->coord() == princess->coord()){
			clear();
			move(1, 0);
			printw("YOU WIN!!!!");
			break;
		}
		for (vector<PCharacter>::iterator i = monsters.begin(); i != monsters.end(); i++){
			logi << "start move " << (*i) << endl;
			(*i)->move();
			logi << "done move" << endl;
		}
		logi << "done step" << endl;
		if (player->hitPoint() <= 0 || princess->hitPoint() <= 0){
			game = true;
			clear();
			move(1, 0);
			printw("YOU LOSE!!!!");
		}
		if (!(step_count % 5)){
			monsters.push_back(new Medkit(&myMap));
		}
		spawn_all();
		step_count++;
	}
	for (int i = 0; i < (int)monsters.size(); i++){
		delete monsters[i];
	}
	monsters.clear();
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

char Character::getChr(){
	return symbol;
}

//Наносит урон dmg данному персонажу
void Character::makeDamage(int dmg){
	hp -= dmg;
	if (hp <= 0){
		myMap->setCharacter(CHR_NOTHING, position.x, position.y);
		position = Point(-1, -1);
	}
	if (hp > max_hp){
		hp = max_hp;
	}
}

//Ввозвращает координаты персонажа
Point Character::coord(){
	return position;
}

//Деструктор
Character::~Character(){

}


void Character::spawn(PCharacter chr, const Point& pos){
	// myMap->setCharacter(chr->getChr(), pos.x, pos.y);
	chr->position = pos;
	new_monsters.push_back(chr);
}

///////////////////////////////////////////////////////////////////////////////////////////
//Princess
//Стандартный конструктор для принцессы
Princess::Princess(PGameMap m, Point p){
	myMap = m;
	if (p == Point(-1, -1)){
		position = myMap->princessCoord();
	}
	else{
		position = p;
	}
	damage = DMG_PRINCESS;
	hp = HP_PRINCESS;
	symbol = CHR_PRINCESS;
	max_hp = HP_PRINCESS;
}


void Princess::colide(Character* chr, const Point& new_point){
	chr->colide(this, new_point);
}

//Перемещение
void Princess::move(){
	return;
}

///////////////////////////////////////////////////////////////////////////////////////////
//Knight
//Стандартный конструктор
Knight::Knight(PGameMap m, Point p){
	myMap = m;
	if (p == Point(-1, -1)){
		position = myMap->playerCoord();
	}
	else{
		position = p;
	}
	hp = HP_KNIGHT;
	damage = DMG_KNIGHT;
	symbol = CHR_KNIGHT;
	max_hp = HP_KNIGHT;
}

//Столкновения
void Knight::colide(Character* chr, const Point& new_point){
	chr->colide(this, new_point);
}

void Knight::colide(Princess* chr, const Point& new_point){
	const Point& np = new_point;
	Point& p = position;
	myMap->setCharacter(CHR_NOTHING, new_point.x, new_point.y);
	myMap->moveCharacter(np.x, np.y, p.x, p.y);
}

void Knight::colide(Monster* chr, const Point& new_point){
	const Point& np = new_point;
	Point& p = position;
	chr->makeDamage(damage);
	if (chr->hitPoint() <= 0){
		myMap->moveCharacter(np.x, np.y, p.x, p.y);
		p = np;
	}
}

void Knight::colide(Object* chr, const Point& new_point){
	const Point& np = new_point;
	Point& p = position;
	chr->makeDamage(damage);
	if (chr->hitPoint() <= 0){
		myMap->moveCharacter(np.x, np.y, p.x, p.y);
		p = np;
	}
	this->makeDamage(chr->getDamage());
}


//Перемещение
void Knight::move(){
	// char comand[15];
	// cin >> comand;
	logi << "move knight" << endl;
	Point &p = position;
	Point np;
	switch (getch()){
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
		position = np;
		return;
	}
	colide_all(this, np);
	// cout << "123" << endl;
}

///////////////////////////////////////////////////////////////////////////////////////////
//Monster
//Столкновения
void Monster::colide(Character* chr, const Point& new_point){
	chr->colide(this, new_point);
}

void Monster::colide(Knight* chr, const Point& new_point){
	const Point& np = new_point;
	Point& p = position;
	chr->makeDamage(damage);
	if (chr->hitPoint() <= 0){
		myMap->moveCharacter(np.x, np.y, p.x, p.y);
		position = np;
	}
}

void Monster::colide(Object* chr, const Point& new_point){
	logi << "colide m o!" << endl;
	const Point& np = new_point;
	Point& p = position;
	chr->makeDamage(damage);
	if (chr->hitPoint() <= 0){
		myMap->moveCharacter(np.x, np.y, p.x, p.y);
		p = np;
	}
	this->makeDamage(chr->getDamage());
	logi << " finish colide m o!" << endl;
}

//перемещение
void Monster::move(){
	if (hp <= 0){
		return;
	}
	int dir = rand() % 4;
	Point &p = position;
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
		position = np;
		return;
	}
	if (c == CHR_WALL){
		this->makeDamage(2);
	}
	else{
		colide_all(this, np);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////
//Object

void Object::colide(Character* chr, const Point& new_point){
	// logi << "colide a!" << endl;
	chr->colide(this, new_point);
}

void Object::colide(Knight* chr, const Point& new_point){
	// const Point& np = new_point;
	// Point& p = position;
	this->makeDamage(chr->getDamage());
	chr->makeDamage(damage);
}

void Object::colide(Monster* chr, const Point& new_point){
	logi << "colide o m!" << endl;
	// logi << "colide m!" << endl;
	// const Point& np = new_point;
	// Point& p = position;
	this->makeDamage(chr->getDamage());
	chr->makeDamage(damage);
}


void Object::colide(Object* chr, const Point& new_point){
	// logi << "colide! s" << endl;
	// const Point& np = new_point;
	// Point& p = position;
	this->makeDamage(chr->getDamage());
	chr->makeDamage(damage);
}

void Object::colide(Princess* chr, const Point& new_point){
	// logi << "colide! s" << endl;
	// const Point& np = new_point;
	// Point& p = position;
	this->makeDamage(chr->getDamage());
	// chr->makeDamage(damage);
}

///////////////////////////////////////////////////////////////////////////////////////////
//FireBall
//Конструктор

FireBall::FireBall(PGameMap m, Point dir){
	myMap = m;
	position = Point(0, 0);
	hp = HP_OBJECT;
	max_hp = HP_OBJECT;
	damage = DMG_FIREBALL;
	direction = dir;
	if (dir == UP_POINT){
		symbol = CHR_FIREBALL_UP;
	}
	if (dir == DOWN_POINT){
		symbol = CHR_FIREBALL_DOWN;
	}
	if (dir == LEFT_POINT){
		symbol = CHR_FIREBALL_LEFT;
	}
	if (dir == RIGHT_POINT){
		symbol = CHR_FIREBALL_RIGHT;
	}
}

void FireBall::move(){
	if (hp <= 0){
		logi << "oops" << endl;
		return;
	}
	Point np = position + direction;
	const Point& p = position;
	logi << "fireball move" << endl;
	logi << np.x << " " << np.y << endl;
	cell c = myMap->getCell(np);
	if (c == CHR_NOTHING){
		logi << "nothing" << endl;
		myMap->moveCharacter(p.x, p.y, np.x, np.y);
	}
	else{
		logi << "not nothing" << endl;
		colide_all(this, np);
	}
	if (c == CHR_WALL){
		logi << "wall" << endl;
		makeDamage(10);
		return;
	}
	position = np;
	logi << "T_T" << endl;
}

///////////////////////////////////////////////////////////////////////////////////////////
//Medkit

Medkit::Medkit(PGameMap m, Point p){
	myMap = m;
	if (p == Point(-1, -1)){
		position = m->medkitCoord();
	}
	else{
		position = p;
	}
	damage = DMG_MEDKIT;
	hp = HP_OBJECT;
	symbol = CHR_MEDKIT;
	max_hp = HP_OBJECT;
}

///////////////////////////////////////////////////////////////////////////////////////////
//Zombie

Zombie::Zombie(PGameMap m, Point p){
	myMap = m;
	if (p == Point(-1, -1)){
		position = m->zombieCoord();
	}
	else{
		position = p;
	}
	damage = DMG_ZOMBIE;
	hp = HP_ZOMBIE;
	symbol = CHR_ZOMBIE;
	max_hp = HP_ZOMBIE;
}

///////////////////////////////////////////////////////////////////////////////////////////
//Dragon

Dragon::Dragon(PGameMap m, Point p){
	myMap = m;
	if (p == Point(-1, -1)){
		position = m->dragonCoord();
	}
	else{
		position = p;
	}
	damage = DMG_DRAGON;
	hp = HP_DRAGON;
	symbol = CHR_DRAGON;
	max_hp = HP_DRAGON;
}

///////////////////////////////////////////////////////////////////////////////////////////
//Witch
Witch::Witch(PGameMap m, Point p){
	myMap = m;
	if (p == Point(-1, -1)){
		position = m->witchCoord();
	}
	else{
		position = p;
	}
	hp = HP_WITCH;
	damage = DMG_FIREBALL;
	symbol = CHR_WITCH;
	max_hp = HP_WITCH;
}

static Point map_search(PGameMap m, Point cur, Point dir){
	while (m->getCell(cur) == CHR_NOTHING){
		cur = cur + dir;
	}
	if (m->getCell(cur) == CHR_KNIGHT){
		return cur;
	}
	return Point(-1, -1);
}

void Witch::move(){
	logi << "move witch" << endl;
	if (hp <= 0){
		return;
	}
	Point& p = position;
	Point a[] = {UP_POINT, DOWN_POINT, LEFT_POINT, RIGHT_POINT};
	Point cur;
	for (int i = 0; i < 4; i++){
		if (myMap->getCell(p + a[i]) == CHR_KNIGHT){
			break;
		}
		cur = map_search(myMap, position + a[i], a[i]);
		if (!(cur == Point(-1, -1))){
			PCharacter c = new FireBall(myMap, a[i]);
			logi << "spawn " << c << endl;
			spawn(c, p + a[i]);
			logi << (*(monsters.end() - 1)) << endl;
			return;
		}
	}
	Monster::move();
}
