#ifndef MATRIX3X4SCALAR_H
#define MATRIX3X4SCALAR_H

#include "MatrixInstantiation.h"

namespace Math {

    class Matrix3X4Scalar : public Matrix3X4ScalarBase
    {
    public:
        Matrix3X4Scalar();

        Matrix3X4Scalar(Matrix3X4ScalarBase&);

        //- operator
        void operator=(const Matrix3X4ScalarBase &rhs);

    };

}
#endif // MATRIX3X4SCALAR_H
