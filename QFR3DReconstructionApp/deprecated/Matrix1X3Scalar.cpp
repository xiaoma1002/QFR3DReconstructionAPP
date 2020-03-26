#include "Matrix1X3Scalar.h"

namespace Math {

    Matrix1X3Scalar::Matrix1X3Scalar()
    {

    }

    Matrix1X3Scalar::Matrix1X3Scalar(Matrix1X3ScalarBase & base)
    {
        *this = base;
    }

    void Matrix1X3Scalar::operator=(const Matrix1X3ScalarBase &rhs)
    {
        for (int i = 0; i < 1; i++)
            for (int j = 0; j < 3; j++)
            {
                (*this)(i,j) = rhs(i,j);
            }
    }

}
