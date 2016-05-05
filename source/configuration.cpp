#include "configuration.h"
#include <fstream>
#include <set>
#include <iostream>

using namespace std;

// bool fileExist(const char name[]){
// 	return access(name, 0) != -1;
// }

// bool isWord(string s){
// 
// }

bool isNumber(string s){
	for (int i = 0; i < (int)s.length(); i++){
		if (s[i] < '0' || s[i] > '9'){
			if ( i == 0 && s[i] == '-'){
				continue;
			}
			return false;
		}
	}
	return true;
}

Configuration::Configuration(){
	if (!read("stats.cfg")){
		damage["KNIGHT"] = 700;
		damage["ZOMBIE"] = 450;
		damage["DRAGON"] = 800;
		damage["MEDKIT"] = 300;
		damage["FIREBALL"] = 500;
		max_hp["KNIGHT"] = 10000;
		max_hp["ZOMBIE"] = 1400;
		max_hp["DRAGON"] = 2800;
		max_hp["WITCH"] = 700;
	}
	damage["PRINCESS"] = 10;
	max_hp["OBJECT"] = 1;
	max_hp["PRINCESS"] = 5000;
}

bool Configuration::read(const char name[]){
	input.open(name);
	if (!input.is_open()){
		error("no file stats.cfg");
		return false;
	}
	getclass_name();
	// string s;
	// while (!input.eof()){
	// 	input >> s;
	// 	if (!atoi(s.c_str())){
	// 		string temp;
	// 		input >> temp;
	// 		damage[s] = atoi(temp.c_str());
	// 		input >> temp;
	// 		max_hp[s] = atoi(temp.c_str());
	// 	}
	// }
	return true;
}

int Configuration::getDamage(string s){
	return damage[s];
}

int Configuration::getMaxHP(string s){
	return max_hp[s];
}

void Configuration::test(){
	for (map<string, int>::iterator i = damage.begin(); i != damage.end(); i++){
		cout << (*i).first << " " << (*i).second << endl;
	}
	for (map<string, int>::iterator i = max_hp.begin(); i != max_hp.end(); i++){
		cout << (*i).first << " " << (*i).second << endl;
	}
}

void Configuration::getclass_name(){
	string temp;
	input >> temp;
	if (isNumber(temp)){
		error("Wrong format of class name " + temp);
	}
	begin(temp);
}

void Configuration::begin(string classname){
	string temp;
	input >> temp;
	if (temp != "begin"){
		error("expected begin, but found " + temp);
	}
	get_statename(classname);
}

void Configuration::end(){
	if (input.eof()){
		finish();
		return;
	}
	getclass_name();
}

void Configuration::error(const string error_text){
	// test();
	throw error_text;
}

void Configuration::get_statename(string classname){
	string temp;
	input >> temp;
	if (temp == "end"){
		end();
		return;
	}
	assigment(classname, temp);
}

void Configuration::assigment(string classname, string stats_name){
	string temp;
	input >> temp;
	if (temp != "="){
		error("expected '=' but '" + temp + "' found");
	}
	if (stats_name == "damage"){
		damage[classname] = get_value();
		get_statename(classname);
		return;
	}
	if (stats_name == "max_hp"){
		max_hp[classname] = get_value();
		get_statename(classname);
		return;
	}	
	error("unknown stats name for " + classname);

}

int Configuration::get_value(){
	string temp;
	input >> temp;
	if (isNumber(temp)){
		return atoi(temp.c_str());
	}
	else{
		error("wrong value '" + temp + "'");
		return -1;
	}
}

void Configuration::finish(){
	set<string> characters = {"KNIGHT", "PRINCESS", "ZOMBIE", "DRAGON", "OBJECT", "FIREBALL", "MEDKIT", "WITCH"};
	for (set<string>::iterator i = characters.begin(); i != characters.end(); i++){
		if (damage.find(*i) == damage.end() || max_hp.find(*i) == max_hp.end()){
			error("no full information about " + (*i));
			return;
		}
	}
}
