#ifndef MATRIX8X1SCALAR_H
#define MATRIX8X1SCALAR_H

#include "matrixinstantiation.h"

namespace Math {

    class Matrix8X1Scalar : public Matrix8X1ScalarBase
    {

    public:

        Matrix8X1Scalar();

        Matrix8X1Scalar(Matrix8X1ScalarBase&);

        //- operator

            void operator=(const Matrix8X1ScalarBase& rhs);
    };

}



#endif // MATRIX8X1SCALAR_H
