#ifndef FINDUNIQUEINTERSECTIONLINEANDCURVE_H
#define FINDUNIQUEINTERSECTIONLINEANDCURVE_H

#include <QList>
#include "Pixel.h"
#include "Eigen/Dense"
#include "PixelCurve.h"

class FindUniqueIntersectionLineAndCurve
{

public:

    //FindUniqueIntersectionLineAndCurve();

    FindUniqueIntersectionLineAndCurve(const PixelCurve &curve);

private:

    const PixelCurve & curve_;

    Pixel inter_;

    //int istart_;

    QList<Pixel> allIntersections_;

    //- for debug
    QList<Pixel> heads_;

    //- for debug
    QList<Pixel> tails_;

    //- for debug
    QList<Pixel> cutline_;

    //- for debug
    QList<Pixel> allPixel_;

    //- for debug
    Eigen::Vector3d line_;

    const int tolerance_;

    //- the unique point has the shortest distance to
    //- the given point
    void pickUniquePixel(const Eigen::Vector2d& point);

    double computeShortestDistance(const Eigen::Vector2d& point);

    void draw();

public:

    inline const Pixel & inter() const { return inter_; }

    //inline const int & istart() const { return istart_; }

    inline const QList<Pixel> & allIntersections() const { return allIntersections_; }

    inline const QList<Pixel> & cutline() const { return cutline_; }

    inline const QList<Pixel> & allPixel() const { return allPixel_; }

    inline const QList<Pixel> & heads() const { return heads_; }

    inline const QList<Pixel> & tails() const { return tails_; }

    inline const int & tolerance() const { return tolerance_; }

    bool findIntersection(const Eigen::Vector3d & line, const Eigen::Vector2d &point, int drawing = 0);

};

#endif // FINDUNIQUEINTERSECTIONLINEANDCURVE_H
