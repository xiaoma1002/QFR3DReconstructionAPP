#include "FindUniqueIntersectionLineAndCurve.h"
#include <Eigen/Dense>
#include "PixelToLineDistanceMeasurer.h"
#include <QMessageBox>
#include "QFRDebugger.h"

//FindUniqueIntersectionLineAndCurve::FindUniqueIntersectionLineAndCurve()
//{

//}

FindUniqueIntersectionLineAndCurve::FindUniqueIntersectionLineAndCurve(const PixelCurve &curve):
    curve_(curve),
    tolerance_(200)
{
//    curve_ = curve;
//    istart_ = 0;
    allPixel_ = curve.curve();
}

bool FindUniqueIntersectionLineAndCurve::findIntersection(const Eigen::Vector3d &line,const Eigen::Vector2d &point, int drawing)
{
    line_ = line;
    allIntersections_.clear();

    const PixelCurve &curve = curve_;

    bool found = false;

    double a = line(0);
    double b = line(1);
    double c = line(2);

    Eigen::Vector3d norm(-b,a,0.0);

    int size = curve.curve().size();
    double xI = 99999.0;
    double yI = 99999.0;
    for (int i = 0; i < size; i++) {
        //- p0
        int index0 = i;
        double x0 = curve.curve()[index0].u();
        double y0 = curve.curve()[index0].v();

        //- p1
        int index1 = i+1;
        if (index1 < size) {
            bool parallel = false;
            double x1 = curve.curve()[index1].u();
            double y1 = curve.curve()[index1].v();

            Eigen::Vector3d norm1(x1 - x0, y1 - y0, 0.0);
            Eigen::Vector3d crossProduct = norm.cross(norm1);
            if (crossProduct.norm() == 0) {
                parallel = true;
                continue;
            }
            else {
                if (a == 0) {
                    if (y1 != y0) {
                        xI = -(b*((x1-x0)*y0-(y1-y0)*x0) + c*(x1-x0))/(b*(y1-y0)+a*(x1-x0));
                        yI = -(a*xI + c)/b;
                    }
                }
                if (b == 0) {
                    if (x1 != x0) {
                        yI = -(c*(y1-y0)-a*((x1-x0)*y0-(y1-y0)*x0))/(b*(y1-y0)+a*(x1-x0));
                        xI = -(b*yI+c)/a;
                    }
                }
                if (a != 0 && b != 0) {
                    xI = -(b*((x1-x0)*y0-(y1-y0)*x0) + c*(x1-x0))/(b*(y1-y0)+a*(x1-x0));
                    yI = -(a*xI + c)/b;
                }
            }

            if (((xI-x0)*(x1-xI) + (yI-y0)*(y1-yI) >= 0) && (!parallel)) {
                //found = true;
                int u = int(xI);
                int v = int(yI);
                allIntersections_.append(Pixel(v,u));

                heads_.append(curve.curve()[index0]);
                tails_.append(curve.curve()[index1]);
                //break;
            }
        }
    }

    if (allIntersections().size() > 0 )
    {
        double shortestDistance = computeShortestDistance(point);
        if (shortestDistance < tolerance_) {
            found = true;
        }
    }
    //- draw cut line
    if (drawing) draw();

    return found;
}

void FindUniqueIntersectionLineAndCurve::pickUniquePixel(const Eigen::Vector2d &point)
{
    double shortestDistance = 1.0e16;
    for (int i = 0; i < allIntersections().size(); i++) {
        double du = allIntersections()[i].u();
        double dv = allIntersections()[i].v();

        Eigen::Vector2d uv;
        uv(0) = du;
        uv(1) = dv;
        double dist = (uv-point).norm();

        //std::cout <<"pick unique pixel " <<  i << " " << du << " " << dv << " dist: " << dist << std::endl;

        if (dist < shortestDistance) {
            shortestDistance = dist;
            int u = du;
            int v = dv;
            const Pixel inter(v,u);
            inter_ = inter;
        }
    }

    if (shortestDistance > 200.0){

        double a = line_(0);
        double b = line_(1);
        double c = line_(2);

        QMessageBox messageBox;
        messageBox.critical(0,"Error"," incorrect edge intersection !");
        messageBox.setFixedSize(500,200);

        allPixel_.clear();
        allPixel_ = curve_.curve();
        cutline_.clear();

        std::cout << "filling the line" << std::endl;
        if (a==0) {
            for (int i = 0; i < 500; i++) {
                int u,v;

                u = i;
                v = -a*u-c;
                v = v/b;

                if (v<0 || v>=500) {
                    continue;
                }

                //std::cout <<"a==0 : "<< u << " " << v << std::endl;
                cutline_.append(Pixel(v,u));
                allPixel_.append(Pixel(v,u));
            }
        }

        if (b==0) {
            for (int i = 0; i < 500; i++) {
                int u,v;
                v = i;
                u = -b*v-c;
                u = u/a;

                if (u<0 || u>=500) {
                    continue;
                }

                //std::cout <<"b==0 : "<< u << " " << v << std::endl;
                cutline_.append(Pixel(v,u));
                allPixel_.append(Pixel(v,u));
            }
        }

        if (a!=0 && b!=0) {
            for (int i = 0; i < 500; i++) {
                int u,v;

                u = i;
                v = -a*u-c;
                v = v/b;

                if (v<0 || v>=500) {
                    continue;
                }

                //std::cout <<"a!=0 and b!=0 : "<< u << " " << v << std::endl;
                cutline_.append(Pixel(v,u));
                allPixel_.append(Pixel(v,u));
            }
        }

        QFRDebugger dbg;
        cv::Mat img = cv::Mat::zeros(cv::Size(500, 500), CV_8UC1);
        std::cout << "cutline: " << cutline().size() << " " << allIntersections().size() << std::endl;
        std::cout << "abc: " << a
                  << " " << b
                  << " " << c << std::endl;
        for (int i = 0; i < allIntersections().size(); i++) {
            std::cout << allIntersections()[i] << heads()[i] << tails()[i] << std::endl;
        }
        std::cout << point << " " << shortestDistance << std::endl;
        dbg.displayOverlay("cut edge",img,allPixel());
    }

}

double FindUniqueIntersectionLineAndCurve::computeShortestDistance(const Eigen::Vector2d &point)
{
    double shortestDistance = 1.0e16;
    for (int i = 0; i < allIntersections().size(); i++) {
        double du = allIntersections()[i].u();
        double dv = allIntersections()[i].v();

        Eigen::Vector2d uv;
        uv(0) = du;
        uv(1) = dv;
        double dist = (uv-point).norm();

        //std::cout <<"pick unique pixel " <<  i << " " << du << " " << dv << " dist: " << dist << std::endl;

        if (dist < shortestDistance) {
            shortestDistance = dist;
            int u = du;
            int v = dv;
            const Pixel inter(v,u);
            inter_ = inter;
        }
    }
    return shortestDistance;
}

void FindUniqueIntersectionLineAndCurve::draw()
{
    double a = line_(0);
    double b = line_(1);
    double c = line_(2);

    allPixel_.clear();
    allPixel_ = curve_.curve();
    cutline_.clear();

    std::cout << "filling the line" << std::endl;
    if (a==0) {
        for (int i = 0; i < 500; i++) {
            int ud,vd;

            ud = i;
            vd = -a*ud-c;
            vd = vd/b;

            int u = ud;
            int v = vd;

            if (v<0 || v>=500) {
                continue;
            }

            //std::cout <<"a==0 : "<< u << " " << v << std::endl;
            cutline_.append(Pixel(v,u));
            allPixel_.append(Pixel(v,u));
        }
    }

    if (b==0) {
        for (int i = 0; i < 500; i++) {
            int ud,vd;
            vd = i;
            ud = -b*vd-c;
            ud = ud/a;

            int u = ud;
            int v = vd;

            if (u<0 || u>=500) {
                continue;
            }

            //std::cout <<"b==0 : "<< u << " " << v << std::endl;
            cutline_.append(Pixel(v,u));
            allPixel_.append(Pixel(v,u));
        }
    }

    if (a!=0 && b!=0) {
        for (int i = 0; i < 500; i++) {
            double ud,vd;

            ud = i;
            vd = -a*ud-c;
            vd = vd/b;

            int u = ud;
            int v = vd;

            if (v<0 || v>=500) {
                continue;
            }

            //std::cout <<"a!=0 and b!=0 : "<< u << " " << v << std::endl;
            cutline_.append(Pixel(v,u));
            allPixel_.append(Pixel(v,u));
        }
    }

    QFRDebugger dbg;
    cv::Mat img = cv::Mat::zeros(cv::Size(500, 500), CV_8UC1);
    std::cout << "cutline: " << cutline().size() << " " << allIntersections().size() << std::endl;
    std::cout << "abc: " << a
              << " " << b
              << " " << c << std::endl;
    for (int i = 0; i < allIntersections().size(); i++) {
        std::cout << allIntersections()[i] << heads()[i] << tails()[i] << std::endl;
    }

    dbg.displayOverlay("cut edge",img,allPixel());
}
