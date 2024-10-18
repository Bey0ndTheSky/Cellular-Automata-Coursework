#pragma once
#include <vector>
#include <utility>
#include <cmath>
#include <stdexcept>
#include <algorithm>

template <typename T>
class Array2D {
public:
    Array2D();
    Array2D(int rows, int cols);
    Array2D(int rows, int cols, const T& def);
    ~Array2D();

    typename std::vector<T>::reference operator()(int row, int col);
    const T& operator()(int row, int col) const;

    typename std::vector<T>::reference operator[](int ix);
    const T& operator[](int ix) const;

    int getRows() const;
    int getCols() const;
    int getSize() const;

    inline int get1D(int row, int col) const;
    inline std::pair<int,int> get2D(int ix) const;
    void reverse();

    typename std::vector<T>::const_iterator begin() const {
        return data.begin();
    }

    typename std::vector<T>::const_iterator end() const {
        return data.end();
    }

private:
    std::vector<T> data;
    int rows;
    int cols;
};

template <typename T>
Array2D<T>::Array2D()
    : data() {}

template <typename T>
Array2D<T>::Array2D(int rows, int cols)
    : data(rows * cols), rows(rows), cols(cols) {}

template <typename T>
Array2D<T>::Array2D(int rows, int cols, const T& def)
    : data(rows* cols, def), rows(rows), cols(cols) {}

template <typename T>
Array2D<T>::~Array2D() {}

template <typename T>
typename std::vector<T>::reference Array2D<T>::operator()(int row, int col) {
    return data[get1D(row, col)];
}

template <typename T>
const T& Array2D<T>::operator()(int row, int col) const {
       return data[get1D(row, col)];
}

template <typename T>
typename std::vector<T>::reference Array2D<T>::operator[](int ix) {
    if (ix < 0 || ix >= data.size()) {
        throw std::out_of_range("Row index out of bounds");
    }
    return data[ix];
}

template <typename T>
const T& Array2D<T>::operator[](int ix) const {
    if (ix < 0 || ix >= data.size()) {
        throw std::out_of_range("Row index out of bounds");
    }
    return data[ix];
}

template <typename T>
inline int Array2D<T>::get1D(int row, int col) const {
    if (row < 0 || row >= rows || col < 0 || col >= cols) {
        throw std::out_of_range("Index out of bounds");
    }
    return row * cols + col;
}

template <typename T>
inline std::pair<int, int> Array2D<T>::get2D(int ix) const {
    if (ix < 0 || ix >= data.size()) {
        throw std::out_of_range("Index out of bounds");
    }
    return (cols & (cols - 1)) == 0
        ? std::make_pair(ix >> static_cast<int>(std::log2(cols)), ix & (cols - 1))
        : std::make_pair(ix / cols, ix % cols);
}

template <typename T>
void Array2D<T>::reverse() {
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols / 2; ++j) {
            int l = i * cols + j;              
            int r = i * cols + (cols - j - 1);
            std::swap(data[l], data[r]);
        }
    }
}

template <typename T>
int Array2D<T>::getRows() const {
    return rows;
}

template <typename T>
int Array2D<T>::getCols() const {
    return cols;
}

template <typename T>
int Array2D<T>::getSize() const {
    return data.size();
}