#include "Matrix6X1Scalar.h"

namespace Math {

    Matrix6X1Scalar::Matrix6X1Scalar()
    {

    }


    Matrix6X1Scalar::Matrix6X1Scalar(Matrix6X1ScalarBase & base)
    {
        *this = base;
    }

    void Matrix6X1Scalar::operator=(const Matrix6X1ScalarBase &rhs)
    {
        for (int i = 0; i < 6; i++)
            for (int j = 0; j < 1; j++)
            {
                (*this)(i,j) = rhs(i,j);
            }
    }

}

