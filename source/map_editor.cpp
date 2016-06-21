#include <ncurses.h>
#include <genmap.h>
#include <stdlib.h>
#include <ctime>
#include <iostream>
#include <character.h>
#include <string>
#include <set>

using namespace std;

int key;
vector<PCharacter> monsters;
PCharacter player;
PCharacter princess;
ofstream logi("log.txt");
Configuration* conf;

void print_legend(){
    clear();
    move(0, 2);
    printw("Legend:"); move(1, 2);
    printw("move cursor: 'w', 'd', 's', 'a'"); move(2, 2);
    printw("Set Knight: 'K'"); move(3, 2);
    printw("Set Princess: 'P'"); move(4, 2);
    printw("Set Zombie: 'Z'"); move(5, 2);
    printw("Set Dragon: 'D'"); move(6, 2);
    printw("Set Witch: 'W'"); move(7, 2);
    printw("Set Medkit: 'M'"); move(8, 2);
    printw("Set Wall: '#'"); move(9, 2);
    printw("Set dragon's lair: '&'"); move(10, 2);
    printw("Set zombie's lair: '+'"); move(11, 2);
    printw("Save map: F2"); move(12, 2);
    printw("Legend: F3"); move(13, 2);
    printw("Exit: F4"); move(14, 2);
    printw("Start Game: F5"); move(15, 2);
    printw("Save Map: 'p'"); move(16, 2);
    printw("Open Map: 'o'"); move(17, 2);
    printw("Clear: Space"); move(18, 2);
    printw("Press any key for continue");
    getch();
}

void render(char** map, int& max_x, int& max_y, const Point& pos){
    clear();
    for (int x = 0; x < max_x; x++){
        for (int y = 0; y < max_y; y++){
            move(y, x);
            addch(map[y][x]);
        }
    }
    move(max_y, 0);
    printw("Show legend F3\n");
    printw("%d", key);
    move(pos.y, pos.x);
}

void change_char(char** map, const Point& pos, int& max_x, int& max_y ,char c){
    int x = pos.x;
    int y = pos.y;
    if (y < 0 || y >  max_y - 1 || x < 0 || x > max_x - 1 ){
        return;
    }
    map[y][x] = c;
    render(map, max_x, max_y, pos);
}

void resize_map(char** map, int& max_x, int& max_y, const Point& dir){
    if (max_x == 198 || max_y == 198){
        return;
    }
    max_x += dir.x;
    max_y += dir.y;
    if (dir.x == -1){
        for (int y = 0; y < max_y; y++){
            map[y][max_x] = 0;
        }
    }
    for (int x = 0; x < max_x; x++){
        for (int y = 0; y < max_y; y++){
            if (y == 0 || y == max_y - 1 || x == 0 || x == max_x - 1 ){
                map[y][x] = CHR_WALL;
            }
        }
    }
}

void change_cursor_pos(Point& pos, const Point& dir, int& max_x, int& max_y){
    Point temp = pos + dir;
    int& x = temp.x;
    int& y = temp.y;
    if (y <= 0 || y >=  max_y - 1 || x <= 0 || x >= max_x - 1 ){
        return;
    }
    pos = temp;
    move(pos.y, pos.x);
}

string get_file(){
    char file_name[200];
    file_name[0] = 0;
    int i = 0;
    clear();move(0, 0);
    printw("Print file name:\n");
    move(1, 0);
    int k = getch();
    while (k != 10){
        if (k==46 || (k>47 && k<58) || (k>64 && k<91) || (k>96 && k<123)){
            addch(k);
            file_name[i] = k;
            file_name[i+1] = 0;
            i++;
        }
        if (k == 127){
            file_name[i] = 0;
            i--;
            move(1, i);
            addch(' ');
            move(1, i);

        }
        k = getch();
    }
    return string(file_name);
}

void save_map(char** map, int& max_x, int& max_y){
    ofstream fout(get_file());
    fout << "SAVE_PRINCESS" << endl;
    fout << max_x << " " << max_y << endl;
    for (int i = 0; i < max_y; i++){
        fout << map[i] << endl;
    }
    fout.close();
}

void open_map(char** map, int& max_x, int& max_y){
    ifstream fin(get_file());
    char buf[200];
    fin >> buf;
    try{
        if (string(buf) != "SAVE_PRINCESS"){
            throw 1;
        }
    }
    catch(...){
        clear();
        move(0, 0);
        printw("Wrang format of file\n");
        getch();
        return;
    }
    fin >> max_x >> max_y;
    fin.getline(buf, 200);

    for (int y = 0; y < max_y; y++){
        fin.getline(buf, 200);
        for (int x = 0; x < max_x; x++){
            map[y][x] = buf[x];
        }
        map[y][max_x] = 0;
    }
    fin.close();
}

void try_start_game(char** map, int max_x, int max_y){
    clear();
    int princess_count = 0;
    int knight_count = 0;
    for (int x = 0; x < max_x; x++){
        for (int y = 0; y < max_y; y++){
            princess_count += ((map[y][x] == CHR_PRINCESS) ? 1 : 0);
            knight_count += ((map[y][x] == CHR_KNIGHT) ? 1 : 0);
        }
    }
    if (princess_count * knight_count == 1){
        char** a;
        get_array(a, max_y, max_x);
        for (int y = 0; y < max_y; y++){
            for (int x = 0; x < max_x; x++){
                a[y][x] = map[y][x];
            }
            a[y][max_x] = 0;
        }
        start_game(max_y, max_x, a);
        delete_array(a);
    }
    else{
        move(0, 0);
        printw("Need only 1 princess and 1 knight, but found:\n"); move(1, 0);
        printw("%d princess\n", princess_count); move(2, 0);
        printw("%d knight\n", knight_count);
        getch();
    }
}

void clear(char** map, int& max_x, int& max_y){
    for (int y = 0; y < max_y; y++){
        for (int x = 0; x < max_x; x++){
            map[y][x] = CHR_NOTHING;
            if (y == 0 || y == max_y - 1 || x == 0 || x == max_x - 1 ){
                map[y][x] = CHR_WALL;
            }
        }
        map[y][max_x] = 0;
    }
}

void get_char(Point& pos, char** map, int& max_x, int& max_y, bool& finish){
    key = getch();
    switch (key){
    case 'a':
        change_cursor_pos(pos, LEFT_POINT, max_x, max_y);
        break;
    case 'w':
        change_cursor_pos(pos, UP_POINT, max_x, max_y);
        break;
    case 'd':
        change_cursor_pos(pos, RIGHT_POINT, max_x, max_y);
        break;
    case 's':
        change_cursor_pos(pos, DOWN_POINT, max_x, max_y);
        break;
    case 81:
        save_map(map, max_x, max_y);
        break;
    case 82:
        print_legend();
        render(map, max_x, max_y, pos);
        break;
    case 83:
        finish = true;
        break;
    case 126:
        clear();
        try_start_game(map, max_x, max_y);
        render(map, max_x, max_y, pos);
        break;
    case 56:
        resize_map(map, max_x, max_y, UP_POINT);
        break;
    case 50:
        resize_map(map, max_x, max_y, DOWN_POINT);
        break;
    case 54:
        resize_map(map, max_x, max_y, RIGHT_POINT);
        break;
    case 52:
        resize_map(map, max_x, max_y, LEFT_POINT);
        break;
    case 99:
        clear(map, max_x, max_y);
        break;
    case 111:
        open_map(map, max_x, max_y);
        break;
    case 112:
        save_map(map, max_x, max_y);
        break;
    }
    set<char> change = {CHR_KNIGHT, CHR_PRINCESS, CHR_ZOMBIE, CHR_DRAGON, CHR_MEDKIT, CHR_WITCH, CHR_NOTHING, CHR_WALL, '+', '&'};
    if (change.find(key) != change.end()){
        change_char(map, pos, max_x, max_y, (char)key);
    }
    render(map, max_x, max_y, pos);
}

int main(){
    try{
        conf = new Configuration();
    }
    catch(string err){
        cout << err << endl;
        // delete conf;
        return -1;
    }

    cout << "Set map size:" << endl;
    int max_x, max_y;
    cin >> max_x >> max_y;
    char** map;
    Point cursor_pos = Point(3, 3);
    bool finish;

    // move(3, 3);
    get_array(map, 200, 200);
    clear(map, max_x, max_y);

    srand(time(0));
    initscr();
	noecho();

	start_color();
    render(map, max_x, max_y, cursor_pos);
    while (!finish){
        get_char(cursor_pos, map, max_x, max_y, finish);
        move(cursor_pos.y, cursor_pos.x);
    }
    endwin();

    // save_map(map, max_x, max_y);
    delete_array(map);
    return 0;
}
