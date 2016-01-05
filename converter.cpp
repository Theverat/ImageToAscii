#include "converter.h"

#include <QColor>

Converter::Converter()
{
}

QString Converter::convert(QImage &image, QString &lookupTable, bool coloredText) {
    QString result("");

    if(lookupTable.size() == 0 || lookupTable.isNull()) {
        return result;
    }

    for(int y = 0; y < image.height(); y += 2) {
        QRgb *scanline = (QRgb*) image.scanLine(y);

        for(int x = 0; x < image.width(); x++) {
            unsigned short brightnessRaw = qRed(scanline[x]) + qGreen(scanline[x]) + qBlue(scanline[x]);
            brightnessRaw /= 3;
            float brightness = (float)brightnessRaw / 255.f;

            int shade = (float)lookupTable.size() * brightness;

            if(shade < 0) {
                shade = 0;
            }
            else if(shade >= lookupTable.size()){
                shade = lookupTable.size() - 1;
            }

            if(coloredText) {
                result.append("<font color=\"");
                result.append(QColor(scanline[x]).name());
                result.append("\">");
                result.append(lookupTable.at(shade));
                result.append("</font>");
            }
            else {
                result.append(lookupTable.at(shade));
            }
        }

        result.append("<br>");
    }

    return result;
}
