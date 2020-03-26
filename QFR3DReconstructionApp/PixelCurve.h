#ifndef PIXELCURVE_H
#define PIXELCURVE_H

#include "Pixel.h"
#include <QList>

//- pixel curve is a list of pixel
//- with head and tail

class PixelCurve
{

public:

    PixelCurve();

private:

    QList<Pixel> curve_;

    Pixel head_;

    Pixel headNext_;

    Pixel tail_;

    Pixel tailNext_;

public:

    void setCurve(const QList<Pixel> & curve);

    void setHead(const Pixel & head);

    void setHeadNext(const Pixel & headNext);

    void setTail(const Pixel & tail);

    void setTailNext(const Pixel & TailNext);

    const QList<Pixel> &curve() const;

    const Pixel &head() const;

    const Pixel &headNext() const;

    const Pixel &tail() const;

    const Pixel &tailNext() const;

    PixelCurve &operator=(const PixelCurve &p);

    friend std::ostream& operator<<(std::ostream& os, const PixelCurve& pixelCurve);
};

#endif // PIXELCURVE_H
