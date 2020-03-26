#include "Matrix8X8Scalar.h"

namespace Math {

    Matrix8X8Scalar::Matrix8X8Scalar()
    {

    }

    Matrix8X8Scalar::Matrix8X8Scalar(Matrix8X8ScalarBase & base)
    {
        *this = base;
    }

    void Matrix8X8Scalar::operator=(const Matrix8X8ScalarBase &rhs)
    {
        for (int i = 0; i < 8; i++)
            for (int j = 0; j < 8; j++)
            {
                (*this)(i,j) = rhs(i,j);
            }
    }
}

