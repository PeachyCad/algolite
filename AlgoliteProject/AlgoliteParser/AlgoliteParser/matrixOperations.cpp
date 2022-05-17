using Vector = vector<double>;
using Matrix = vector<Vector>;

Matrix x_matrix;
Matrix x_matrix_tra;
Vector y_vector;

void createXY(int polynomial_degree, vector<pair<long long, double>> func) {
    Vector current_vector;
    for (const pair<long long, double>& point : func) {
        double current_x = 1;

        for (int i = 0; i <= polynomial_degree; i++) {
            current_vector.push_back(current_x);
            current_x *= point.first;
        }

        x_matrix.push_back(current_vector);
        y_vector.push_back(point.second);

        current_vector.clear();
    }
}

void multiplyMV(const Matrix& a, const Vector& f, Vector& out) {
    for (int i = 0; i < a.size(); i++) {
        out[i] = 0;
        for (int j = 0; j < a[i].size(); j++) {
            out[i] += a[i][j] * f[j];
        }
    }
}

void multiplyMM(const Matrix& a, const Matrix& b, Matrix& out) {
    for (int i = 0; i < a.size(); i++) {
        for (int j = 0; j < b[0].size(); j++) {
            out[i][j] = 0;
            for (int k = 0; k < b.size(); k++) {
                out[i][j] += a[i][k] * b[k][j];
            }
        }
    }
}

void multiplyMTM(const Matrix& a, Matrix& out) {
    for (int i = 0; i < a[0].size(); i++) {
        for (int j = 0; j < a[0].size(); j++) {
            out[i][j] = 0;
            for (int k = 0; k < a.size(); k++) {
                out[i][j] += a[k][i] * a[k][j];
            }
        }
    }
}

void multiplyMTV(const Matrix& a, const Vector& f, Vector& out) {
    for (int i = 0; i < a[0].size(); i++) {
        out[i] = 0;
        for (int j = 0; j < a.size(); j++) {
            out[i] += a[j][i] * f[j];
        }
    }
}

void getGaussianSolution(const Matrix& a, const Vector& f, Vector& out) {
    Matrix m = a;

    out = f;

    for (int i = 0; i < m.size(); i++) {
        int best_index = i;
        double best_val = abs(m[i][i]);
        for (int index = i + 1; index < m.size(); index++)
            if (best_val < abs(m[index][i])) {
                best_val = abs(m[index][i]);
                best_index = index;
            }

        if (best_index != i) {
            for (int j = i; j < m[0].size(); j++)
                swap(m[best_index][j], m[i][j]);
            swap(out[best_index], out[i]);
        }

        double val = m[i][i];
        for (int k = 0; k < m[0].size(); k++) {
            m[i][k] /= val;
        }
        out[i] /= val;

        for (int j = i + 1; j < m.size(); j++) {
            double current_column_element = m[j][i];
            for (int k = 0; k < m[0].size(); k++) {
                m[j][k] -= m[i][k] * current_column_element;
            }
            out[j] -= out[i] * current_column_element;
        }

    }

    for (int i = m.size() - 1; i >= 0; i--) {
        for (int j = i + 1; j < m[0].size(); j++) {
            out[i] -= m[i][j] * out[j];
        }
    }
}