#ifndef SORTMATRIX_H
#define SORTMATRIX_H

#include<iostream>
#include<vector>

using namespace std;

namespace Math {

    // Driver function to sort the 2D vector
    // on basis of a particular column
    bool sortcol( const vector<double>& v1, const vector<double>& v2) {
        return v1[0] < v2[0];
    }


}

#endif // SORTMATRIX_H
