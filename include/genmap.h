#pragma once
#define SQR(x) ((x)*(x))

#define N 50
#define MIN_L 5
#define MAX_L 8

#define LEFT_POINT Point(-1, 0)
#define RIGHT_POINT Point(1, 0)
#define UP_POINT Point(0, -1)
#define DOWN_POINT Point(0, 1)

class Point{
public:
	Point();
	Point(int ax, int ay);
	bool operator==(const Point& p);
	friend bool operator<(const Point& p1, const Point& p2);
	friend Point operator+(const Point& p1, const Point& p2);
	int x;
	int y;
};

class Room{
public:
	Room();
	Room(Point tl, Point rb);
	Room(const Room& r);
	void draw(char a[N][N+1]);
	bool point_in(Point p);
	bool crossing(Room r);
	bool check();
	Point center();
private:
	Point leftTop;
	Point rightBottom;
};

void makeRandomRooms(char myMap[N][N + 1]);