#include "Matrix6X6Scalar.h"

namespace Math {

    Matrix6X6Scalar::Matrix6X6Scalar()
    {

    }

    Matrix6X6Scalar::Matrix6X6Scalar(Matrix6X6ScalarBase & base)
    {
        *this = base;
    }

    void Matrix6X6Scalar::operator=(const Matrix6X6ScalarBase &rhs)
    {
        for (int i = 0; i < 6; i++)
            for (int j = 0; j < 6; j++)
            {
                (*this)(i,j) = rhs(i,j);
            }
    }

}

