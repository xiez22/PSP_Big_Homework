#pragma once
#include "fltk/std_lib_facilities.h"
#include "fltk/Graph.h"
#include <FL/Fl_Chart.H>
#include <map>
using namespace std;

class Default_Pie_Chart :public Graph_lib::Shape {
public:
	void draw_lines() const {
		Fl_Chart chart(100, 0, 600, 600);
		fl_color(Fl_Color(0xff000000));
		fl_line_style(FL_SOLID);
		chart.type(FL_PIE_CHART);
		chart.textsize(20);
		chart.add(1, "a", 0xff000000);
		chart.add(2, "b", 0x00ff0000);
		chart.add(3, "c(20.20%)", 0x0000ff00);
		chart.add(4, "d", 0xffff0000);
		chart.autosize(1);
		((Fl_Widget*)(&chart))->draw();
	}
};

class Pie_Chart :public Graph_lib::Shape {
protected:
	//Hyper Parameters
	double dt = 0.001;
	//Parameters
	double r = 0, cx = 0, cy = 0;
	vector<double> num_data, p_data;
	vector<string> label_data;

	void make_p_data() {
		double total_size = 0.0;
		for (auto p : num_data)
			total_size += p;
		for (auto p : num_data)
			p_data.emplace_back(p / total_size);
	}
public:
	Pie_Chart() = delete;
	Pie_Chart(double cx, double cy, double r) :cx(cx), cy(cy), r(r) {}

	void set_data(const vector<double>& num_v) {
		num_data = num_v;
		make_p_data();
	}
	void set_data(const vector<int>& num_v) {
		for (auto p : num_v)
			num_data.emplace_back((double)p);
		make_p_data();
	}

	void set_label(const vector<string>& label) {
		label_data = label;
	}

	void draw_lines() const {
		Graph_lib::Color border_color(Graph_lib::Color::invisible);
		vector<Graph_lib::Color> fill_color({ (int)0xcc333300,(int)0x33cc6600,(int)0x3399cc00,(int)0xff990000,(int)0xff66cc00,(int)0xcccc6600,(int)0x9966ff00 });
		constexpr auto PI = 3.1415926535897932;

		double t = 0.0;
		for (size_t i = 0; i < p_data.size(); ++i) {
			Graph_lib::Polygon polygon;
			polygon.add(Point(cx, cy));
			double d = 0.0;
			for (d = 0.0; d < p_data[i]; d += dt) {
				polygon.add(Point(cx + r * sin(2 * PI * (d + t))
					, cy - r * cos(2 * PI * (d + t))));
			}
			polygon.set_color(border_color);
			polygon.set_fill_color(fill_color[i % fill_color.size()]);
			polygon.draw();
			string to_show = to_string(p_data[i] * 100);
			to_show = to_show.substr(0, to_show.find('.') + 2) + "%";
			to_show = label_data[i] + "(" + to_show + ")";
			Graph_lib::Text text(Point(cx + (r + 50) * sin(2 * PI * (d / 2 + t)) - 5 * to_show.length(),
				cy - (r + 50) * cos(2 * PI * (d / 2 + t))), to_show);
			text.set_color(fill_color[i % fill_color.size()]);
			text.set_font_size(18);
			text.draw();
			t += d;
		}
	}
};

class Line_Chart :public Graph_lib::Shape {
protected:
	double cx = 0, cy = 0, lx = 0, ly = 0;
	double sx = 0, ex = 0, dx = 0;
	double sy = 0, ey = 0, dy = 0;
	vector<string> label_x, label_y;
	vector<vector<pair<double, double>>> all_data;
	vector<string> line_label_str;
	string x_label_name = "", y_label_name = "";

	int nx = 0, ny = 0;
public:
	bool show_label = false, show_line_label = false;
	Line_Chart(double cx, double cy, double lx, double ly, int nx, int ny) :
		cx(cx), cy(cy), lx(lx), ly(ly), nx(nx), ny(ny) {
		dx = lx / nx;
		label_x = vector<string>(nx, "");
		dy = ly / (ny - 0.5);
		label_y = vector<string>(ny, "");
	}

	void set_axis_name(string x, string y) {
		x_label_name = x;
		y_label_name = y;
	}

	void set_range(double start_x, double end_x, double start_y, double end_y) {
		sx = start_x, ex = end_x;
		sy = start_y, ey = end_y;
	}

	void set_label(const vector<string>& x, const vector<string>& y) {
		label_x = x, label_y = y;
	}

	void add_data(const vector<pair<double, double>>& data, string line_label = "") {
		all_data.emplace_back(data);
		line_label_str.emplace_back(line_label);
	}

	void draw_lines() const {
		vector<int> fill_color({ (int)0xcc333300,(int)0x33cc6600,(int)0x3399cc00,(int)0xff990000,(int)0xff66cc00,(int)0xcccc6600,(int)0x9966ff00 });

		fl_color(0);
		fl_line(cx, cy, cx, cy - ly);
		fl_line(cx, cy, cx + lx, cy);

		//Show axis name
		Graph_lib::Text x_name(Point(cx + lx, cy + 20), x_label_name);
		Graph_lib::Text y_name(Point(cx - 20, cy - ly - 15), y_label_name);
		x_name.set_color(0), y_name.set_color(0);
		x_name.draw(), y_name.draw();

		for (int i = 1; i <= nx; ++i) {
			fl_line(cx + dx * (i - 0.5), cy, cx + dx * (i - 0.5), cy - 5);
			Graph_lib::Text t(Point(cx + dx * (i - 0.5) - 5, cy + 20), label_x[i - 1]);
			t.set_color(0);
			t.draw();
		}
		for (int i = 0; i < ny; ++i) {
			fl_line(cx, cy - dy * i, cx + 5, cy - dy * i);
			Graph_lib::Text t(Point(cx - 40, cy - dy * i + 5), label_y[i]);
			t.set_color(0);
			t.draw();
		}

		int step = 0;
		for (auto& vdata : all_data) {
			for (int i = 0; i < vdata.size() - 1; ++i) {
				double px = (vdata[i].first - sx) / (ex - sx) * (nx - 1) * dx + dx / 2 + cx;
				double py = -(vdata[i].second - sy) / (ey - sy) * (ny - 1) * dy + cy;

				double pnx = (vdata[i + 1].first - sx) / (ex - sx) * (nx - 1) * dx + dx / 2 + cx;
				double pny = -(vdata[i + 1].second - sy) / (ey - sy) * (ny - 1) * dy + cy;

				fl_color(fill_color[step % fill_color.size()]);
				fl_line_style(0, 2);
				fl_line(px, py, pnx, pny);

				if (show_label) {
					string to_show1 = to_string(vdata[i].second);
					string to_show2 = to_string(vdata[i + 1].second);
					Graph_lib::Text t1(Point(px - 5, py - 20), to_show1.substr(0, to_show1.find('.') + 2));
					t1.set_color(0);
					t1.draw();
					Graph_lib::Text t2(Point(pnx - 5, pny - 20), to_show2.substr(0, to_show2.find('.') + 2));
					t2.set_color(0);
					t2.draw();
				}
			}
			if (show_line_label) {
				fl_color(fill_color[step % fill_color.size()]);
				fl_line_style(0, 2);
				fl_line(cx + lx - dx / 2, cy - ly + (step + 1) * 30,
					cx + lx + 50 - dx / 2, cy - ly + (step + 1) * 30);

				Graph_lib::Text text(Point(cx + lx + 70 - dx / 2, 
					cy - ly + (step + 1) * 30 + 5), line_label_str[step]);
				text.set_font_size(16);
				text.set_color(0);
				text.draw();
			}
			++step;
		}
	}
};

class Bar_Chart :public Graph_lib::Shape {
	int cx = 0, cy = 0, lx = 0, ly = 0;
	double sy = 0, ey = 0;
	vector<pair<double, string>> vdata;
	vector<string> y_label;
	string x_axis_name = "", y_axis_name = "";
public:
	Bar_Chart(int cx, int cy, int lx, int ly) :
		cx(cx), cy(cy), lx(lx), ly(ly) {}

	void set_range(double start_y, double end_y) {
		sy = start_y, ey = end_y;
	}

	void set_label(vector<string> label) {
		y_label = label;
	}

	void set_axis_name(string x, string y) {
		x_axis_name = x, y_axis_name = y;
	}

	void add_data(double val, const string& label) {
		vdata.emplace_back(make_pair(val, label));
	}

	void draw_lines() const {
		
		vector<int> fill_color({ (int)0xcc333300,(int)0x33cc6600,(int)0x3399cc00,(int)0xff990000,(int)0xff66cc00,(int)0xcccc6600,(int)0x9966ff00 });

		fl_color(0);
		fl_line(cx, cy, cx + lx, cy);
		fl_line(cx, cy, cx, cy - ly);
		//Show axis name
		Graph_lib::Text x_name(Point(cx + lx, cy + 30), x_axis_name);
		Graph_lib::Text y_name(Point(cx - 30, cy - ly - 25), y_axis_name);
		x_name.set_color(0), y_name.set_color(0);
		x_name.set_font_size(18);
		y_name.set_font_size(18);
		x_name.draw(), y_name.draw();
		
		int dx = lx / (vdata.size() + 1);

		for (int i = 0; i < vdata.size(); ++i) {
			Graph_lib::Text label_text(Point(cx + dx * (i + 1) - 20, cy + 20), vdata[i].second);
			label_text.set_color(0);
			label_text.set_font_size(18);
			label_text.draw();

			Graph_lib::Rectangle rect(Point(cx + dx * (i + 1) - 30,
				cy - (vdata[i].first - sy) / (ey - sy) * ly),
				Point(cx + dx * (i + 1) + 30, cy));
			rect.set_color(Graph_lib::Color::invisible);
			rect.set_fill_color(fill_color[i % fill_color.size()]);
			rect.draw();

			Graph_lib::Text num_text(Point(cx + dx * (i + 1) - 20,
				cy - (vdata[i].first - sy) / (ey - sy) * ly - 15), to_string(int(vdata[i].first)));
			num_text.set_color(fill_color[i % fill_color.size()]);
			num_text.set_font_size(18);
			num_text.draw();
		}

		for (int i = 0; i < y_label.size(); ++i) {
			int dy = ly / (y_label.size() - 1);
			Graph_lib::Text val_text(Point(cx - 40, cy - i * dy + 5), y_label[i]);
			val_text.set_color(0);
			val_text.draw();
			fl_color(0);
			fl_line(cx, cy - i * dy, cx + 10, cy - i * dy);
		}
	}
};

class Proportion_Chart :public Graph_lib::Shape {
public:
	double cx = 0, cy = 0, lx = 0, ly = 0;
	double sx = 0, ex = 0, dx = 0;
	double sy = 0, ey = 1, dy = 0.25;
	vector<string> label_x, label_y = { "0%", "25%","50%","75%","100%" };
	map<double, vector<double>> all_data;
	vector<string> line_label_str;
	int nx = 0, ny = 5, n = 0;
	string axis_name = "";
 public:
	bool show_label = false, show_line_label = false;
	Proportion_Chart(double cx, double cy, double lx, double ly, int nx, int n) :
		cx(cx), cy(cy), lx(lx), ly(ly), nx(nx), n(n) {
		dx = lx / (nx - 1);
		label_x = vector<string>(nx, "");
		dy = ly / 4;
	}

	void set_axis_name(string name) {
		axis_name = name;
	}

	void set_range(double start_x, double end_x) {
		sx = start_x, ex = end_x;
	}

	void set_x_label(const vector<string>& x) {
		label_x = x;
	}

	void set_label(const vector<string>& x) {
		line_label_str = x;
	}

	void add_data(double x, const vector<double>& y) {
		all_data.insert(make_pair(x, y));
	}

	void process_data() {
		//Process the data
		for (auto& p : all_data) {
			double sum = 0;
			for (auto q : p.second)
				sum += q;
			for (auto& q : p.second)
				q = q / sum;
		}
	}

	void draw_lines() const {
		vector<int> fill_color({ (int)0xcc333300,(int)0x33cc6600,(int)0x3399cc00,(int)0xff990000,(int)0xff66cc00,(int)0xcccc6600,(int)0x9966ff00 });

		map<double, double> cur_data;
		for (auto p : all_data)
			cur_data.insert(make_pair(p.first, 0.0));
		for (size_t step = 0; step < n; ++step) {
			Graph_lib::Polygon polygon;
			for (auto p = cur_data.rbegin(); p != cur_data.rend();++p) {
				polygon.add(Point((p->first - sx) / (ex - sx) * lx + cx,
					-(p->second) * ly + cy));
			}
			for (auto p : all_data) {
				polygon.add(Point((p.first - sx) / (ex - sx) * lx + cx,
					-(cur_data[p.first] + p.second[step]) * ly + cy));
				cur_data[p.first] += p.second[step];
			}
			polygon.set_fill_color(fill_color[step % fill_color.size()]);
			polygon.set_color(Graph_lib::Color::invisible);
			polygon.draw();

			if (show_line_label) {
				fl_color(fill_color[step % fill_color.size()]);
				fl_line_style(0, 2);
				fl_line(cx + lx + dx / 2, cy - ly + (step + 1) * 30,
					cx + lx + 50 + dx / 2, cy - ly + (step + 1) * 30);

				Graph_lib::Text text(Point(cx + lx + 70 + dx / 2,
					cy - ly + (step + 1) * 30 + 5), line_label_str[step]);
				text.set_font_size(16);
				text.set_color(0);
				text.draw();
			}

			//Draw the axis.
			fl_color(0);
			fl_line(cx, cy, cx, cy - ly);
			fl_line(cx, cy, cx + lx, cy);

			Graph_lib::Text axis_text(Point(cx + lx + 15, cy - 10), axis_name);
			axis_text.set_color(0);
			axis_text.draw();

			for (int i = 0; i < nx; ++i) {
				fl_line(cx + dx * i, cy, cx + dx * i, cy - 5);
				Graph_lib::Text t(Point(cx + dx * i - 5, cy + 20), label_x[i]);
				t.set_color(0);
				t.draw();
			}
			for (int i = 0; i < ny; ++i) {
				fl_line(cx, cy - dy * i, cx + 5, cy - dy * i);
				Graph_lib::Text t(Point(cx - 40, cy - dy * i + 5), label_y[i]);
				t.set_color(0);
				t.draw();
			}
		}
	}
};
