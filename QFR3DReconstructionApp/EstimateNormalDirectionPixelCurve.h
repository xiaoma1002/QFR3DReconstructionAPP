#ifndef ESTIMATENORMALDIRECTIONPIXELCURVE_H
#define ESTIMATENORMALDIRECTIONPIXELCURVE_H

#include "PixelCurve.h"
#include "Eigen/Dense"

class EstimateNormalDirectionPixelCurve
{

public:

    EstimateNormalDirectionPixelCurve(const PixelCurve & curve);

private:

    const PixelCurve & curve_;

    int stride_;

    QList<Eigen::Vector2d> direction_;

    QList<Eigen::Vector2d> orthNormal_;

    void estimate();

    void findDirection(int it,Eigen::Vector2d&,Eigen::Vector2d&);

    void setStride(const int stride);

public:

    inline const PixelCurve & curve() const { return curve_; }

    inline const int & stride() const { return stride_; }

    inline const QList<Eigen::Vector2d> &direction() const { return direction_; }

    inline const QList<Eigen::Vector2d> &orthNormal() const { return orthNormal_; }

    friend std::ostream& operator<<(std::ostream& os, const EstimateNormalDirectionPixelCurve&);


};

#endif // ESTIMATENORMALDIRECTIONPIXELCURVE_H
