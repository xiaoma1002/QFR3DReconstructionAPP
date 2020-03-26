#ifndef MATRIX1X3SCALAR_H
#define MATRIX1X3SCALAR_H

#include "matrixinstantiation.h"

namespace Math {

    class Matrix1X3Scalar : public Matrix1X3ScalarBase
    {
    public:
        Matrix1X3Scalar();

        Matrix1X3Scalar(Matrix1X3ScalarBase&);

        //- operator

            void operator=(const Matrix1X3ScalarBase& rhs);
    };


}


#endif // MATRIX1X3SCALAR_H
