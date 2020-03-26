#ifndef MATRIX6X1SCALAR_H
#define MATRIX6X1SCALAR_H

#include "matrixinstantiation.h"

namespace Math {

    class Matrix6X1Scalar : public Matrix6X1ScalarBase
    {

    public:

        Matrix6X1Scalar();

        Matrix6X1Scalar(Matrix6X1ScalarBase&);

        //- operator

            void operator=(const Matrix6X1ScalarBase& rhs);
    };


}

#endif // MATRIX6X1SCALAR_H
