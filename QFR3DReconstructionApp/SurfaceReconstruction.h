#ifndef SURFACERECONSTRUCTION_H
#define SURFACERECONSTRUCTION_H

#include "ValidPointDetector.h"
#include "ValidPointGroup.h"
#include "Eigen/Dense"
#include <QList>

class SurfaceReconstruction
{

public:

//    SurfaceReconstruction();

    SurfaceReconstruction(\
            const Eigen::Vector3d &HL, \
            const Eigen::Vector3d &HR, \
            const ValidPointDetector &detector);

private:

    const Eigen::Vector3d &HL_;

    const Eigen::Vector3d &HR_;

    const ValidPointDetector &detector_;


public:

    void construct();

    void construct3DPoints(const CameraPinHole&camera, const QList<ValidPointGroup>&vpgl);
};

#endif // SURFACERECONSTRUCTION_H
