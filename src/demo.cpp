#include <iostream>
#include "solve.h"
#include "fltk/Graph.h"
#include "fltk/Simple_window.h"
#include "graph_utils.h"
using namespace std;

#define SHOW_INFO

int main() {
#ifdef SHOW_INFO
	cout << "欢迎使用！正在进行数据读取，请等待..." << endl;
	cout << endl << "读取每日数据" << endl;
#endif // SHOW_INFO
	// Read Data
	Data_Reader day_data("day.csv", 1);
	day_data.process_data();
#ifdef SHOW_INFO
	cout << "每日数据读取完成！" << endl;
	cout << endl << "读取每小时数据" << endl;
#endif // SHOW_INFO
	Data_Reader hour_data("hour.csv", 2);
	hour_data.process_data();
#ifdef SHOW_INFO
	cout << "每小时数据读取完成！" << endl;
	cout << endl << "请在一下菜单中选择需要进行的操作:\n____________________________________________" << endl;
#endif // SHOW_INFO

	while (true) {
		cout << endl << "1、可视化。\n2、数据预测。\n3、退出。\n请输入您的选择序号:";
		int input;
		cin >> input;

		if (input == 1) {
			cout << endl << "1、每日总使用量折线图。\n2、每日各类别使用量折线图。\n3、用户类别比例饼状图。\n4、用户类别比例变化比例图。\n5、每周各日用量占比比例图。\n6、每天各时段平均使用量折线图。\n7、不同温度使用量折线图。\n8、不同湿度使用量折线图。\n9、不同天气条件使用量条形图。\n10、工作日与非工作日情况条形图。\n请输入您的选择：";
			cin >> input;
			if (input == 1) {
				Show_Data show_data(day_data);
				show_data.show_year_graph1();
			}
			else if (input == 2) {
				Show_Data show_data(day_data);
				show_data.show_year_graph2();
			}
			else if (input == 3) {
				Show_Data show_data(day_data);
				show_data.show_pie_chart1();
			}
			else if (input == 4) {
				Show_Data show_data(day_data);
				show_data.show_proportion_chart1();
			}
			else if (input == 5) {
				Show_Data show_data(day_data);
				show_data.show_proportion_chart2();
			}
			else if (input == 6) {
				Show_Data show_data(hour_data);
				show_data.show_day_graph1();
			}
			else if (input == 7) {
				Show_Data show_data(day_data);
				show_data.show_temp_chart1();
			}
			else if (input == 8) {
				Show_Data show_data(day_data);
				show_data.show_hum_chart1();
			}
			else if (input == 9) {
				Show_Data show_data(day_data);
				show_data.show_bar_chart1();
			}
			else if (input == 10) {
				Show_Data show_data(day_data);
				show_data.show_bar_chart2();
			}
		}
		else if (input == 2) {
			cout << endl << "请注意，由于训练使用神经网络，所以可能运行时间较长（2分钟左右）\n请您仔细斟酌后选择\n1、随机区分测试集与训练集，预测每日用户量\n2、前七个季度作为训练集，后一个季度作为测试集，预测每日用户量\n3、随机区分测试集与训练集，预测每小时用户量\n请输入您的选择：";
			cin >> input;
			if (input == 1) {
				Data_Analyzer analyzer(day_data, 1);
				cout << "开始使用最小二乘法训练..." << endl;
				analyzer.linear_regression();
				auto loss1 = analyzer.check_accuracy();
				cout << "训练完成。\n\n使用神经网络训练..." << endl;
				analyzer.nerual_network();
				auto loss2 = analyzer.check_accuracy();
				cout << endl << "训练完成，最小二乘法的误差率为" << loss1 << "，神经网络的误差率为" << loss2 << endl;
			}
			else if (input == 2) {
				Data_Analyzer analyzer(day_data, 3);
				cout << "开始使用最小二乘法训练..." << endl;
				analyzer.linear_regression();
				auto loss1 = analyzer.check_accuracy();
				cout << "训练完成。\n\n使用神经网络训练..." << endl;
				analyzer.nerual_network();
				auto loss2 = analyzer.check_accuracy();
				cout << endl << "训练完成，最小二乘法的误差率为" << loss1 << "，神经网络的误差率为" << loss2 << endl;
			}
			else {
				Data_Analyzer analyzer(hour_data, 2);
				cout << "开始使用最小二乘法训练..." << endl;
				analyzer.linear_regression();
				auto loss1 = analyzer.check_accuracy();
				cout << "训练完成。\n\n使用神经网络训练..." << endl;
				analyzer.nerual_network();
				auto loss2 = analyzer.check_accuracy();
				cout << endl << "训练完成，最小二乘法的误差率为" << loss1 << "，神经网络的误差率为" << loss2 << endl;
			}
		}
		else {
			break;
		}
	}

	return 0;
}
