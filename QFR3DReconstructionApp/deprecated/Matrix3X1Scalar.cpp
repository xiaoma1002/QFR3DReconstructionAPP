#include "Matrix3X1Scalar.h"

namespace Math {

    Matrix3X1Scalar::Matrix3X1Scalar()
    {

    }

    Matrix3X1Scalar::Matrix3X1Scalar(Matrix3X1ScalarBase & base)
    {
        *this = base;
    }

    void Matrix3X1Scalar::operator=(const Matrix3X1ScalarBase &rhs)
    {
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 1; j++)
            {
                (*this)(i,j) = rhs(i,j);
            }
    }
}

