#include "FindEpiloarIntersectionForPair.h"
#include "QFRDebugger.h"
#include "PixelUVImageXYTransformer.h"

//FindEpiloarIntersectionForPair::FindEpiloarIntersectionForPair()
//{

//}

FindEpiloarIntersectionForPair::FindEpiloarIntersectionForPair(const CameraPinHole& cam,\
        const Eigen::Matrix3d& F,\
        const PixelCurve & centerline):
    cam_(cam),
    F_(F),
    centerline_(centerline)
{
//    cam_ = cam;

//    F_ = F;

//    centerline_ = centerline;

    QList<Pixel> epipolarLine;

    epipolarLine_ = epipolarLine;

    istart_ = 0;
}

bool FindEpiloarIntersectionForPair::find(const Eigen::Vector3d &x, Pixel & intersection)
{

    const PixelCurve & centerline = centerline_;

    int size = centerline.curve().size();

    searchingArea_ = epipolarLine_;

    for (int i = istart_; i < size; i++) {
        searchingArea_.append(centerline.curve()[i]);
    }
    //QFRDebugger dbg;

    const int & uMax = cam_.resolutionU();
    const int & vMax = cam_.resolutionV();

    //-int m, n;
    // const int &m = cam_.resolutionV();
    const int &n = cam_.resolutionU();

    Eigen::Vector3d line;
    line = F_*x;

    double a = line(0);
    double b = line(1);
    double c = line(2);

    epipolarInImage_(0) = a;
    epipolarInImage_(1) = b;
    epipolarInImage_(2) = c;

    QList<double> t;
    QList<double> lt;

    int ndivision = 100;

    PixelUVImageXYTransformer coordTransformer(uMax,vMax);

    double delta = n/float(ndivision);

    for(int i = 0; i < ndivision; i++)
    {
        double tt = i*delta;
        t.append(tt);
        //- image coordinate system
        double value = (c+a*tt)/(-b);
        double xx = tt;
        double yy = value;

        if (yy <= 0 || yy >= vMax) {
            continue;
        }

        int u = coordTransformer.PixelU(xx);
        int v = coordTransformer.PixelV(yy);
        epipolarLine_.append(Pixel(v,u));

        lt.append(value);
    }

    a = a/b;
    b = vMax + c/b;

    //- y=ax+b => ax-y+b = 0;
    epipolarInPixel_(0) = a;
    epipolarInPixel_(1) = -1.;
    epipolarInPixel_(2) = b;

    bool found = false;

    std::cout <<"start index : " << istart_ << std::endl;
    std::cout <<"a,b,c: " << a <<" " << b <<" " << c << std::endl;
    for (int i = istart_; i < size; i++) {

        //- P0
        double x0 = centerline.curve()[i].u();
        double y0 = centerline.curve()[i].v();

        if (i+1 == size) {
            break;
        }

        //- P1
        double x1 = centerline.curve()[i+1].u();
        double y1 = centerline.curve()[i+1].v();

        //- P2
        double xI = ((b-y0)*(x1-x0) + x0*(y1-y0))/((y1-y0)-a*(x1-x0));
        double yI = a*xI + b;

        double test = (xI-x0)*(x1-xI) + (yI-y0)*(y1-yI);

        if(0) {
            std::cout <<"["<< x0 <<" " << y0
                      <<"] \t[" << x1 << " " << y1
                      <<"] \t[" << xI << " " << yI << "] \t"
                      << test << " " << i << std::endl;
        }

        //- test if (xI, yI) stays within P0P1
        if (test >= 0) {
            int u = xI;
            int v = yI;
            intersection = Pixel(v,u);
            istart_ = i;

//            if (istart_ < 0) {
//                istart_ = 0;
//            }

            found = true;
            break;
        } else {
//            if (fabs(test) < 1.0) {
//                //double distSq0 = (xI-x0)*(xI-x0)+(yI-y0)*(yI-y0);
//                //double distSq1 = (xI-x1)*(xI-x1)+(yI-y1)*(yI-y1);
//                int u = xI;
//                int v = yI;
//                intersection = Pixel(v,u);
//                istart_ = i;
//                found = true;
//                break;
//            }
        }
    }

    return found;
}
