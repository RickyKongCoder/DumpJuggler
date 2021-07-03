#ifndef PENCOLOR_H
#define PENCOLOR_H
#include "stdint.h"
#include <QColor>
#include <QDebug>
#include <QList>
#include <QRandomGenerator>
#include <QTime>
#define PenColors(X) \
    X(ff52d5) \
    X(e3e6ff) \
    X(ffe6f7) \
    X(c901ed) \
    X(5f2b8f) \
    X(99f8ff) \
    X(db2c2c) \
    X(ffdd00) \
    X(97cdff) \
    X(ffa739) \
    X(ff8cfa) \
    X(cfd3ff) \
    X(aaf661) \
    X(98ff98) \
    X(fabc4d) \
    X(fe8cff) \
    X(40777c) \
    X(08c7c9) \
    X(08c96b) \
    X(eee3ff) \
    X(bad2c5) \
    X(fffef4) \
    X(5fbc9a) \
    X(a64837) \
    X(861c28) \
    X(e20039) \
    X(fc0040) \
    X(090206) \
    X(011105) \
    X(f36c1f) \
    X(bf73ff) \
    X(4181f5) \
    X(8685ff)
#define ColorHexToName(hex) Color_##hex,
#define ColorsHex(hex) 0x##hex,
#define PenColorNames PenColors(ColorHexToName)
#define PenColorHex PenColors(ColorsHex)
typedef enum { PenColorNames ColorNum } BrightColor;
extern unsigned int pencolor[];
QRgba64 randomBrightColor(quint8, quint32);
#endif // PENCOLOR_H
