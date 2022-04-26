#include "pbPlots.hpp"
#include "supportLib.hpp"
#include <vector>
#include <cmath>
#include <iostream>
#include <limits>
#include <string>
#define LLONG_MAX numeric_limits<long long>().max()

using namespace std;

vector<double> xs;
vector<double> ys;
vector<double> xs1;
vector<double> ys1;
string complexity_class;
const wchar_t* graph_title;
wstring w;

pair<double, double> linearRegression(vector<pair<long long, double>> func) {
    double error = 0;
    long long n = func.size();
    double a;
    double b;

    double v_sum = 0;
    double u_sum = 0;
    double vu_sum = 0;
    double vv_sum = 0;

    for (long long i = 0; i < n; i++) {

        double v = log10(func[i].first);
        double u = log10(func[i].second);

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

void deleteOverflowing(vector<pair<long long, double>>& func) {
    for (auto points_pair = func.begin(); points_pair != func.end(); points_pair++) {
        if ((*points_pair).second > 0.6 * LLONG_MAX || (*points_pair).second < 0.0) {
            func.erase(points_pair, func.end());
            break;
        }
    }
}

long long par, _counter;

long long _factorial(int n) {
    long long f = 1;
    _counter += 1;
    _counter += 1;
for (long long i = 2; _counter += 1, i <= n; i++, _counter += 1) {f *= i, _counter += 2;    }
    _counter += 0;
switch(f) { 
    case (5 * 10 - 3 / 2 + 1):
    cout << "f is 5..." << 5 * 2 * 10 - 3 / 2 + 1 << "\n", _counter += 5;
    break;
    case 1:
    cout << "f is 1" << "\n", _counter += 0;
    break;
    default: 
    cout << "f is not 5 or 1" << "\n", _counter += 0;
    break;
    while(_counter += 0, false) {
    int e = 5;
    _counter += 1;
    if (_counter += 1, e < 7) {e = 7, _counter += 1;    }
        double a[ 100 ][ 100 ];

    double b[ 100 ][ 100 ];

    double c[ 100 ][ 100 ];

    int i, j;

    for (i = 0; i < par; i++) {for (j = 0; j < par; j++) {
    a[ i ][ j ] = 1;
    b[ i ][ j ] = 1;
    c[ i ][ j ] = 0;
    }    }
    i = 4;
    j = 0;
    for (int v = 0; v < par; v++) {
    a[ i ][ j ] = 2;
    i -= 1;
    j += 1;
    }
    i = 4;
    j = 4;
    for (int v = 0; v < par; v++) {
    b[ i ][ j ] = 2;
    i -= 1;
    j -= 1;
    }
    a[ 2 ][ 4 ] = 3;
    for (i = 0; i < par; i++) {
    for (j = 0; j < par; j++) {cout << a[ i ][ j ] << " ";    }
    cout << endl;
    }
    cout << endl << endl;
    b[ 3 ][ 2 ] = 5;
    for (i = 0; i < par; i++) {
    for (j = 0; j < par; j++) {cout << b[ i ][ j ] << " ";    }
    cout << endl;
    }
    cout << endl << endl;
    for (i = 0; i < par; i++) {for (j = 0; j < par; j++) {for (int k = 0; k < par; k++) {c[ i ][ j ] += a[ i ][ k ] * b[ k ][ j ];    }    }    }
    for (i = 0; i < par; i++) {
    for (j = 0; j < par; j++) {cout << c[ i ][ j ] << " ";    }
    cout << endl;
    }

    }
 }
    _counter += 0;
return f;
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
    for (i = 0, _counter += 1; _counter += 1, i < par; i++, _counter += 1) {for (j = 0, _counter += 1; _counter += 1, j < par; j++, _counter += 1) {
    a[ i ][ j ] = 1, _counter += 1;
    b[ i ][ j ] = 1, _counter += 1;
    c[ i ][ j ] = 0, _counter += 1;
    }    }
    i = 4, _counter += 1;
    j = 0, _counter += 1;
    _counter += 1;
for (int v = 0; _counter += 1, v < par; v++, _counter += 1) {
    a[ i ][ j ] = 2, _counter += 1;
    i -= 1, _counter += 2;
    j += 1, _counter += 2;
    }
    i = 4, _counter += 1;
    j = 4, _counter += 1;
    _counter += 1;
for (int v = 0; _counter += 1, v < par; v++, _counter += 1) {
    b[ i ][ j ] = 2, _counter += 1;
    i -= 1, _counter += 2;
    j -= 1, _counter += 2;
    }
    a[ 2 ][ 4 ] = 3, _counter += 1;
    for (i = 0, _counter += 1; _counter += 1, i < par; i++, _counter += 1) {
    for (j = 0, _counter += 1; _counter += 1, j < par; j++, _counter += 1) {cout << a[ i ][ j ] << " ", _counter += 0;    }
    cout << endl, _counter += 0;
    }
    cout << endl << endl, _counter += 0;
    b[ 3 ][ 2 ] = 5, _counter += 1;
    for (i = 0, _counter += 1; _counter += 1, i < par; i++, _counter += 1) {
    for (j = 0, _counter += 1; _counter += 1, j < par; j++, _counter += 1) {cout << b[ i ][ j ] << " ", _counter += 0;    }
    cout << endl, _counter += 0;
    }
    cout << endl << endl, _counter += 0;
    for (i = 0, _counter += 1; _counter += 1, i < par; i++, _counter += 1) {for (j = 0, _counter += 1; _counter += 1, j < par; j++, _counter += 1) {_counter += 1;
for (int k = 0; _counter += 1, k < par; k++, _counter += 1) {c[ i ][ j ] += a[ i ][ k ] * b[ k ][ j ], _counter += 3;    }    }    }
    for (i = 0, _counter += 1; _counter += 1, i < par; i++, _counter += 1) {
    for (j = 0, _counter += 1; _counter += 1, j < par; j++, _counter += 1) {cout << c[ i ][ j ] << " ", _counter += 0;    }
    cout << endl, _counter += 0;
    }
}

void _main() {
    _factorial(4), _counter += 0;
    _matrixComputing(), _counter += 0;
}

vector<pair<long long, double>> createComplexityFunc() {
    vector<pair<long long, double>> result_func;
 for (par = 4; par < 45; par += 1) {
	      _counter = 0;
	      _main();
        result_func.push_back(make_pair(par, _counter)); 
	  }
	  return result_func;
}

vector<pair<long long, double>> createLogarithmicComplexityFunc() {
    vector<pair<long long, double>> result_func;
 for (par = 4; par < 45; par += 1) {
	      _counter = 0;
	      _main();
        result_func.push_back(make_pair(par, _counter / log10(par))); 
	  }
	  return result_func;
}

void findComplexityClass(vector<pair<long long, double>>& func) {
    vector<pair<long long, double>> logarithmic_func;
    pair<double, double> params;
    int first_degree, second_degree;
    double first_error, second_error;
    
    func = createComplexityFunc();

    deleteOverflowing(func);

    params = linearRegression(func);
    first_error = params.first;
    first_degree = (int) round(abs(params.second));

    logarithmic_func = createLogarithmicComplexityFunc();

    deleteOverflowing(logarithmic_func);

    params = linearRegression(logarithmic_func);
    second_error = params.first;
    second_degree = (int) round(abs(params.second));
   
    complexity_class = "O(";

    if (first_error < second_error) {
        if (first_degree != 0) {
            complexity_class += "x";
            if (first_degree > 1 && first_degree <= 5)
				complexity_class += "^" + to_string(first_degree);
			else if (first_degree > 5)
					complexity_class = "> " + complexity_class + "^5";
        }
        else
            complexity_class += "1";
    }
    else {
        if (second_degree != 0) {
            complexity_class += "x";
            if (second_degree > 1 && second_degree <= 5)
				complexity_class += "^" + to_string(second_degree);
			else if (second_degree > 5)
					complexity_class = "> " + complexity_class + "^5";
            complexity_class += " * ";
        }
        complexity_class += "log(x)";
    }

    complexity_class += ")";

    w = wstring(complexity_class.begin(), complexity_class.end());
    graph_title = w.c_str();

}

int main() {
    vector<pair<long long, double>> func;
    findComplexityClass(func);

    RGBABitmapImageReference* imageReference = CreateRGBABitmapImageReference();

    for (auto points_pair = func.begin(); points_pair != func.end(); points_pair++) {
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