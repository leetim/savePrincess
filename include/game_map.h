#pragma once
#define MAP_SIZE 15
#define CHR_WALL '#'
#define CHR_NOTHING '.'
#define CHR_KNIGHT 'K'
#define CHR_PRINCESS 'P'
#define CHR_ZOMBIE 'Z'
#define CHR_DRAGON 'D'

#define UP_POINT point(0, -1);
#define DOWN_POINT point(0, 1);
#define RIGHT_POINT point(1, 0);
#define LEFT_POINT point(-1, 0);

#define UP_CODE 0
#define RIGHT_CODE 1
#define LEFT_CODE 2
#define DOWN_CODE 3

typedef char cell;

struct point{
	point();
	point(int x, int y);
	int x;
	int y;
};

point random_point();

class GameMap{
public:
	GameMap();
	GameMap(const char file[]);
	void setCharacter(cell c, int x, int y);
	void moveCharacter(int x1, int y1, int x2, int y2);
	void reDraw();
	cell getCell(const point& p);
	point dragonCoord();
	point playerCoord();
	point princessCoord();
private:
	point getCoord(const char& c);
	cell m[MAP_SIZE][MAP_SIZE + 1];
};

typedef GameMap* PGameMap;

point operator+(const point& f, const point& s);
bool operator==(const point& f, const point& s);
