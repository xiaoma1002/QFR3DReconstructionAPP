#ifndef CROSSSECTIONPROFILE_H
#define CROSSSECTIONPROFILE_H

#include<QList>

class CrossSectionProfile
{

public:

    CrossSectionProfile();

private:

    QList<double> x_;

    QList<double> y_;

public:

    inline QList<double> & x() { return x_;}

    inline QList<double> & y() { return y_;}

    inline const QList<double> & x() const { return x_;}

    inline const QList<double> & y() const { return y_;}

};

#endif // CROSSSECTIONPROFILE_H
