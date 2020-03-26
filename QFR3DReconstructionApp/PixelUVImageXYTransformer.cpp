#include "PixelUVImageXYTransformer.h"

//PixelUVImageXYTransformer::PixelUVImageXYTransformer()
//{

//}

PixelUVImageXYTransformer::PixelUVImageXYTransformer(int uMax, int vMax)
{
    uMax_ = uMax;
    vMax_ = vMax;
}

int PixelUVImageXYTransformer::imagePointX(int u)
{
    return u;
}

int PixelUVImageXYTransformer::imagePointY(int v)
{
    return vMax_ - v;
}

int PixelUVImageXYTransformer::PixelU(int imageX)
{
    return imageX;
}

int PixelUVImageXYTransformer::PixelV(int imageY)
{
    return vMax_ - imageY;
}

int PixelUVImageXYTransformer::imageX2CameraPointX(int imageX)
{
    return int(imageX-0.5*uMax_);
}

int PixelUVImageXYTransformer::imageY2CameraPointY(int imageY)
{
    return int(imageY-0.5*vMax_);
}

int PixelUVImageXYTransformer::cameraPointX2ImageX(int cameraX)
{
    return int(cameraX+0.5*uMax_);
}

int PixelUVImageXYTransformer::cameraPointY2ImageY(int cameraY)
{
    return int(cameraY+0.5*vMax_);
}
