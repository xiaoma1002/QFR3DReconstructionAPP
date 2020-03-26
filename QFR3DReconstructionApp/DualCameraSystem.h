#ifndef DUALCAMERASYSTEM_H
#define DUALCAMERASYSTEM_H

#include "cameraPinHole.h"
#include "ValidPointDetector.h"
#include "ValidPointGroup.h"
#include "Eigen/Dense"

class DualCameraSystem
{

public:

    DualCameraSystem(const ValidPointDetector&detector);

private:

    const CameraPinHole & camL_;

    const CameraPinHole & camR_;

    Eigen::Matrix3d Rrel12_;

    Eigen::Matrix3d Rrel21_;

    Eigen::Vector3d Trel12_;

    Eigen::Vector3d Trel21_;

    Eigen::Matrix3d F12_;

    Eigen::Matrix3d F21_;

    void CalculateRrel();

    void CalculateTrel();

    void ConstructFundamentalMatrix();

    //- engine

    const ValidPointDetector & validPointDetector_;

    const QList<ValidPointGroup> & validPointsL() const;

    const QList<ValidPointGroup> & validPointsR() const;

public:

    inline const Eigen::Matrix3d &Rrel12() const { return Rrel12_; }

    inline const Eigen::Matrix3d &Rrel21() const { return Rrel21_; }

    inline const Eigen::Vector3d &Trel12() const { return Trel12_; }

    inline const Eigen::Vector3d &Trel21() const { return Trel21_; }

    inline const Eigen::Matrix3d &F12() const { return F12_; }

    inline const Eigen::Matrix3d &F21() const { return F21_; }

    void surfaceReconstruction();

};

#endif // DUALCAMERASYSTEM_H
