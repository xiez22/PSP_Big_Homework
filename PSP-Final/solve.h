#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <fstream>
#include "utils.h"
#include "graph_utils.h"
using namespace std;

//Enum classes.
enum class Season { spring, summer, fall, winter };
enum class Weather { clear, cloudy, light, heavy };

//A class to save the data.
class Bike_Data {
public:
	//Basic Information
	string dateday;
	int year = 0, month = 0, day = 0, hour = 0, weekday = 0;
	bool holiday = false, workingday = true;
	double temperature = 0.0, windspeed = 0.0, hum = 0.0;
	Season season;
	Weather weather;

	//User data.
	int casual = 0, registered = 0, total_cnt = 0;
};

//A class to read the raw data from the files.
class Data_Reader {
	friend class Show_Data;
	friend class Data_Analyzer;
	//A vector to storage raw_data.
	vector<vector<string>> raw_data;
	//A vector to storage transformed data.
	vector<Bike_Data> processed_data;
	//mode:day-1,hour2.	
	int mode = 1;

public:
	Data_Reader() = default;
	Data_Reader(const string& file_name, int mode = 1);

	//Read the raw data.
	void read(const string& file_name, int md = 1);
	//Remove the dirty data and storage into the bike_data vector.
	void process_data();

private:
	bool check_raw_data(const vector<string>&);
	bool check_processed_data(const Bike_Data&);
};

//A class to show the data with FLTK.
class Show_Data {
protected:
	Data_Reader data;
public:
	Show_Data() = default;
	Show_Data(const Data_Reader& data) :data(data) {}
	void set_data(const Data_Reader& dat) { 
		data = dat; 
	}

	void show_year_graph1() const;
	void show_year_graph2() const;
	void show_day_graph1() const;
	void show_pie_chart1() const;
	void show_proportion_chart1() const;
	void show_proportion_chart2() const;
	void show_bar_chart1() const;
	void show_bar_chart2() const;
	void show_temp_chart1() const;
	void show_hum_chart1() const;
	void show_loss_chart1(double, const vector<double>&) const;
	void show_loss_chart2(const vector<double>&, const vector<double>&, const vector<double>&) const;
};

//A class to predict the data with my nerual network framework.
class Data_Analyzer {
protected:
	double train_proportion = 0.9;
	size_t feature_num = 8;
	int mode = 1;
	Data_Reader data;
	void process_data();
public:
	vector<vector<double>> train_data, check_data;
	vector<double> train_label, check_label, check_pred;
	vector<double> loss_line;
	Data_Analyzer() = default;
	Data_Analyzer(const Data_Reader& data, int mode = 1) :data(data), mode(mode) {
		process_data();
	}
	void set_data(const Data_Reader& dat, int mod = 1) {
		data = dat, mode = mod;
		process_data();
	}

	void linear_regression();
	void nerual_network();
	double check_accuracy();
};
