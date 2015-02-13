#ifndef CONVERTER_H
#define CONVERTER_H

#include <QString>
#include <QImage>

class Converter
{
public:
    Converter();
    QString convert(QImage &image, QString &lookupTable);
};

#endif // CONVERTER_H
