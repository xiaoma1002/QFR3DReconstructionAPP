#ifndef MATRIX3X1SCALAR_H
#define MATRIX3X1SCALAR_H

#include "matrixinstantiation.h"

namespace Math {

    class Matrix3X1Scalar : public Matrix3X1ScalarBase
    {
    public:
        Matrix3X1Scalar();

        Matrix3X1Scalar(Matrix3X1ScalarBase&);

        //- operator

            void operator=(const Matrix3X1ScalarBase& rhs);
    };

}

#endif // MATRIX3X1SCALAR_H
