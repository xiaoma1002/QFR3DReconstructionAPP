#ifndef FINDEDGEPOINTFORCAMERA_H
#define FINDEDGEPOINTFORCAMERA_H

#include "cameraPinHole.h"
#include "Pixel.h"

class FindEdgePointForCamera
{

public:

    FindEdgePointForCamera(const CameraPinHole &);

private:

    const CameraPinHole & camera_;

public:

    inline const CameraPinHole& camera() const { return camera_; }

    Eigen::Vector3d construct(const Pixel &e1,
              const Pixel &e2,
              const Eigen::Vector3d& xWorld,
              Eigen::Vector3d& E1w,
              Eigen::Vector3d& E2w);

    void construct2(const Pixel &e1,
              const Pixel &e2,
              const Eigen::Vector3d& xWorld,
              const Eigen::Vector3d& normal,
              Eigen::Vector3d& E1w,
              Eigen::Vector3d& E2w);
};

#endif // FINDEDGEPOINTFORCAMERA_H
