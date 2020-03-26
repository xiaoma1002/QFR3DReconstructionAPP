#ifndef MATRIX4X1SCALAR_H
#define MATRIX4X1SCALAR_H

#include "matrixdeclarations.h"

namespace Math {

    class Matrix4X1Scalar : public Matrix4X1ScalarBase
    {
    public:
        Matrix4X1Scalar();

        Matrix4X1Scalar(Matrix4X1ScalarBase&);

        //- operator

            void operator=(const Matrix4X1ScalarBase& rhs);
    };

}


#endif // MATRIX4X1SCALAR_H
