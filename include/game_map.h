#pragma once
#include <genmap.h>
#include <ncurses.h>

#define MAP_SIZE 50
#define CHR_KNIGHT 'K'
#define CHR_PRINCESS 'P'
#define CHR_ZOMBIE 'Z'
#define CHR_DRAGON 'D'
#define CHR_FIREBALL_UP '^'
#define CHR_FIREBALL_DOWN 'v'
#define CHR_FIREBALL_LEFT '<'
#define CHR_FIREBALL_RIGHT '>'
#define CHR_MEDKIT '+'
#define CHR_WITCH 'W'

#define UP_CODE 0
#define RIGHT_CODE 1
#define LEFT_CODE 2
#define DOWN_CODE 3

typedef char cell;


class GameMap{
public:
	GameMap();
	GameMap(int n, int m);
	GameMap(const char file[]);
	void setCharacter(cell c, int x, int y);
	void moveCharacter(int x1, int y1, int x2, int y2);
	void reDraw();
	cell getCell(const Point& p);
	Point dragonCoord();
	Point playerCoord();
	Point princessCoord();
	Point zombieCoord();
	Point witchCoord();
	~GameMap();
	int ny;
	int nx;
private:
	Point getCoord(const char& c);
	cell** m;
};

typedef GameMap* PGameMap;
