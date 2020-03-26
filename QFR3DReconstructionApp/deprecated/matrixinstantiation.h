#ifndef MATRIXINSTANTIATION_H
#define MATRIXINSTANTIATION_H

#include "Matrix.h"

namespace Math {

    typedef Matrix<3, 3, double> Matrix3X3ScalarBase; //- R,K,F
    typedef Matrix<3, 1, double> Matrix3X1ScalarBase; //- t
    typedef Matrix<6, 1, double> Matrix6X1ScalarBase; //- construct 3D point by Ax=b
    typedef Matrix<8, 1, double> Matrix8X1ScalarBase; //- construct 3D point by Ax=b
    typedef Matrix<4, 1, double> Matrix4X1ScalarBase; //- Xest
    typedef Matrix<1, 3, double> Matrix1X3ScalarBase; //- t^T
    typedef Matrix<3, 4, double> Matrix3X4ScalarBase; //- P,[R|t]
    typedef Matrix<4, 3, double> Matrix4X3ScalarBase; //-

    typedef Matrix<6, 6, double> Matrix6X6ScalarBase; //- Reconstruct 3D point
    typedef Matrix<8, 8, double> Matrix8X8ScalarBase; //- 3D reconstruction
}

#endif // MATRIXINSTANTIATION_H
