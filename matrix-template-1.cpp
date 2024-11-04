#include <iostream>
#include <fstream>
#include <iomanip> // For std::setw
#include <stdexcept> // For std::invalid_argument

using namespace std;

class Matrix {
public:
    int matrix_size;
    double** matrix_data;

    Matrix(int size = 0) : matrix_size(size) {
        matrix_data = new double*[matrix_size];
        for (int i = 0; i < matrix_size; ++i) {
            matrix_data[i] = new double[matrix_size];
        }
    }

    Matrix(const Matrix& other) : matrix_size(other.matrix_size) {
        matrix_data = new double*[matrix_size];
        for (int i = 0; i < matrix_size; ++i) {
            matrix_data[i] = new double[matrix_size];
            for (int j = 0; j < matrix_size; ++j) {
                matrix_data[i][j] = other.matrix_data[i][j];
            }
        }
    }

    Matrix& operator=(const Matrix& other) {
        if (this == &other) {
            return *this;
        }

        for (int i = 0; i < matrix_size; ++i) {
            delete[] matrix_data[i];
        }
        delete[] matrix_data;

        matrix_size = other.matrix_size;

        matrix_data = new double*[matrix_size];
        for (int i = 0; i < matrix_size; ++i) {
            matrix_data[i] = new double[matrix_size];
            for (int j = 0; j < matrix_size; ++j) {
                matrix_data[i][j] = other.matrix_data[i][j];
            }
        }

        return *this;
    }

    ~Matrix() {
        for (int i = 0; i < matrix_size; ++i) {
            delete[] matrix_data[i];
        }
        delete[] matrix_data;
    }
};

void read_matrix_from_file(const string& file_name, Matrix& matrix_1, Matrix& matrix_2) {
    ifstream file(file_name);
    if (!file.is_open()) {
        throw invalid_argument("Could not open the file.");
    }

    // Reading the first 4 rows into matrix_1
    for (int i = 0; i < matrix_1.matrix_size; ++i) {
        for (int j = 0; j < matrix_1.matrix_size; ++j) {
            file >> matrix_1.matrix_data[i][j];
        }
    }

    // Reading the next 4 rows into matrix_2
    for (int i = 0; i < matrix_2.matrix_size; ++i) {
        for (int j = 0; j < matrix_2.matrix_size; ++j) {
            file >> matrix_2.matrix_data[i][j];
        }
    }

    file.close();
}

void print_matrix(const Matrix& matrix) {
    for (int i = 0; i < matrix.matrix_size; ++i) {
        for (int j = 0; j < matrix.matrix_size; ++j) {
            cout << setw(5) << matrix.matrix_data[i][j] << " ";
        }
        cout << endl;
    }
}

Matrix operator+(const Matrix& matrix_1, const Matrix& matrix_2) {
    if (matrix_1.matrix_size != matrix_2.matrix_size) {
        throw invalid_argument("Matrices must be of the same size for addition.");
    }
    
    Matrix result(matrix_1.matrix_size);
    for (int i = 0; i < matrix_1.matrix_size; ++i) {
        for (int j = 0; j < matrix_1.matrix_size; ++j) {
            result.matrix_data[i][j] = matrix_1.matrix_data[i][j] + matrix_2.matrix_data[i][j];
        }
    }
    return result;
}

Matrix operator*(const Matrix& matrix_1, const Matrix& matrix_2) {
    if (matrix_1.matrix_size != matrix_2.matrix_size) {
        throw invalid_argument("Matrices must be of the same size for multiplication.");
    }

    Matrix result(matrix_1.matrix_size);
    for (int i = 0; i < matrix_1.matrix_size; ++i) {
        for (int j = 0; j < matrix_2.matrix_size; ++j) {
            result.matrix_data[i][j] = 0;
            for (int k = 0; k < matrix_1.matrix_size; ++k) {
                result.matrix_data[i][j] += matrix_1.matrix_data[i][k] * matrix_2.matrix_data[k][j];
            }
        }
    }
    return result;
}

void get_diagonal_sum(const Matrix& matrix) {
    double main_diagonal_sum = 0.0;
    double secondary_diagonal_sum = 0.0;

    for (int i = 0; i < matrix.matrix_size; ++i) {
        main_diagonal_sum += matrix.matrix_data[i][i];
        secondary_diagonal_sum += matrix.matrix_data[i][matrix.matrix_size - 1 - i];
    }

    cout << "Main diagonal sum: " << main_diagonal_sum << endl;
    cout << "Secondary diagonal sum: " << secondary_diagonal_sum << endl;
    cout << "Total diagonal sum: " << (main_diagonal_sum + secondary_diagonal_sum) << endl;
}

void swap_matrix_row(Matrix& matrix, int row1, int row2) {
    if (row1 < 0 || row1 >= matrix.matrix_size || row2 < 0 || row2 >= matrix.matrix_size) {
        throw invalid_argument("Row indices are out of bounds.");
    }

    for (int j = 0; j < matrix.matrix_size; ++j) {
        swap(matrix.matrix_data[row1][j], matrix.matrix_data[row2][j]);
    }

    cout << "Matrix after swapping rows " << row1 << " and " << row2 << ":" << endl;
    print_matrix(matrix);
}

int main() {
    // Assuming both matrices are 4x4 as per your input
    Matrix matrix_1(4), matrix_2(4);
    
    try {
        read_matrix_from_file("matrix.txt", matrix_1, matrix_2);

        cout << "Matrix 1:" << endl;
        print_matrix(matrix_1);
        cout << "Matrix 2:" << endl;
        print_matrix(matrix_2);

        Matrix add_result = matrix_1 + matrix_2;
        cout << "Addition result:" << endl;
        print_matrix(add_result);

        Matrix multiply_result = matrix_1 * matrix_2;
        cout << "Multiplication result:" << endl;
        print_matrix(multiply_result);

        get_diagonal_sum(matrix_1);
        
        cout << "Swapping rows 0 and 1 in Matrix 1." << endl;
        swap_matrix_row(matrix_1, 0, 1);
        
    } catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
    }

    return 0;
}