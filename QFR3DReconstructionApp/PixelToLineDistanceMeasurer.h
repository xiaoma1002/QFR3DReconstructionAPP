#ifndef NEARESTPIXELTOLINE_H
#define NEARESTPIXELTOLINE_H

#include "Pixel.h"
#include "Eigen/Dense"
#include "QList"

class PixelToLineDistanceMeasurer
{

public:

    PixelToLineDistanceMeasurer(const Eigen::Vector3d &line, const QList<Pixel> &pixelList);

private:

    //- all in o-u-v pixel coordinate system

    const Eigen::Vector3d & line_;

    const QList<Pixel> & pixelList_;

    Pixel nearestPixel_;

    Pixel farthestPixel_;

    void findThePixels();

public:

    inline const Pixel & nearestPixel() const { return nearestPixel_; }

    inline const Pixel & farthestPixel() const { return farthestPixel_; }

};

#endif // NEARESTPIXELTOLINE_H
