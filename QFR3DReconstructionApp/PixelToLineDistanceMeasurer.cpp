#include "PixelToLineDistanceMeasurer.h"

PixelToLineDistanceMeasurer::PixelToLineDistanceMeasurer(const Eigen::Vector3d &line, const QList<Pixel> &pixelList):
    line_(line),
    pixelList_(pixelList)
{
    nearestPixel_ = Pixel(-1,-1);
    farthestPixel_ = Pixel(-1,-1);
    findThePixels();
}

void PixelToLineDistanceMeasurer::findThePixels()
{
    const int size = pixelList_.size();

    double shortestDistance = 1.0e16;
    double longestDistance = -1.0e16;

    for (int i = 0; i < size; i++) {

        //- calculate the distance to the line
        //- line is defined as ax+by+c = 0
        //- a = line[0]
        //- b = line[1]
        //- c = line[2]

        const Pixel &p = pixelList_[i];

        double a = line_[0];
        double b = line_[1];
        double c = line_[2];

        double u = p.u();
        double v = p.v();

        double distance = fabs(a*u+b*v+c)/sqrt(a*a+b*b);

        if (distance < shortestDistance) {
            shortestDistance = distance;
            nearestPixel_ = p;
        }

        if (distance > longestDistance) {
            longestDistance = distance;
            farthestPixel_ = p;
        }
    }
}
