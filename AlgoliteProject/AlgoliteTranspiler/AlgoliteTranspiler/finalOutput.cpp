#include "pbPlots.hpp"
#include "supportLib.hpp"
#include "mingw.thread.h"
#include <vector>
#include <cmath>
#include <iostream>
#include <limits>
#include <string>
#include <sstream>
#define LLONG_MAX numeric_limits<long long>().max()

using namespace std;
#include "matrixOperations.cpp"

vector<double> xs;
vector<double> ys;
vector<double> xs1;
vector<double> ys1;
string complexity_class;
string complete_complexity_function;
const wchar_t* graph_title;
wstring w;

pair<double, double> linearRegression(vector<pair<long long, double> > func, bool log_first_var, bool log_second_var) {
	double error = 0;
	long long n = func.size();
	double a;
	double b;

	double v_sum = 0;
	double u_sum = 0;
	double vu_sum = 0;
	double vv_sum = 0;

	for (long long i = 0; i < n; i++) {

		double v = log_first_var ? log10(func[i].first) : func[i].first;
		double u = log_second_var ? log10(func[i].second) : func[i].second;

		v_sum += v;
		u_sum += u;
		vu_sum += v * u;
		vv_sum += v * v;
	}

	b = (n * vu_sum - v_sum * u_sum) / (n * vv_sum - v_sum * v_sum);
	a = (u_sum - b * v_sum) / n;

	for (long long i = 0; i < n; i++) {
		double v = log10(func[i].first);
		double u = log10(func[i].second);

		double estimated_u = a + b * v;
		error += (estimated_u - u) * (estimated_u - u);
	}

	error /= n;
	return make_pair(error, b);
}

Vector polynomialRegression(int polynomial_degree, vector<pair<long long, double> > func) {
	createXY(polynomial_degree, func);
	Matrix left_matrix(polynomial_degree + 1);
	Vector right_vector(polynomial_degree + 1);
	Vector result(polynomial_degree + 1);

	for (int i = 0; i < left_matrix.size(); i++) {
		Vector current_vector(polynomial_degree + 1);
		left_matrix[i] = current_vector;
	}

	multiplyMTM(x_matrix, left_matrix);
	multiplyMTV(x_matrix, y_vector, right_vector);

	getGaussianSolution(left_matrix, right_vector, result);

	return result;
}

string polynomialComplexityFunction(int polynomial_degree, vector<pair<long long, double> > func) {
	vector<double> resultCoefficients = polynomialRegression(polynomial_degree, func);
	stringstream result;

	result.precision(2);

	for (int i = polynomial_degree; i > 0; i--) {
		result << resultCoefficients[i] << " * x ^ " << i << " + ";
	}
	result << resultCoefficients[0];

	return result.str();
}

void deleteOverflowing(vector<pair<long long, double> >& func) {
	for (auto points_pair = func.begin(); points_pair != func.end(); points_pair++) {
		if ((*points_pair).second > 0.6 * LLONG_MAX || (*points_pair).second < 0.0) {
			func.erase(points_pair, func.end());
			break;
		}
	}
}

__thread long long _counter;
__thread long long par;
__thread int global_thread_number;
vector<pair<long long, double> > func_vec(96);

long long _everything() {
	long long a = 42;
	_counter += 1;
	_counter += 0;
	return a;
}

void _matrixComputing() {
	double a[ 100 ][ 100 ];
	_counter += 0;
	double b[ 100 ][ 100 ];
	_counter += 0;
	double c[ 100 ][ 100 ];
	_counter += 0;
	int i, j;
	_counter += 0;
	for (i = 0, _counter += 1; _counter += 1, i < par; i++, _counter += 1) {
		for (j = 0, _counter += 1; _counter += 1, j < par; j++, _counter += 1) {
			a[ i ][ j ] = 1, _counter += 1;
			b[ i ][ j ] = 1, _counter += 1;
			c[ i ][ j ] = 0, _counter += 1;
		}
	}
	i = 4, _counter += 1;
	j = 0, _counter += 1;
	_counter += 1;
	for (int v = 0; _counter += 1, v < par; v++, _counter += 1) {
		a[ i ][ j ] = 2, _counter += 1;
		i -= 1, _counter += 2;
		j += 1, _counter += 2;
	}
}

void _main() {
	_matrixComputing(), _counter += 0;
}

void fillComplexityFuncInThread(int thread_number) {
	global_thread_number = thread_number;
	int index = 0;
	for (par = 4 + 1 * global_thread_number; par < 100; par += 1 * 4) {
		_counter = 0;
		_main();
		func_vec[global_thread_number + index] = make_pair(par, _counter);
		index += 4;
	}
}

void createComplexityFunc() {
	thread t0(fillComplexityFuncInThread, 0);
	thread t1(fillComplexityFuncInThread, 1);
	thread t2(fillComplexityFuncInThread, 2);
	thread t3(fillComplexityFuncInThread, 3);
	t0.join();
	t1.join();
	t2.join();
	t3.join();
}

void findComplexityClass() {
	vector<pair<long long, double> > func;
	vector<pair<long long, double> > logarithmic_func;
	vector<pair<long long, double> > exponential_func;
	pair<double, double> params;
	int polynomial_degree, logarithmic_degree, exponential_degree, func_size, part, mod;
	double polynomial_error, logarithmic_error, exponential_error;

	int polynomial_error_increasings = 0;
	int logarithmic_error_increasings = 0;
	int exponential_error_increasings = 0;
	int polynomial_degree_changes = 0;
	int logarithmic_degree_changes = 0;
	int exponential_degree_changes = 0;
	double last_error = INFINITY;
	int last_degree = -1;
	const int PARTS_NUMBER = 10;
	complete_complexity_function = "";

	createComplexityFunc();

	func = func_vec;

	logarithmic_func = func;
	for (pair<long long, double>& element : logarithmic_func) {
		element.second /= log10(element.first);
	}

	exponential_func = func;

	deleteOverflowing(func_vec);
	deleteOverflowing(func);

	func_size = func.size();
	part = func_size / PARTS_NUMBER;
	mod = func_size - part * PARTS_NUMBER;

	for (int i = part + mod; i < func_size; i += part) {
		vector<pair<long long, double> > current_func(func.begin(), func.begin() + i);
		params = linearRegression(current_func, true, true);

		double current_error = params.first;
		int current_degree = round(abs(params.second));

		if (current_error > last_error * 1.1)
			polynomial_error_increasings++;
		last_error = current_error;

		if (current_degree != last_degree)
			polynomial_degree_changes++;
		last_degree = current_degree;
	}

	polynomial_error = last_error;
	polynomial_degree = last_degree;

	last_error = INFINITY;
	last_degree = -1;

	deleteOverflowing(logarithmic_func);

	func_size = logarithmic_func.size();
	part = func_size / PARTS_NUMBER;
	mod = func_size - part * PARTS_NUMBER;

	for (int i = part + mod; i < func_size; i += part) {
		vector<pair<long long, double> > current_func(logarithmic_func.begin(), logarithmic_func.begin() + i);
		params = linearRegression(current_func, true, true);

		double current_error = params.first;
		int current_degree = round(abs(params.second));

		if (current_error > last_error * 1.1)
			logarithmic_error_increasings++;
		last_error = current_error;

		if (current_degree != last_degree)
			logarithmic_degree_changes++;
		last_degree = current_degree;
	}

	logarithmic_error = last_error;
	logarithmic_degree = last_degree;

	last_error = INFINITY;
	last_degree = -1;

	deleteOverflowing(exponential_func);

	func_size = exponential_func.size();
	part = func_size / PARTS_NUMBER;
	mod = func_size - part * PARTS_NUMBER;

	for (int i = part + mod; i < func_size; i += part) {
		vector<pair<long long, double> > current_func(exponential_func.begin(), exponential_func.begin() + i);
		params = linearRegression(current_func, false, true);

		double current_error = params.first;
		int current_degree = round(abs(params.second));

		if (current_error > last_error * 1.1)
			exponential_error_increasings++;
		last_error = current_error;

		if (current_degree != last_degree)
			exponential_degree_changes++;
		last_degree = current_degree;
	}

	exponential_error = last_error;
	exponential_degree = last_degree;

	bool class_is_unknown = false;

	complexity_class = "O(";

	if (polynomial_error < logarithmic_error && polynomial_error < exponential_error) {
		if ((polynomial_error_increasings <= PARTS_NUMBER / 2 || polynomial_error < 1e-04) && polynomial_degree_changes < 5) {
			if (polynomial_degree != 0) {
				complexity_class += "x";
				if (polynomial_degree > 1 && polynomial_degree <= 5)
					complexity_class += "^" + to_string(polynomial_degree);
				else if (polynomial_degree > 5)
					complexity_class = "> " + complexity_class + "^5";
			}
			else
				complexity_class += "1";
			complete_complexity_function = "; " + polynomialComplexityFunction(polynomial_degree, func_vec);
		}
		else
			class_is_unknown = true;
	}
	else if (logarithmic_error < polynomial_error && logarithmic_error < exponential_error) {
		if ((logarithmic_error_increasings <= PARTS_NUMBER / 2 || logarithmic_error < 1e-04) && logarithmic_degree_changes < 5) {
			if (logarithmic_degree != 0) {
				complexity_class += "x";
				if (logarithmic_degree > 1 && logarithmic_degree <= 5)
					complexity_class += "^" + to_string(logarithmic_degree);
				else if (logarithmic_degree > 5)
					complexity_class = "> " + complexity_class + "^5";
				complexity_class += " * ";
			}
			complexity_class += "log(x)";
		}
		else
			class_is_unknown = true;
	}
	else if (exponential_error <= polynomial_error && exponential_error <= logarithmic_error) {
		if ((exponential_error_increasings <= PARTS_NUMBER / 2 || exponential_error < 1e-04) && exponential_degree_changes < 5) {
			if (exponential_degree != 0) {
				complexity_class += "e^";
				if (exponential_degree > 1)
					complexity_class += "(" + to_string(exponential_degree) + "x)";
				else
					complexity_class += "x";
			}
			else
				complexity_class += "1";
		}
		else
			class_is_unknown = true;
	}
	else
		class_is_unknown = true;

	complexity_class += ")" + complete_complexity_function;

	if (class_is_unknown)
		complexity_class = "Unknown";

	w = wstring(complexity_class.begin(), complexity_class.end());
	graph_title = w.c_str();

}
int main() {
	findComplexityClass();

	RGBABitmapImageReference* imageReference = CreateRGBABitmapImageReference();

	for (auto points_pair = func_vec.begin(); points_pair != func_vec.end(); points_pair++) {
		xs.push_back((*points_pair).first);
		ys.push_back((*points_pair).second);
	}

	ScatterPlotSeries* series = GetDefaultScatterPlotSeriesSettings();
	series->xs = &xs;
	series->ys = &ys;
	series->linearInterpolation = true;
	series->lineThickness = 5;
	series->color = CreateRGBColor(0.5, 0.7, 1);

	ScatterPlotSettings* settings = GetDefaultScatterPlotSettings();
	settings->width = 1000;
	settings->height = 1000;
	settings->autoBoundaries = true;
	settings->autoPadding = true;
	settings->title = toVector(graph_title);
	settings->xLabel = toVector(L"complexity parameter");
	settings->yLabel = toVector(L"operations");
	settings->scatterPlotSeries->push_back(series);
	settings->gridColor = CreateRGBColor(0.6, 0.6, 0.6);
	settings->showGrid = true;

	DrawScatterPlotFromSettings(imageReference, settings);

	vector<double>* pngdata = ConvertToPNG(imageReference->image);

	WriteToFile(pngdata, "func.png");
	DeleteImage(imageReference->image);
	system("start func.png");
}