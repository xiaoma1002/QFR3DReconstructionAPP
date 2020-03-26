#ifndef MATRIX8X8SCALAR_H
#define MATRIX8X8SCALAR_H

#include "matrixinstantiation.h"

namespace Math {

    class Matrix8X8Scalar : public Matrix8X8ScalarBase
    {
    public:
        Matrix8X8Scalar();

        Matrix8X8Scalar(Matrix8X8ScalarBase&);

        //- operator

            void operator=(const Matrix8X8ScalarBase& rhs);
    };

}

#endif // MATRIX8X8SCALAR_H
