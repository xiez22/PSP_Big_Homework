#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <cassert>
using namespace std;

inline vector<string> splitString(string str, string s) {
	vector<string> ans;
	for (auto p = str.find(s); p != str.npos; p = str.find(s)) {
		ans.emplace_back(str.substr(0, p));
		str = str.substr(p + s.length());
	}
	ans.emplace_back(str);
	return ans;
}

inline bool isNum(string str) {
	bool hasPoints = false;
	auto length = str.length();

	for (size_t i = 0; i < length; ++i) {
		if (str[i] == '.') {
			if (hasPoints || i == 0)
				return false;
			hasPoints = true;
		}
		else if (str[i] == '-' && i != 0) {
			return false;
		}
		else if (str[i] < '0' || str[i] > '9') {
			return false;
		}
	}
	return true;
}

inline vector<int> stringToDate(string str) {
	auto ls = splitString(str, "-");
	if(!(ls.size() == 3 && isNum(ls[0]) && isNum(ls[1]) && isNum(ls[2])))
		throw "ERROR!";
	return vector<int>{stoi(ls[0]), stoi(ls[1]), stoi(ls[2])};
}
