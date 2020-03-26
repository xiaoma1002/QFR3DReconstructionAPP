#ifndef VALIDPOINTDETECTOR_H
#define VALIDPOINTDETECTOR_H

#include <QList>
#include <ValidPointGroup.h>
#include "cameraPinHole.h"
#include "FindEpiloarIntersectionForPair.h"
#include "FindUniqueIntersectionLineAndCurve.h"
#include "Eigen/Dense"
#include "PixelUVImageXYTransformer.h"

class ValidPointDetector
{

public:

//    ValidPointDetector();

    ValidPointDetector(const CameraPinHole &camL, const CameraPinHole &camR);

private:

    const CameraPinHole &camL_;

    const CameraPinHole &camR_;

    // Eigen::Matrix3d &F_;

    QList<ValidPointGroup> validPointsL_;

    QList<ValidPointGroup> validPointsR_;

    void findValidPoints(const CameraPinHole&,
                          const CameraPinHole&,
                          const PixelCurve&,
                          const PixelCurve&,
                          PixelUVImageXYTransformer &,
                          PixelUVImageXYTransformer &,
                          const Eigen::Matrix3d&,
                          QList<ValidPointGroup>&);

public:

    void findValidPoints(const Eigen::Matrix3d &F12, const Eigen::Matrix3d &F21);

    inline const QList<ValidPointGroup> &validPointsL() const { return validPointsL_;}

    inline const QList<ValidPointGroup> &validPointsR() const { return validPointsR_;}

    inline const CameraPinHole & camL() const { return camL_; }

    //inline CameraPinHole & camL() { return camL_; }

    inline const CameraPinHole & camR() const { return camR_; }

    //inline CameraPinHole & camR() { return camR_; }
};

#endif // VALIDPOINTDETECTOR_H
