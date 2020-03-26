#ifndef FINDEPILOARINTERSECTIONFORPAIR_H
#define FINDEPILOARINTERSECTIONFORPAIR_H

#include "Pixel.h"
//#include "matrixdeclarations.h"
#include "cameraPinHole.h"
#include "Eigen/Dense"
#include <QList>
#include "PixelCurve.h"

class FindEpiloarIntersectionForPair
{

public:

//    FindEpiloarIntersectionForPair();

    FindEpiloarIntersectionForPair(const CameraPinHole&,\
                                   const Eigen::Matrix3d&,
                                   const PixelCurve&);

private:

    const CameraPinHole &cam_;

    const Eigen::Matrix3d &F_;

    const PixelCurve &centerline_;

    QList<Pixel> epipolarLine_;

    QList<Pixel> searchingArea_;

    Eigen::Vector3d epipolarInPixel_;

    Eigen::Vector3d epipolarInImage_;

    //- next find starts from istart_
    //- to ensure unique intersection
    int istart_;

public:

    bool find(const Eigen::Vector3d &x, Pixel &intersection);

    inline const QList<Pixel> & epipolarLine() const { return epipolarLine_; }

    inline const Eigen::Vector3d & epipolarInPixel() const { return epipolarInPixel_; }

    inline const Eigen::Vector3d & epipolarInImage() const { return epipolarInImage_; }

    inline const QList<Pixel> & searchingArea() const { return searchingArea_; }
};

#endif // FINDEPILOARINTERSECTIONFORPAIR_H
