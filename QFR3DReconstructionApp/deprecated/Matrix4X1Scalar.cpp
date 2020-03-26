#include "Matrix4X1Scalar.h"

namespace Math {

    Matrix4X1Scalar::Matrix4X1Scalar()
    {

    }

    Matrix4X1Scalar::Matrix4X1Scalar(Matrix4X1ScalarBase & base)
    {
        *this = base;
    }

    void Matrix4X1Scalar::operator=(const Matrix4X1ScalarBase &rhs)
    {
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 1; j++)
            {
                (*this)(i,j) = rhs(i,j);
            }
    }
}

