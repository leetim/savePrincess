#pragma once
#include <string>
#include <map>
#include <fstream>

class Configuration{
public:
	Configuration();
	int getDamage(std::string s);
	int getMaxHP(std::string s);
	void test();
private:
	bool read(const char name[]);
	void getclass_name();
	void begin(std::string classname);
	void end();
	void error(const std::string error_text);
	void get_statename(std::string classname);
	void assigment(std::string classname, std::string stats_name);
	int get_value();
	void finish();
	std::ifstream input;
	std::map<std::string, int> damage;
	std::map<std::string, int> max_hp;
};