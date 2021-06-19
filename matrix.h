#pragma once

#include <memory>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <numeric>
#include <functional>

//#define NDEBUG

// custom assert with message
#ifndef NDEBUG
#   define M_Assert(Expr, Msg) \
    __M_Assert(#Expr, Expr, __FILE__, __LINE__, Msg)
#else
#   define M_Assert(Expr, Msg) ;
#endif

void __M_Assert(const char* expr_str, bool expr, const char* file, int line, const char* msg)
{
	if (!expr)
	{
		std::cerr << "Assert failed:\t" << msg << "\n"
			<< "Expected:\t" << expr_str << "\n"
			<< "Source:\t\t" << file << ", line " << line << "\n";
		abort();
	}
}

template<class X>
class matrix {
	X* _m;
	size_t _rows;
	size_t _columns;
public:

	matrix(const size_t h, const size_t w) {
		_m = new X[w * h];
		for (size_t i = 0; i < (w * h); ++i) {
			_m[i] = i;
		}
		_rows = h;
		_columns = w;
	}

	matrix(const matrix<X>& m) {
		this->_rows = m._rows;
		this->_columns = m._columns;
		this->_m = new X[this->_rows * this->_columns];
		std::copy(m._m, m._m + m._columns * m._rows, this->_m);
	}

	matrix(const std::initializer_list<std::initializer_list<X>>& l) :
		_rows(l.size()),
		_columns(l.begin()->size()) {
		_m = new X[_rows * _columns];

		size_t i = 0, j = 0;
		for (const auto& x : l) {
			for (const auto& y : x) {
				_m[to1D(i, j)] = y;
				++j;
			}
			++i;
			j = 0;
		}
	}

	~matrix() {
		delete[] _m;
	}

	size_t rows() {
		return _rows;
	}
	size_t columns() {
		return _columns;
	}

	size_t to1D(size_t row, size_t column) const {
		return row * _columns + column;
	}

	size_t to1D(size_t row, size_t column, size_t matrixColumns) const {
		return row * matrixColumns + column;
	}

	X& operator()(size_t row, size_t column) {
		return _m[to1D(row, column)];
	}

	X& operator()(size_t row, size_t column) const {
		return _m[to1D(row, column)];
	}

	X& first() {
		return _m[0];
	}

	X& last() {
		return _m[_rows * _columns];
	}

	size_t size() {
		size_t size = _rows * _columns;
		return size;
	}

	X sum() {
		return std::accumulate(_m, _m + size(), 0.0);
	}

	matrix<X> sum(int axis, bool keepdims = false) {
		// by columns
		if (axis == 0) {
			matrix<X> out(1, _columns);
			for (size_t i = 0; i < _columns; ++i) {
				out(0, i) = 0.0;
				for (size_t j = 0; j < _rows; ++j) {
					out(0, i) += _m[to1D(j, i)];
				}
			}
			return out;
		}

		// by rows, return as 1,_rows (horizontal vector)
		if (keepdims == false) {
			matrix<X> out(1, _rows);
			for (size_t i = 0; i < _rows; ++i) {
				out(0, i) = 0.0;
				for (size_t j = 0; j < _columns; ++j) {
					out(0, i) += _m[to1D(i, j)];
				}
			}
			return out;
		}

		// by rows, return as _rows,1 (vertical vector)
		matrix<X> out(_rows, 1);
		for (size_t i = 0; i < _rows; ++i) {
			out(i, 0) = 0.0;
			for (size_t j = 0; j < _columns; ++j) {
				out(i, 0) += _m[to1D(i, j)];
			}
		}
		return out;
	}

	void exp() {
		std::transform(_m, _m + size(), _m, static_cast<double(*)(double)>(std::exp));
	}

	X max() {
		return *(std::max_element(_m, _m + size()));
	}

	matrix<X> max(int axis, bool keepdims = false) {
		// by columns
		if (axis == 0) {
			matrix<X> out(1, _columns);


			for (size_t i = 0; i < _columns; ++i) {
				out(0, i) = _m[to1D(0, i)];
				for (size_t j = 1; j < _rows; ++j) {
					if (out(0, i) < _m[to1D(j, i)])
						out(0, i) = _m[to1D(j, i)];
				}
			}


			return out;
		}

		// by rows, return as 1,_rows (horizontal vector)
		if (keepdims == false) {
			matrix<X> out(1, _rows);
			for (size_t i = 0; i < _rows; ++i) {
				out(0, i) = _m[to1D(i, 0)];
				for (size_t j = 0; j < _columns; ++j) {
					if (out(0, i) < _m[to1D(i, j)])
						out(0, i) = _m[to1D(i, j)];
				}
			}
			return out;
		}

		// by rows, return as _rows,1 (vertical vector)
		matrix<X> out(_rows, 1);
		for (size_t i = 0; i < _rows; ++i) {
			out(i, 0) = _m[to1D(i, 0)];
			for (size_t j = 0; j < _columns; ++j) {
				if (out(i, 0) < _m[to1D(i, j)])
					out(i, 0) = _m[to1D(i, j)];
			}
		}
		return out;
	}

	matrix T() {
		matrix<X> newM(_columns, _rows);

for (int i = 0; i < _rows; ++i) {
	for (int j = 0; j < _columns; ++j) {
		newM(j, i) = _m[to1D(i, j)];
	}
}

return newM;
	}

	// TODO: implement a faster matrix multiplication method
	matrix operator*(const matrix& m) {
		M_Assert(this->_columns == m._rows, "Can't multiply! Matrix dimensions must match (A.columns == B.rows)");

		matrix<X> out(this->_rows, m._columns);

		for (size_t i = 0; i < this->_rows; ++i)
			for (size_t j = 0; j < m._columns; ++j)
				out(i, j) = 0;

		for (size_t i = 0; i < this->_rows; ++i)
			for (size_t j = 0; j < m._columns; ++j)
				for (size_t k = 0; k < this->_columns; ++k)
					out(i, j) += (*this)(i, k) * m(k, j);

		return out;
	}

	void operator*=(const matrix& m) {
		M_Assert(this->_columns == m._rows, "Can't multiply! Matrix dimensions must match (A.columns == B.rows)");

		X* newM = new X[this->_rows * m._columns];

		std::fill_n(newM, (this->_rows * m._columns), 0);

		// for (size_t i = 0; i < this->_rows*m._columns; ++i) {
		//     newM[i] = 0;
		// }

		for (size_t i = 0; i < this->_rows; ++i)
			for (size_t j = 0; j < m._columns; ++j)
				for (size_t k = 0; k < this->_columns; ++k)
					newM[to1D(i, j, m._columns)] += (*this)(i, k) * m(k, j);

		delete[] _m;
		_m = newM;
		this->_columns = m._columns;
	}

	matrix& operator=(const matrix& m) {
		M_Assert((this->_columns == m._columns) &&
			(this->_rows == m._rows),
			"Can't assign! Matrix dimensions must match (A.columns == B.rows)");

		if (this != &m) {
			std::copy(m._m, m._m + m._columns * m._rows, this->_m);
		}

		return *this;
	}

	matrix operator+(const matrix& m) {
		matrix<X> out(*this);
		mathOperationRaw(out, m, std::plus<X>());
		return out;
	}
	void operator+=(const matrix& m) {
		mathOperationRaw(*this, m, std::plus<X>());
	}
	matrix operator-(const matrix& m) {
		matrix<X> out(*this);
		mathOperationRaw(out, m, std::minus<X>());
		return out;
	}
	void operator-=(const matrix& m) {
		mathOperationRaw(*this, m, std::minus<X>());
	}
	matrix operator/(const matrix& m) {
		matrix<X> out(*this);
		mathOperationRaw(out, m, std::divides<X>());
		return out;
	}
	void operator/=(const matrix& m) {
		mathOperationRaw(*this, m, std::divides<X>());
	}

	void mathOperationRaw(const matrix& out, const matrix& m, std::function<X(X, X)> func) {
		size_t maxRows = std::max(out._rows, m._rows);
		size_t maxColumns = std::max(out._columns, m._columns);

		if ((out._columns != 1 && out._columns != maxColumns) ||
			(out._rows != 1 && out._rows != maxRows) ||
			(m._columns != 1 && m._columns != maxColumns) ||
			(m._rows != 1 && m._rows != maxRows)) {
			std::ostringstream errorMessage;
			errorMessage << "Can't perform matrix operation! Invalid matrices size: (" << out._rows << "," << out._columns << ") and (" << m._rows << "," << m._columns << ")...\n";

			M_Assert(false, errorMessage.str().c_str());
		}

		for (size_t i = 0; i < maxRows; ++i) {
			for (size_t j = 0; j < maxColumns; ++j) {
				size_t outIndexColumns = (out._columns == 1) ? 0 : j;
				size_t outIndexRows = (out._rows == 1) ? 0 : i;
				size_t mIndexColumns = (m._columns == 1) ? 0 : j;
				size_t mIndexRows = (m._rows == 1) ? 0 : i;

				out(outIndexRows, outIndexColumns) = func(out(outIndexRows, outIndexColumns), m(mIndexRows, mIndexColumns));
			}
		}

	}

	void randomizeDouble() {
		std::generate(_m, _m + _rows * _columns, []() {return (static_cast<X>(rand()) / static_cast<X>(RAND_MAX)) * 2.0 - 1.0; });
	}

	friend std::ostream& operator<<(std::ostream& os, matrix& m) {
		os << "\n> Matrix " << m._rows << "x" << m._columns << ":\n";

		for (size_t i = 0; i < m._rows; ++i) {
			for (size_t j = 0; j < m._columns; ++j) {
				os.width(10);
				os << std::right << m(i, j) << ",";
			}
			os << "\n";
		}

		os << "\n";

		return os;
	}

};