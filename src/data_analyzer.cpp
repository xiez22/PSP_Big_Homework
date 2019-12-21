#include "solve.h"
#include <vector>
#include <algorithm>
#include <random>
#include <tuple>
#include <cmath>
#include "nn/nn.h"

#define SHOW_INFO

void Data_Analyzer::process_data() {
	//Clear the vector.
	train_data.clear(), train_label.clear();
	check_data.clear(), check_label.clear();
	//Generate the random real number.
	std::default_random_engine e;
	std::uniform_real_distribution<> u(0, 1);
	for (auto& p:data.processed_data) {
		//Convert the data to the vector form.
		//We only use the useful data to predict the total_cnt.
		vector<double> tdata;
		if (mode == 1) {
			feature_num = 8;
			tdata = {
				(double)p.year,
				(double)p.month,
				(double)p.weekday,
				(double)p.holiday,
				(double)p.workingday,
				static_cast<double>(p.weather),
				p.temperature,
				p.hum
			};
			double label = (double)p.total_cnt;

			if (u(e) < train_proportion) {
				train_data.emplace_back(tdata);
				train_label.emplace_back(label);
			}
			else {
				check_data.emplace_back(tdata);
				check_label.emplace_back(label);
			}
		}
		else if (mode == 2) {
			feature_num = 9;
			tdata = {
				(double)p.year,
				(double)p.month,
				(double)p.hour,
				(double)p.weekday,
				(double)p.holiday,
				(double)p.workingday,
				static_cast<double>(p.weather),
				p.temperature,
				p.hum
			};
			double label = (double)p.total_cnt;

			if (u(e) < train_proportion) {
				train_data.emplace_back(tdata);
				train_label.emplace_back(label);
			}
			else {
				check_data.emplace_back(tdata);
				check_label.emplace_back(label);
			}
		}
		else if (mode == 3) {
			feature_num = 8;
			tdata = {
				(double)p.year,
				(double)p.month,
				(double)p.weekday,
				(double)p.holiday,
				(double)p.workingday,
				static_cast<double>(p.weather),
				p.temperature,
				p.hum
			};
			double label = (double)p.total_cnt;

			if (!(p.year == 1 && p.month >= 10)) {
				train_data.emplace_back(tdata);
				train_label.emplace_back(label);
			}
			else {
				check_data.emplace_back(tdata);
				check_label.emplace_back(label);
			}
		}
		else {
			feature_num = 9;
			tdata = {
				(double)p.year,
				(double)p.month,
				(double)p.hour,
				(double)p.weekday,
				(double)p.holiday,
				(double)p.workingday,
				static_cast<double>(p.weather),
				p.temperature,
				p.hum
			};
			double label = (double)p.total_cnt;

			if (u(e) < train_proportion) {
				train_data.emplace_back(tdata);
				train_label.emplace_back(label);
			}
			if (p.year == 0 && p.month == 6 && (p.day == 1 || p.day == 2)) {
				check_data.emplace_back(tdata);
				check_label.emplace_back(label);
			}
		}
	}

	//Shuffle
	std::default_random_engine e1;
	std::uniform_int_distribution<> u1(0, train_data.size() - 1);

	for (size_t i = 0; i < train_data.size(); ++i) {
		size_t j = u1(e1);
		swap(train_data[i], train_data[j]);
		swap(train_label[i], train_label[j]);
	}
}

void Data_Analyzer::linear_regression() {
	//Clear the pred vector.
	check_pred.clear();
	vector<double> weight;
	double bias = 0.0;
	//Linear regression.
#ifdef SHOW_INFO
	cout << "Start Linear Regression..." << endl;
#endif // SHOW_INFO
	tie(weight, bias) = nn::linear_regression(train_data, train_label);
#ifdef SHOW_INFO
	cout << "Start Prediction..." << endl;
#endif // SHOW_INFO
	//Predict.
	for (auto& p : check_data) {
		double pred = bias;
		for (size_t i = 0; i < feature_num; ++i)
			pred += weight[i] * p[i];
		check_pred.emplace_back(pred);
	}
#ifdef SHOW_INFO
	cout << "Finished!" << endl;
#endif // SHOW_INFO
}

void Data_Analyzer::nerual_network() {
	loss_line.clear();
	//Hyper Parameters for nn.
	auto LR = 0.005;
	size_t EPOCH, BATCH_SIZE = 100;
	if (mode == 1) {
		EPOCH = 1000;
	}
	else if (mode == 2) {
		EPOCH = 100;
	}
	else {
		EPOCH = 100;
	}
	double max_y = 10000;

	//Clear the check pred.
	check_pred.clear();

#ifdef SHOW_INFO
	cout << "Start Nerual Network..." << endl;
#endif // SHOW_INFO

	//Define the network.
	auto net = nn::Sequential();
	if (mode == 1 || mode == 3) {
		net.add_layer(nn::Linear(feature_num, 32));
		net.add_layer(nn::ReLU());
		net.add_layer(nn::Linear(32, 64));
		net.add_layer(nn::ReLU());
		net.add_layer(nn::Linear(64, 1));
	}
	else {
		net.add_layer(nn::Linear(feature_num, 16));
		net.add_layer(nn::ReLU());
		net.add_layer(nn::Linear(16, 16));
		net.add_layer(nn::ReLU());
		net.add_layer(nn::Linear(16, 32));
		net.add_layer(nn::ReLU());
		net.add_layer(nn::Linear(32, 4));
		net.add_layer(nn::ReLU());
		net.add_layer(nn::Linear(4, 1));
	}

	//Define the Var.
	nn::Var train_x(train_data), x;
	nn::Var train_y(train_label.size(), 1), check_y(check_data.size(), 1);
	//Normalize the y.
	for (size_t i = 0; i < train_data.size(); ++i)
		train_y[i][0] = train_label[i];
	for (size_t i = 0; i < check_data.size(); ++i)
		check_y[i][0] = check_label[i];
	auto y_ = net(x);
	nn::Var y;
	auto loss = ((y_ - y).abs() / y).mean();
	auto optimizer = nn::Var::Adam;

#ifdef SHOW_INFO
	cout << "Start training..." << endl;
#endif // SHOW_INFO

	for (size_t epoch = 0; epoch < EPOCH; ++epoch) {
		for (size_t step = 0; step * BATCH_SIZE < train_data.size(); ++step) {
			x.set_data(train_x.sub_var(step * BATCH_SIZE, (step + 1) * BATCH_SIZE));
			y.set_data(train_y.sub_var(step * BATCH_SIZE, (step + 1) * BATCH_SIZE));
			loss.calculate();
			loss.zero_grad();
			loss.backward();
			loss.optim(optimizer, LR);
#ifdef SHOW_INFO
			if (step % 50 == 0) {
				cout << "Epoch:" << epoch << " Step:" << step << " Loss:";
				loss.print();
			}
#endif // SHOW_INFO
		}
		loss_line.emplace_back(loss[0][0]);
	}

#ifdef SHOW_INFO
	cout << "Start prediction..." << endl;
#endif // SHOW_INFO

	//Prediction.
	x.set_data((nn::Matrix)check_data);
	y_.calculate();

	for (auto p : y_._data().data)
		check_pred.emplace_back(max(p.front(), 0));
}

double Data_Analyzer::check_accuracy() {
	double total_error = 0.0;
	for (size_t i = 0; i < check_label.size(); ++i)
		total_error += min(1.0, abs(check_label[i] - check_pred[i]) / check_label[i]);
	return total_error / (double)check_label.size();
}
