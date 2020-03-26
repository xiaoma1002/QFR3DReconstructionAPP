#include "Matrix4X3Scalar.h"

namespace Math {

    Matrix4X3Scalar::Matrix4X3Scalar()
    {

    }

    Matrix4X3Scalar::Matrix4X3Scalar(Matrix4X3ScalarBase & base)
    {
        *this = base;
    }

    void Matrix4X3Scalar::operator=(const Matrix4X3ScalarBase &rhs)
    {
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 3; j++)
            {
                (*this)(i,j) = rhs(i,j);
            }
    }

}

