#ifndef MATRIX3X3SCALAR_H
#define MATRIX3X3SCALAR_H

#include "MatrixInstantiation.h"

namespace Math {

    class Matrix3X3Scalar : public Matrix3X3ScalarBase
    {

    public:

        Matrix3X3Scalar();

        Matrix3X3Scalar(Matrix3X3ScalarBase&);

        Matrix3X3Scalar inverse();

        //- operator

            void operator=(const Matrix3X3ScalarBase& rhs);

    };

}


#endif // MATRIX3X3SCALAR_H
