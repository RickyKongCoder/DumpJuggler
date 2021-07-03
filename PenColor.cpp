#include "PenColor.h"

unsigned int pencolor[] = {PenColorHex};
/*Generate random birght color from bright color table*/
QRgba64 randomBrightColor(quint8 transparency, quint32 seed_offset)
{
    QRandomGenerator generator;
    generator.seed(QTime::currentTime().hour() * QTime::currentTime().hour()
                   + QTime::currentTime().minute() + QTime::currentTime().second() + seed_offset);
    quint32 color_index = generator.bounded(0, ColorNum - 1);
    ;
    qDebug() << color_index << endl;
    uint32_t rgb = pencolor[color_index];
    qDebug() << rgb << endl;
    return QRgba64::fromRgba(quint8(rgb >> 16), quint8(rgb >> 8), quint8(rgb), transparency);
}
