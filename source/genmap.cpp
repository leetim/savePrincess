#include <iostream>
#include <queue>
#include <vector>
#include <stdlib.h>
#include <ctime>

#define SQR(x) ((x)*(x))

#define N 1000
#define MIN_L 5
#define MAX_L 8

#define POINT_LEFT Point(-1, 0)
#define POINT_RIGHT Point(1, 0)
#define POINT_TOP Point(0, -1)
#define POINT_BOTTOM Point(0, 1)

using namespace std;
////////////////////////////////////////////////////////////////////////////////////////////////////
//Point
//Прототип класса точки
class Point{
public:
	Point(){
		x = 0;
		y = 0;
	}

	Point(int ax, int ay){
		x = ax;
		y = ay;
	}

	bool operator==(const Point& p){
		return (p.x == x) && (p.y == y);
	}

	friend bool operator<(const Point& p1, const Point& p2);

	friend Point operator+(const Point& p1, const Point& p2);
	int x;
	int y;
};

Point operator+(const Point& p1, const Point& p2){
	return Point(p1.x + p2.x, p1.y + p2.y);
}

//Оператор < по сути не нужен вообще, но g++ ругается на его отцуствие
bool operator<(const Point& p1, const Point& p2){
	return false;
}

//Квадрат расстояния между точками
//Не используется
int dist2(Point p1, Point p2){
	return SQR(p1.x - p2.x) + SQR(p1.y - p2.y);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//Room
//Модель комнаты
class Room{
public:
	//Генерация произвольной комнаты
	Room(){
		//Отступ 2 клеточки от границ карты
		leftTop.x = 2 + (rand() % (N - MIN_L - 2));
		leftTop.y = 2 + (rand() % (N - MIN_L - 2));
		rightBottom.x = MIN_L + (rand() % (MAX_L)) + leftTop.x;
		rightBottom.y = MIN_L + (rand() % (MAX_L)) + leftTop.y;
	}

	Room(Point tl, Point rb){
		leftTop = tl;
		rightBottom = rb;
	}

	Room(const Room& r){
		leftTop = r.leftTop;
		rightBottom = r.rightBottom;
	}

	//Отрисовка карты в массив a
	void draw(char a[N][N+1]){
		for (int x = leftTop.x + 1; x < rightBottom.x; x++){
			for(int y = leftTop.y + 1; y < rightBottom.y; y++){
				a[y][x] = '.';
			}
		}
	}

	//Проверка: лежит ли точка в нутри комнаты(точка на границе допускается)
	bool point_in(Point p){
		return (p.x > leftTop.x)&&(p.x < rightBottom.x)&&(p.y > leftTop.y)&&(p.y < rightBottom.y);
	}

	//Проверка на пересечение комнат
	bool crossing(Room r){
		Point lb(leftTop.x, rightBottom.y);
		Point rt(rightBottom.x, leftTop.y);
		return r.point_in(leftTop)||r.point_in(rightBottom)||r.point_in(lb)||r.point_in(rt);
	}

	//Проверка можно ли уместить данную комнату на карту размерами NxN
	bool check(){
		return (rightBottom.x < N - 1)&&(rightBottom.y < N - 1);
	}

	//Получение центральной точки комнаты
	Point center(){
		int mx = (rightBottom.x + leftTop.x) / 2;
		int my = (rightBottom.y + leftTop.y) / 2;
		return Point(mx, my);
	}
private:
	Point leftTop;
	Point rightBottom;
};

//Массив для генерации карты
char myMap[N][N + 1];

//подсчет стоимости прохождения через клетку
int cost(Point p, Point finish){
	int v = (myMap[p.y][p.x] == '.') ? 1 : 1000000000;
	return v;
}

//Поиск пути из p1 в p2 с наименьшей стоимостью и заполнением его точками
void search_way(Point p1, Point p2){
	char ch[N][N];
	Point start = p1;

	//Заполнение массива с флагами для посещенных клеток карты нулями
	Room m(Point(), Point(N-1, N-1));
	for (int i = 0; i < N; i++){
		for (int j = 0; j < N; j++){
			ch[i][j] = 0;
		}
	}

	//4 направления и их коды для массива с посещениями
	Point dir[4] = {POINT_LEFT, POINT_TOP, POINT_RIGHT, POINT_BOTTOM};
	char code[4] = {'>', 'v', '<', '^'};

	//По сути говоря это обычный алгоритм Диекстры на куче
	priority_queue<pair<int, Point> > q;
	q.push(make_pair(0, p1));
	ch[p1.y][p1.x] = 'S';
	while(!q.empty()){
		pair<int, Point> next = q.top();
		q.pop();
		if (next.second == p2){
			Point t = p2;
			while (!(t == p1)){
				switch(ch[t.y][t.x]){
					case '>': 
						t = t+POINT_RIGHT;
						break;
					case '<': 
						t = t+POINT_LEFT;
						break;
					case 'v': 
						t = t+POINT_BOTTOM;
						break;
					case '^': 
						t = t+POINT_TOP;
						break;
				}
				myMap[t.y][t.x] = '.';
			}
			return;
		}
		for (int i = 0; i < 4; i++){
			Point p = next.second + dir[i];
			if (!m.point_in(p)){
				continue;
			}
			if (!ch[p.y][p.x]){
				int v = cost(p, p2);
				q.push(make_pair(v + next.first, p));
				ch[p.y][p.x] = code[i];
			}
		}

	}
}

//Генерация комнат и связывание их
void makeRandomRooms(){
	int bad_try = 0;
	Room r;
	vector <Room> a;

	//Мы выкидываем комнату на карту и проверяем можем ли мы ее туда поместить
	//Если не получилось то пробуем заново
	//После 100 неудачных попыток забрасываем это гнилое дело
	while (bad_try < 100){
		r = Room();
		if (r.check()){
			bool fail = false;
			for (int i = 0; i < a.size(); i++){
				if (r.crossing(a[i])){
					bad_try++;
					fail = true;
					break;
				}
			}
			if (fail){
				continue;
			}
		}
		else{
			bad_try++;
			continue;
		}
		a.push_back(r);
		r.draw(myMap);
		bad_try = 0;
	}

	//Поиск пути из 1-й комнаты во все остальные
	//К сожалению из-за криво написаной Дийкстры квадратичная асимптотика мало того,
	//Что замедляет работу в несколько 10-ков раз, так еще и дает такой же результат
	//Если уменьшить стоимость стен то Дийкстра начинает искать какие-то неочевидные пути
	//К примеру вдоль границ карты
	for (int i = 1; i < a.size(); i++){
		search_way(a[0].center(), a[i].center());
	}
}
