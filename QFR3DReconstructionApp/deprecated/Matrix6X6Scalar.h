#ifndef MATRIX6X6SCALAR_H
#define MATRIX6X6SCALAR_H

#include "matrixinstantiation.h"

namespace Math {

    class Matrix6X6Scalar : public Matrix6X6ScalarBase
    {
    public:
        Matrix6X6Scalar();

        Matrix6X6Scalar(Matrix6X6ScalarBase&);

        //- operator

            void operator=(const Matrix6X6ScalarBase& rhs);
    };

}

#endif // MATRIX6X6SCALAR_H
