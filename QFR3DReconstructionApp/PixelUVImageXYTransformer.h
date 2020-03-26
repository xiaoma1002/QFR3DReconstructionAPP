#ifndef PIXELUXIMAGEXYTRANSFORMER_H
#define PIXELUXIMAGEXYTRANSFORMER_H


class PixelUVImageXYTransformer
{

public:

//    PixelUVImageXYTransformer();

    PixelUVImageXYTransformer(int uMax, int vMax);

private:

    int uMax_;

    int vMax_;

public:

    int imagePointX(int u);

    int imagePointY(int v);

    int PixelU(int imageX);

    int PixelV(int imageY);

    int imageX2CameraPointX(int imageX);

    int imageY2CameraPointY(int imageY);

    int cameraPointX2ImageX(int cameraX);

    int cameraPointY2ImageY(int cameraY);
};

#endif // PIXELUVIMAGEXYTRANSFORMER_H
