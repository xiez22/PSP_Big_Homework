#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <cassert>
#include "utils.h"
#include "solve.h"
using namespace std;

#define SHOW_INFO

Data_Reader::Data_Reader(const string& file_name, int mode) :mode(mode) {
#ifdef SHOW_INFO
	cout << "开始读取数据..." << endl;
#endif // SHOW_INFO

	ifstream file_in(file_name, ios::in);
	string str;
	while (getline(file_in, str))
		raw_data.emplace_back(splitString(str, ","));
	file_in.close();

#ifdef SHOW_INFO
	cout << "共读取了" << raw_data.size() << "条数据" << endl;
#endif // SHOW_INFO
}

void Data_Reader::read(const string& file_name, int md) {
#ifdef SHOW_INFO
	cout << "开始读取数据..." << endl;
#endif // SHOW_INFO
	mode = md;
	ifstream file_in(file_name, ios::in);
	string str;
	while (getline(file_in, str))
		raw_data.emplace_back(splitString(str, ","));
	file_in.close();
#ifdef SHOW_INFO
	cout << "开始读取数据..." << endl;
#endif // SHOW_INFO
}

void Data_Reader::process_data() {
#ifdef SHOW_INFO
	cout << "开始处理脏数据..." << endl;
#endif // SHOW_INFO
	for (auto p : raw_data) {
		//Check dirty data. If it is dirty data, just ignore it.
		//Because we are not able to fix the dirty data.
		if (!check_raw_data(p))
			continue;
		Bike_Data bike_data;
		bike_data.dateday = p[1];
		bike_data.day = stringToDate(p[1])[2];
		bike_data.season = static_cast<Season>(stoi(p[2]) - 1);
		bike_data.year = stoi(p[3]);
		bike_data.month = stoi(p[4]);
		if (mode == 1) {
			bike_data.holiday = (bool)stoi(p[5]);
			bike_data.weekday = stoi(p[6]);
			bike_data.workingday = (bool)stoi(p[7]);
			bike_data.weather = static_cast<Weather>(stoi(p[8]) - 1);
			bike_data.temperature = stod(p[10]);
			bike_data.hum = stod(p[11]);
			bike_data.windspeed = stod(p[12]);
			bike_data.casual = stoi(p[13]);
			bike_data.registered = stoi(p[14]);
			bike_data.total_cnt = stoi(p[15]);
		}
		else {
			bike_data.hour = stoi(p[5]);
			bike_data.holiday = (bool)stoi(p[6]);
			bike_data.weekday = stoi(p[7]);
			bike_data.workingday = (bool)stoi(p[8]);
			bike_data.weather = static_cast<Weather>(stoi(p[9]) - 1);
			bike_data.temperature = stod(p[11]);
			bike_data.hum = stod(p[12]);
			bike_data.windspeed = stod(p[13]);
			bike_data.casual = stoi(p[14]);
			bike_data.registered = stoi(p[15]);
			bike_data.total_cnt = stoi(p[16]);
		}
		if (check_processed_data(bike_data))
			processed_data.emplace_back(bike_data);
	}
#ifdef SHOW_INFO
	cout << "脏数据处理与数据格式转换完成，共" << processed_data.size() << "条有效数据" << endl;
#endif // SHOW_INFO
}

//Find dirty data
bool Data_Reader::check_raw_data(const vector<string>& data_item) {
	try {
		if (!isNum(data_item[0]))
			return false;
		//Check valid date.
		stringToDate(data_item[1]);
		for (size_t i = 2; i < data_item.size(); ++i)
			if(!isNum(data_item[i]))
				return false;
	}
	catch (...) {
		return false;
	}
	return true;
}

//Find dirty data in processed data.
bool Data_Reader::check_processed_data(const Bike_Data& bike_data) {
	if (bike_data.hum < 0.1 || bike_data.hum>1.0 ||
		bike_data.registered + bike_data.casual != bike_data.total_cnt ||
		bike_data.weekday < 0 || bike_data.weekday>6||
		bike_data.windspeed < 0 || bike_data.windspeed>1||
		bike_data.temperature < 0 || bike_data.temperature>1) {
		return false;
	}
	return true;
}
