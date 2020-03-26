#ifndef SURFACERECONSTRUCTION2_H
#define SURFACERECONSTRUCTION2_H

//- find the best fit ellipse to approximate
//- the cross section of the vessel
//- the input : a set of 3D points
//- each set has (center,E11,E21,E12,E22)

#include "ValidPointDetector2.h"
#include "ValidPointGroup2.h"
#include "CrossSectionProfile.h"

class SurfaceReconstruction2
{

public:

    SurfaceReconstruction2(const ValidPointDetector2 &detector);

private:

    const ValidPointDetector2 & detector_;

    int nPoints_;

    double dGamma_;

    QList<CrossSectionProfile> crossSections_;

    QList<Eigen::Vector3d> crossSectionsWorld_;

public:

    inline const int &nPoints() const { return nPoints_; }

    inline const double &dGamma() const { return dGamma_; }

    inline const QList<CrossSectionProfile> &crossSections() const { return crossSections_; }

    inline const QList<Eigen::Vector3d> &crossSectionsWorld() const { return crossSectionsWorld_; }

    void setNPoints(int n);

    void write();

};

#endif // SURFACERECONSTRUCTION2_H
