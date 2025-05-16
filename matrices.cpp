#include "Matrices.h"
#include <cmath>
#include <stdexcept>
#include <ostream>

using namespace Matrices;

namespace Matrices {

    Matrix::Matrix(int _rows, int _cols)
    : rows(_rows), cols(_cols), a(_rows, std::vector<double>(_cols, 0.0)) { }

    RotationMatrix::RotationMatrix(double theta)
        : Matrix(2, 2) {
        (*this)(0, 0) = std::cos(theta);
        (*this)(0, 1) = -std::sin(theta);
        (*this)(1, 0) = std::sin(theta);
        (*this)(1, 1) = std::cos(theta);
    }

    ScalingMatrix::ScalingMatrix(double scale)
        : Matrix(2, 2) {
        (*this)(0, 0) = scale;
        (*this)(0, 1) = 0.0;
        (*this)(1, 0) = 0.0;
        (*this)(1, 1) = scale;
    }

    TranslationMatrix::TranslationMatrix(double xShift, double yShift, int nCols)
        : Matrix(2, nCols) {
        for (int j = 0; j < nCols; ++j) { 
            (*this)(0, j) = xShift;
            (*this)(1, j) = yShift;
        }
    }

    Matrix operator+(const Matrix& a, const Matrix& b) {
        if (a.getRows() != b.getRows() || a.getCols() != b.getCols())
            throw std::invalid_argument("Matrix sizes do not match for addition");
        Matrix result(a.getRows(), a.getCols());
        for (int i = 0; i < a.getRows(); ++i)
            for (int j = 0; j < a.getCols(); ++j)
                result(i, j) = a(i, j) + b(i, j);
        return result;
    }

    Matrix operator*(const Matrix& a, const Matrix& b) {
        if (a.getCols() != b.getRows())
            throw std::invalid_argument("Matrix sizes do not match for multiplication");
        Matrix result(a.getRows(), b.getCols());
        for (int i = 0; i < a.getRows(); ++i)
            for (int j = 0; j < b.getCols(); ++j) {
                result(i, j) = 0.0;
                for (int k = 0; k < a.getCols(); ++k)
                    result(i, j) += a(i, k) * b(k, j);
            }
        return result;
    }
}



bool operator==(const Matrix& a, const Matrix& b) {
    if (a.getRows() != b.getRows() || a.getCols() != b.getCols())
        return false;
    for (int i = 0; i < a.getRows(); ++i)
        for (int j = 0; j < a.getCols(); ++j)
            if (a(i, j) != b(i, j))
                return false;
    return true;
}

std::ostream& operator<<(std::ostream& os, const Matrix& a) {
    for (int i = 0; i < a.getRows(); ++i) {
        for (int j = 0; j < a.getCols(); ++j) {
            os << a(i, j);
            if (j < a.getCols() - 1) os << ' ';
        }
        os << '\n';
    }
    return os;
}
