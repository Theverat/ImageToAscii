#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QUrl>

#include "converter.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QImage input;
    Converter converter;
    QColor backgroundColor;

    QImage scaleImage(QImage image);
    void fitInView();

public slots:
    void loadImage(QUrl url);
    void calculate();
    void setFontSize(int value);
    void setBackgroundColor(QColor color);
    void showBackgroundColorDialog();
    void toggleFullscreen();
};

#endif // MAINWINDOW_H
