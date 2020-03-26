#include "Matrix3X3Scalar.h"

namespace Math {

    Matrix3X3Scalar::Matrix3X3Scalar():
        Matrix3X3ScalarBase()
    {

    }

    Matrix3X3Scalar::Matrix3X3Scalar(Matrix3X3ScalarBase & base)
    {
        *this = base;
    }

    Matrix3X3Scalar Matrix3X3Scalar::inverse()
    {
        Matrix3X3Scalar result;

        Math::inverse(*this,result);

        return result;
    }

    void Matrix3X3Scalar::operator=(const Matrix3X3ScalarBase &rhs)
    {
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
            {
                (*this)(i,j) = rhs(i,j);
            }
    }

}

