#ifndef MATRIX4X3SCALAR_H
#define MATRIX4X3SCALAR_H

#include "MatrixInstantiation.h"

namespace Math {

    class Matrix4X3Scalar : public Matrix4X3ScalarBase
    {
    public:
        Matrix4X3Scalar();

        Matrix4X3Scalar(Matrix4X3ScalarBase&);

        //- operator

            void operator=(const Matrix4X3ScalarBase& rhs);
    };

}



#endif // MATRIX4X3SCALAR_H
