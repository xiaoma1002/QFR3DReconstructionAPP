#ifndef VALIDPOINTDETECTOR2_H
#define VALIDPOINTDETECTOR2_H

#include "cameraPinHole.h"
#include "ValidPointDetector2.h"
#include "ValidPointGroup2.h"
#include <QList>

class ValidPointDetector2
{

public:

    ValidPointDetector2(const CameraPinHole &camL, const CameraPinHole &camR);

private:

    const CameraPinHole &camL_;

    const CameraPinHole &camR_;

    // Eigen::Matrix3d &F_;

    QList<ValidPointGroup2> validPoints_;

public:

    void findValidPoints(const Eigen::Matrix3d &F12);

    inline const QList<ValidPointGroup2> &validPoints() const { return validPoints_; }

    inline const CameraPinHole & camL() const { return camL_; }

    inline const CameraPinHole & camR() const { return camR_; }
};

#endif // VALIDPOINTDETECTOR2_H
