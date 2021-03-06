#include <iostream>
#include <queue>
#include <vector>
#include <stdlib.h>
#include <genmap.h>

using namespace std;
////////////////////////////////////////////////////////////////////////////////////////////////////
//Point
//Прототип класса точки
Point::Point(){
	x = 0;
	y = 0;
}

Point::Point(int ax, int ay){
	x = ax;
	y = ay;
}

bool Point::operator==(const Point& p){
	return (p.x == x) && (p.y == y);
}

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

//Генерация случайной точки
Point random_point(){
	int a = rand() % (N*N);
	return Point(a / N, a % N);
}

Point random_point(int n, int m){
	int a = rand() % (n*m);
	return Point(a / n, a % n);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//Room
//Модель комнаты
//Генерация произвольной комнаты
Room::Room(){
	//Отступ 2 клеточки от границ карты
	leftTop.x = 2 + (rand() % (N - MIN_L - 2));
	leftTop.y = 2 + (rand() % (N - MIN_L - 2));
	rightBottom.x = MIN_L + (rand() % (MAX_L)) + leftTop.x;
	rightBottom.y = MIN_L + (rand() % (MAX_L)) + leftTop.y;
}

Room::Room(int n, int m){
	//Отступ 2 клеточки от границ карты
	leftTop.x = 2 + (rand() % (m - MIN_L - 2));
	leftTop.y = 2 + (rand() % (n - MIN_L - 2));
	rightBottom.x = MIN_L + (rand() % (MAX_L)) + leftTop.x;
	rightBottom.y = MIN_L + (rand() % (MAX_L)) + leftTop.y;
}

Room::Room(Point tl, Point rb){
	leftTop = tl;
	rightBottom = rb;
}

Room::Room(const Room& r){
	leftTop = r.leftTop;
	rightBottom = r.rightBottom;
}

//Отладочная функция
void print_p(Point p){
	cout << p.x << " " << p.y << endl;
}

//Отрисовка карты в массив a
void Room::draw(char** a){
	for (int x = leftTop.x + 1; x < rightBottom.x; x++){
		for(int y = leftTop.y + 1; y < rightBottom.y; y++){
			a[y][x] = CHR_NOTHING;
		}
	}
}

//Проверка: лежит ли точка в нутри комнаты(точка на границе допускается)
bool Room::point_in(Point p){
	return (p.x > leftTop.x)&&(p.x < rightBottom.x)&&(p.y > leftTop.y)&&(p.y < rightBottom.y);
}

//Проверка на пересечение комнат
bool Room::crossing(Room r){
	Point lb(leftTop.x, rightBottom.y);
	Point rt(rightBottom.x, leftTop.y);
	return r.point_in(leftTop)||r.point_in(rightBottom)||r.point_in(lb)||r.point_in(rt);
}

//Проверка можно ли уместить данную комнату на карту размерами n x m
bool Room::check(int n, int m){
	return (rightBottom.x < m - 1)&&(rightBottom.y < n - 1);
}

//Получение центральной точки комнаты
Point Room::center(){
	int mx = (rightBottom.x + leftTop.x) / 2;
	int my = (rightBottom.y + leftTop.y) / 2;
	return Point(mx, my);
}

//подсчет стоимости прохождения через клетку
static int cost(char** myMap, Point p, Point finish){
	int v = (myMap[p.y][p.x] == CHR_NOTHING) ? 1 : 1000000000;
	return v;
}

//Поиск пути из p1 в p2 с наименьшей стоимостью и заполнением его точками
static void search_way(char** myMap, int n, int m, Point p1, Point p2){
	char** ch;
	get_array(ch, n, m);

	//Заполнение массива с флагами для посещенных клеток карты нулями
	Room map(Point(), Point(m-1, n-1));
	for (int i = 0; i < n; i++){
		for (int j = 0; j < m; j++){
			ch[i][j] = 0;
		}
	}

	//4 направления и их коды для массива с посещениями
	Point dir[4] = {LEFT_POINT, UP_POINT, RIGHT_POINT, DOWN_POINT};
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
						t = t+RIGHT_POINT;
						break;
					case '<':
						t = t+LEFT_POINT;
						break;
					case 'v':
						t = t+DOWN_POINT;
						break;
					case '^':
						t = t+UP_POINT;
						break;
				}
				myMap[t.y][t.x] = CHR_NOTHING;
			}
			return;
		}
		for (int i = 0; i < 4; i++){
			Point p = next.second + dir[i];
			if (!map.point_in(p)){
				continue;
			}
			if (!ch[p.y][p.x]){
				int v = cost(myMap, p, p2);
				q.push(make_pair(v + next.first, p));
				ch[p.y][p.x] = code[i];
			}
		}

	}
}

//Генерация комнат и связывание их
void makeRandomRooms(char** myMap, int n, int m){
	int bad_try = 0;
	Room r;
	vector <Room> a;
	for (int i = 0; i < n; i++){
		for (int j = 0; j < m - 1; j++){
			myMap[i][j] = CHR_WALL;
		}
		myMap[i][m - 1] = '\0';
	}
	m--;
	////////////////////////////////////////////////////////!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	//Мы выкидываем комнату на карту и проверяем можем ли мы ее туда поместить
	//Если не получилось то пробуем заново
	//После 100 неудачных попыток забрасываем это гнилое дело
	while (bad_try < 100){
		r = Room(n, m);
		if (r.check(n, m)){
			bool fail = false;
			for (int i = 0; (unsigned int)i < a.size(); i++){
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
	for (int i = 1; (unsigned int)i < a.size(); i++){
		search_way(myMap, n, m, a[0].center(), a[i].center());
	}
}

//Выделение памяти под двумерный массив
void get_array(char**& a, int n, int m){
	a = new char*[n];
	char* b = new char[n * (m+1)];
	for (int i = 0; i < n; i++){
		a[i] = &b[i * (m + 1)];
	}
}

//Удаление двумерного массива
void delete_array(char**& a){
	delete[] a[0];
	delete[] a;
}
