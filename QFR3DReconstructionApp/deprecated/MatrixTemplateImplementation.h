// #include "Matrix.h"

#include <QDebug>
#include <Eigen/Core>
#include <Eigen/Dense>


namespace Math
{
    template <int N, int M, typename T>
    Matrix<N, M, T>::Matrix()
    {
        setToZero();
    }

    template<int N, int M, typename T>
    void Matrix<N, M, T>::setToZero()
    {
        for (int i = 0; i < N; i++)
            for (int j = 0; j < M; j++)
                m[i][j] = 0.0f;
    }

    template<int N, int M, typename T>
    void Matrix<N, M, T>::fill(T value)
    {
        for (int i = 0; i < N; i++)
            for (int j = 0; j < M; j++)
                m[i][j] = value;
    }

    template<int N, int M, typename T>
    Matrix<M, N, T> Matrix<N, M, T>::transposed() const
    {
        Matrix<M, N, T> result;
        for (int i = 0; i < N; i++)
            for (int j = 0; j < M; j++)
                result.m[i][j] = m[i][j];
        return result;
    }

//    template<int N, int M, typename T>
//    Matrix<N, M, T>& Matrix<N, M, T>::replaceWith(int startRow, int endRow, int startCol, int endCol, )
//    {

//    }

//    template<int N, int M, typename T>
//    Matrix<N, M, T> Matrix<N, M, T>::inverse() const
//    {
//        Matrix<N, M, T> result();
//        for (int row = 0; row < M; ++row)
//            for (int col = 0; col < N; ++col)
//                result.m[row][col] = m[col][row];
//        return result;
//    }

    template <int N, int M, typename T>
    const T& Matrix<N, M, T>::operator()(int i, int j) const
    {
        Q_ASSERT(i >= 0 && i < N && j >= 0 && j < M);
        return m[i][j];
    }

    template <int N, int M, typename T>
    T& Matrix<N, M, T>::operator()(int i, int j)
    {
        Q_ASSERT(i >= 0 && i < N && j >= 0 && j < M);
        return m[i][j];
    }

    template<int N, int M, typename T>
    Matrix<N, M, T> &Matrix<N, M, T>::operator+=(const Matrix<N, M, T> &other)
    {
        for (int i = 0; i < N; i++)
            for (int j = 0; j < N; j++)
                m[i][j] += other.m[i][j];
        return *this;
    }

    template<int N, int M, typename T>
    Matrix<N, M, T> &Matrix<N, M, T>::operator-=(const Matrix<N, M, T> &other)
    {
        for (int i = 0; i < N; i++)
            for (int j = 0; j < N; j++)
                m[i][j] -= other.m[i][j];
        return *this;
    }

    template<int N, int M, typename T>
    Matrix<N, M, T> &Matrix<N, M, T>::operator*=(T factor)
    {
        for (int i = 0; i < N; i++)
            for (int j = 0; j < N; j++)
                m[i][j] += m[i][j]*factor;
        return *this;
    }

    template<int N, int M, typename T>
    Matrix<N, M, T> &Matrix<N, M, T>::operator/=(T divisor)
    {
        for (int i = 0; i < N; i++)
            for (int j = 0; j < M; j++)
                m[i][j] += m[i][j]/divisor;
        return *this;
    }

    template <int N, int M, typename T>
    void Matrix<N, M, T>::operator=(const Matrix<N, M, T>& rhs)
    {
        for (int i = 0; i < N; i++)
            for (int j = 0; j < M; j++)  {
                m[i][j] = rhs.m[i][j];
            }
        // return;
    }

    template <int N, int M, typename T>
    bool Matrix<N, M, T>::operator==(const Matrix<N, M, T>& other) const
    {
        for (int i = 0; i < N; i++)
            for (int j = 0; j < M; j++)  {
                if (m[i][j] != other.m[i][j])
                    return false;
            }
        return true;
    }

    template <int N, int M, typename T>
    bool Matrix<N, M, T>::operator!=(const Matrix<N, M, T>& other) const
    {
        for (int i = 0; j < N; i++)
            for (int j = 0; j < M; j++)  {
                if (m[i][j] != other.m[i][j])
                    return true;
            }
        return false;
    }

    template<int N, int M, typename T>
    Matrix<N, M, T> &Matrix<N, M, T>::operator+(const Matrix<N, M, T> &other)
    {
        Matrix<N, M, T> result;
        for (int i = 0; i < N; i++)
            for (int j = 0; j < M; j++)
                result.m[i][j] = m[i][j] + other.m[i][j];
        return result;
    }

    template<int N, int M, typename T>
    Matrix<N, M, T> &Matrix<N, M, T>::operator-(const Matrix<N, M, T> &other)
    {
        Matrix<N, M, T> result;
        for (int i = 0; i < N; i++)
            for (int j = 0; j < M; j++)
                result.m[i][j] = m[i][j] - other.m[i][j];
        return result;
    }

    template<int N, int M, typename T>
    Matrix<N, M, T> Matrix<N, M, T>::operator*(T factor)
    {
        Matrix<N, M, T> result;
        for (int i = 0; i < N; i++)
            for (int j = 0; j < M; j++)
                result.m[i][j] = m[i][j]*factor;
        return result;
    }

    template<int N, int M, typename T>
    Matrix<N, M, T> &Matrix<N, M, T>::operator/(T divisor)
    {
        Matrix<N, M, T> result;
        for (int i = 0; i < N; i++)
            for (int j = 0; j < M; j++)
                result.m[i][j] = m[i][j]/divisor;

        return result;
    }


    //- non-memeber function
    //- help functions

    template <int N, int M, typename T>
    QDebug operator<<(QDebug dbg, const Matrix<N, M, T> &m)
    {
        //QDebugStateSaver saver(dbg);
        dbg.nospace() << "Matrix<" << N << ", " << M
            << ", " << QTypeInfo<T>::name()
            << ">(" << endl << qSetFieldWidth(10);
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < M; j++)
                dbg << m(i, j);
            dbg << endl;
        }
        dbg << qSetFieldWidth(0) << ')';
        return dbg;
    }

    //- NxM and MxK multipication
    //- return a copy (small matrix is fine with it)
    template <int N, int M, int K, typename T>
    Matrix<N, K, T> operator*(const Matrix<N, M, T> &m1, const Matrix<M, K, T> &m2)
    {
        Matrix<N, K, T> result;
        for (int i = 0; i < N; i++)
            for (int j = 0; j < K; j++)
                for(int ii = 0; ii < M; ii++)
                {
                    result(i,j) += m1(i,ii)*m2(ii,j);
                }
        return result;
    }

    //- replace part of matrix (NxM) with another small matrix (PxQ)
    template <int N, int M, int P, int Q, typename T>
    void replace(Matrix<N, M, T> &m1, const Matrix<P, Q, T> &m2, int r1, int c1)
    {
        for (int i = r1; i < r1+P; i++)
            for (int j = c1; j < c1+Q; j++)
            {
                m1(i,j) = m2(i-r1,j-c1);
            }
    }

    //- homogenize a vector
    template <int N, typename T>
    Matrix<N+1, 1, T> homogenize(const Matrix<N, 1, T> &m)
    {
        Matrix<N+1, 1, T> result;
        for (int i = 0; i < N; i++)
        {
            result(i,0) = m(i,0);
        }

        result(N,0) = 1.0;

        return result;
    }

    //- deHomogenize a vector
    template <int N, typename T>
    Matrix<N-1, 1, T> deHomogenize(const Matrix<N, 1, T> &m)
    {
        Matrix<N-1, 1, T> result;
        for (int i = 0; i < N-1; i++)
        {
            result(i,0) = m(i,0)/m(N-1,0);
        }

        return result;
    }

    template <int N, typename T>
    void inverse(const Matrix<N, N, T> & input, Matrix<N, N, T> &output)
    {
        Eigen::MatrixXf m = Eigen::MatrixXf::Zero(N, N);

        for (int i = 0; i < N; i++)
            for (int j = 0; j < N; j++) {
                m(i,j) = input(i,j);
            }

        Eigen::MatrixXf inversed = m.inverse();

        for (int i = 0; i < N; i++)
            for (int j = 0; j < N; j++) {
                output(i,j) = inversed(i,j);
            }
    }

    /*
    //- find inverse of square matrix
    //- Function to get cofactor of A[p][q] in temp[][]. n is current
    //- dimension of A[][]
    template <int N, typename T>
    void getCofactor(T A[N][N], T temp[N][N], int p, int q, int n)
    {
        int i = 0, j = 0;

        //- Looping for each element of the matrix
        for (int row = 0; row < n; row++)
        {
            for (int col = 0; col < n; col++)
            {
                //- Copying into temporary matrix only those element
                //- which are not in given row and column
                if (row != p && col != q)
                {
                    temp[i][j++] = A[row][col];

                    //- Row is filled, so increase row index and
                    //- reset col index
                    if (j == n - 1)
                    {
                        j = 0;
                        i++;
                    }
                }
            }
        }
    }

    //- Recursive function for finding determinant of matrix.
    //-   n is current dimension of A[][].
    template <int N, typename T>
    T determinant(T A[N][N], int n)
    {
        //- Initialize result
        T D = 0;

        //- Base case : if matrix contains single element
        if (n == 1)
            return A[0][0];
        //- To store cofactors
        T temp[N][N];

        //- To store sign multiplier
        int sign = 1;

        //- Iterate for each element of first row
        for (int f = 0; f < n; f++)
        {
            //- Getting Cofactor of A[0][f]
            getCofactor<N,T>(A, temp, 0, f, n);
            D += sign * A[0][f] * determinant<N,T>(temp, n - 1);

            //- terms are to be added with alternate sign
            sign = -sign;
        }

        return D;
    }

    //- Function to get adjoint of A[N][N] in adj[N][N].
    template <int N, typename T>
    void adjoint(T A[N][N],T adj[N][N])
    {
        if (N == 1)
        {
            adj[0][0] = 1.0;
            return;
        }

        //- temp is used to store cofactors of A[][]
        int sign = 1;
        T temp[N][N];

        for (int i=0; i<N; i++)
        {
            for (int j=0; j<N; j++)
            {
                //- Get cofactor of A[i][j]
                getCofactor<N,T>(A, temp, i, j, N);

                //- sign of adj[j][i] positive if sum of row
                //- and column indexes is even.
                sign = ((i+j)%2==0)? 1: -1;

                //- Interchanging rows and columns to get the
                //- transpose of the cofactor matrix
                adj[j][i] = (sign)*(determinant<N,T>(temp, N-1));
            }
        }
    }

    //- Function to calculate and store the inverse, returns false if
    //- matrix is singular
    template <int N, typename T>
    bool inverse(const Matrix<N, N, T> & m, Matrix<N, N, T> &inversed)
    {
        T A[N][N];

        for(int i = 0; i < N; i++)
            for (int j = 0; j < N; j++)
            {
                A[i][j] = m(i,j);
            }

        //- Find determinant of A[][]
        T det = determinant<N,T>(A, N);
        if (det == 0)
        {
            qDebug() << "Singular matrix, can't find its inverse";
            return false;
        }

        //- Find adjoint
        T adj[N][N];
        adjoint<N,T>(A, adj);

        //- Find Inverse using formula "inverse(A) = adj(A)/det(A)"
        for (int i=0; i<N; i++)
            for (int j=0; j<N; j++)
                inversed(i,j) = adj[i][j]/T(det);

        return true;
    }
    */
}
