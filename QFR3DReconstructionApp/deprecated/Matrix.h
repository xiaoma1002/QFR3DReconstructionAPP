#ifndef MATRIX_H
#define MATRIX_H

namespace Math
{
    template <int N, int M, typename T>
    class Matrix
    {

    public:

        Matrix();

    private:

        //- member variables

            //- NxM matrix body
            T m[N][M];

            friend class Matrix;

        //- member functions

            void setToZero();

    public:

        //- inline functions

        //- access

            T *data() { return *m; }

            // const T *data() const { return *m; }

            const T *constData() const { return *m; }

            void fill(T value);

            Matrix<M, N, T> transposed() const;

            // Matrix<N, M, T>& replaceWith();

            // virtual Matrix<N, M, T> inverse() const;

        //- operator

            const T& operator()(int row, int column) const;

            T& operator()(int row, int column);

            //- binary operator

            Matrix<N, M, T>& operator+=(const Matrix<N, M, T>& other);

            Matrix<N, M, T>& operator-=(const Matrix<N, M, T>& other);

            Matrix<N, M, T>& operator*=(T factor);

            Matrix<N, M, T>& operator/=(T divisor);

            bool operator==(const Matrix<N, M, T>& other) const;

            bool operator!=(const Matrix<N, M, T>& other) const;

            void operator=(const Matrix<N, M, T>& rhs);

            //- unary operator

            Matrix<N, M, T>& operator+(const Matrix<N, M, T>& other);

            Matrix<N, M, T>& operator-(const Matrix<N, M, T>& other);

            Matrix<N, M, T> operator*(T factor);

            Matrix<N, M, T>& operator/(T divisor);

    };

}

#include "MatrixTemplateImplementation.h"

#endif // MATRIX_H
