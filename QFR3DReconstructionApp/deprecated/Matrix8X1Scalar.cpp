#include "Matrix8X1Scalar.h"

namespace Math {

    Matrix8X1Scalar::Matrix8X1Scalar()
    {

    }

    Matrix8X1Scalar::Matrix8X1Scalar(Matrix8X1ScalarBase & base)
    {
        *this = base;
    }

    void Matrix8X1Scalar::operator=(const Matrix8X1ScalarBase &rhs)
    {
        for (int i = 0; i < 8; i++)
            for (int j = 0; j < 1; j++)
            {
                (*this)(i,j) = rhs(i,j);
            }
    }

}

