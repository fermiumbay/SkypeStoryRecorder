#include "common.h"

string common::ItoS(int value){
	string ret;
	stringstream ss;
	ss << value;
	ss >> ret;
	return ret;
}

string common::ItoS(int value, int num){
	string ret = ItoS(value);
	num -= ret.size();
	while (num > 0){
		ret = "0" + ret;
		num--;
	}
	return ret;
}

int common::StoI(string str){
	int ret;
	stringstream ss;
	ss << str;
	ss >> ret;
	return ret;
}

vector<string> common::Split(string s, char d){
	vector<string> ret;
	stringstream ss(s);
	string item;
	while (getline(ss, item, d)) {
		if (!item.empty()) {
			ret.push_back(item);
		}
	}
	return ret;
};
