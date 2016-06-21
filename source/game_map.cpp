#include <iostream>
#include <fstream>
#include <stdio.h>
#include <game_map.h>

using namespace std;

///////////////////////////////////////////////////////////////////////////////////////////
//GameMap
//Стандартный конструктор
GameMap::GameMap(){
	get_array(m, MAP_SIZE, MAP_SIZE + 1);
	nx = MAP_SIZE;
	ny = MAP_SIZE + 1;
	makeRandomRooms(m, MAP_SIZE, MAP_SIZE + 1);
}


GameMap::GameMap(int _n, int _m, char** a){
	nx = _m;
	ny = _n;
	if (a == NULL){
		get_array(m, _n, _m + 1);
		makeRandomRooms(m, _n, _m + 1);
	}
	else{
		m = a;
	}
	is_my_pointer = (a == NULL);
}

//Конструктор из файла с картой
GameMap::GameMap(const char file[]){
	get_array(m, MAP_SIZE, MAP_SIZE + 1);
	nx = MAP_SIZE;
	ny = MAP_SIZE + 1;
	ifstream input(file);
	for (int i = 0; i < MAP_SIZE; i++){
		input >> m[i];
	}
	input.close();
}

//Устанавливает символ "с" в ячейку с координатами (x, y)
void GameMap::setCharacter(cell c, int x, int y){
	m[y][x] = c;
}

//Перемещает(точнее говоря меняет местами с) точку с координатами (x1, y1) на место (x2, y2)
void GameMap::moveCharacter(int x1, int y1, int x2, int y2){
	swap(m[y1][x1], m[y2][x2]);
}

//Выводит карту на экран(желательно предворительно его почистить)
void GameMap::reDraw(){
	//system("clear");
	for (int x = 0; x < nx; x++){
		for (int y = 0; y < ny; y++){
			move(y + 2, x);
			switch(m[y][x]){
				case CHR_KNIGHT:
					addch(m[y][x] | COLOR_PAIR(1));
					break;
				case CHR_PRINCESS:
					addch(m[y][x] | COLOR_PAIR(2));
					break;
				case CHR_DRAGON:
					addch(m[y][x] | COLOR_PAIR(3));
					break;
				case CHR_WITCH:
					addch(m[y][x] | COLOR_PAIR(4));
					break;
				case CHR_ZOMBIE:
					addch(m[y][x] | COLOR_PAIR(5));
					break;
				default:
					addch(m[y][x]);
			}
			// if (m[y][x] == CHR_KNIGHT){
			// 	addch(m[y][x] | COLOR_PAIR(1));
			// }
			// if (m[y][x] == CHR_PRINCESS){
			// 	addch(m[y][x] | COLOR_PAIR(2));
			// }
			// addch(m[y][x]);
		}
	}
	move(2, 0);
	refresh();
}

//Получение символа из ячейки
cell GameMap::getCell(const Point& p){
	return m[p.y][p.x];
}

//Получает координаты символа линейным поиском
Point GameMap::getCoord(const char& c){
	Point p = random_point(ny, nx);
	while (m[p.y][p.x] != CHR_NOTHING){
		p = random_point(ny, nx);
	}
	m[p.y][p.x] = c;
	return p;
}

//Получает координаты символа 'D' на момент написания коментария обозначающий дракона
Point GameMap::dragonCoord(){
	return getCoord(CHR_DRAGON);
}

//Получает координаты символа 'K' на момент написания коментария обозначающий рыцаря
Point GameMap::playerCoord(){
	return getCoord(CHR_KNIGHT);
}

//Получает координаты символа 'P' на момент написания коментария обозначающий принцессу
Point GameMap::princessCoord(){
	return getCoord(CHR_PRINCESS);
}

//Получает координаты символа 'Z' на момент написания коментария обозначающий зомби
Point GameMap::zombieCoord(){
	return getCoord(CHR_ZOMBIE);
}

//Получает координаты символа 'W' на момент написания коментария обозначающий вудьму
Point GameMap::witchCoord(){
	return getCoord(CHR_WITCH);
}

//Получает координаты символа 'M' на момент написания коментария обозначающий аптечку
Point GameMap::medkitCoord(){
	return getCoord(CHR_MEDKIT);
}

//Получение координат символа С для спаунера
Point GameMap::spawnerCoord(char C){
	return getCoord(C);
}

//Деструктор
GameMap::~GameMap(){
	if (is_my_pointer){
		delete_array(m);
	}
}
