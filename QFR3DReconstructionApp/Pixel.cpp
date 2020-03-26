#include "Pixel.h"

Pixel::Pixel()
{
    i_ = -9999;
    j_ = -9999;
}

Pixel::Pixel(const Pixel& p)
{
    i_ = p.i();
    j_ = p.j();
//    x_ = p.x();
//    y_ = p.y();
}

Pixel::Pixel(int i, int j)
{
    i_ = i;
    j_ = j;
}

double Pixel::distance(const Pixel &p) const
{
    double dist = 0.0;
    int dI = i_ - p.i();
    int dJ = j_ - p.j();
    dist = sqrt(dI*dI + dJ*dJ);
    return dist;
}

//Pixel::Pixel(double x, double y)
//{
//    x_ = x;
//    y_ = y;
//}

// A simplistic implementation of operator= (see better implementation below)
Pixel& Pixel::operator=(const Pixel &p)
{
    // do the copy
    i_ = p.i();
    j_ = p.j();
//    x_ = p.x();
//    y_ = p.y();

    return *this;
}

std::ostream& operator<<(std::ostream& os, const Pixel& pixel)
{
    os << pixel.i() << ',' << pixel.j() << std::endl;
    return os;
}
