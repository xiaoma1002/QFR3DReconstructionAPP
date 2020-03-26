#include "Matrix3X4Scalar.h"

namespace Math {

    Matrix3X4Scalar::Matrix3X4Scalar():
        Matrix3X4ScalarBase()
    {

    }

    Matrix3X4Scalar::Matrix3X4Scalar(Matrix3X4ScalarBase & base)
    {
        *this = base;
    }

    void Matrix3X4Scalar::operator=(const Matrix3X4ScalarBase &rhs)
    {
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 4; j++)
            {
                (*this)(i,j) = rhs(i,j);
            }
    }

}


