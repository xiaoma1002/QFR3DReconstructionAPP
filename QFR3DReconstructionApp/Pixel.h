#ifndef PIXEL_H
#define PIXEL_H

#include <iostream>

//-

class Pixel
{
public:
    Pixel();

    Pixel(const Pixel &);

    //- construct with i,j (row, column)
    //- (i,j) <-> (u,v)
    Pixel(int i, int j);

    //- x,y
    // Pixel(double x, double y);

private:

    int i_;

    int j_;

    //- for intersection
    // double x_;

    // double y_;

public:

    inline const int i() const { return i_; }

    inline const int j() const { return j_; }

    inline const int u() const { return j_; }

    inline const int v() const { return i_; }

    double distance(const Pixel&) const;

    // inline const double x() const { return x_; }

    // inline const double y() const { return y_; }

    Pixel &operator=(const Pixel &p);

    friend std::ostream& operator<<(std::ostream& os, const Pixel& pixel);
};


#endif // PIXEL_H
