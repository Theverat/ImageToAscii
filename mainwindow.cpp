#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "graphicsscene.h"

#include <QColorDialog>
#include <QShortcut>
#include <QApplication>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Initialize graphicsview
    GraphicsScene *scene = new GraphicsScene();
    ui->graphicsView_inputImage->setScene(scene);
    scene->setBackgroundBrush(QBrush(Qt::gray));
    scene->addText("Drop image here.");
    ui->graphicsView_inputImage->setDragMode(QGraphicsView::ScrollHandDrag);
    ui->graphicsView_inputImage->setAcceptDrops(true);

    // Initialize converter
    this->converter = Converter();

    setBackgroundColor(QColor(Qt::white));

    // Initialize fonts
    QFont outputFont("Monospace", ui->spinBox_fontSize->value());
    ui->textBrowser_outputText->setFont(outputFont);
    QFont lineEditFont("Monospace");
    ui->lineEdit_lookupTable->setFont(lineEditFont);

    // Connect signal/slots
    connect(ui->graphicsView_inputImage, SIGNAL(singleImageDropped(QUrl)), this, SLOT(loadImage(QUrl)));
    connect(ui->lineEdit_lookupTable, SIGNAL(textChanged(QString)), this, SLOT(calculate()));
    connect(ui->checkBox_scale, SIGNAL(clicked()), this, SLOT(calculate()));
    connect(ui->spinBox_height, SIGNAL(valueChanged(int)), this, SLOT(calculate()));
    connect(ui->spinBox_width, SIGNAL(valueChanged(int)), this, SLOT(calculate()));
    connect(ui->checkBox_coloredText, SIGNAL(clicked()), this, SLOT(calculate()));
    connect(ui->spinBox_fontSize, SIGNAL(valueChanged(int)), this, SLOT(setFontSize(int)));
    connect(ui->pushButton_background_color, SIGNAL(clicked()), this, SLOT(showBackgroundColorDialog()));
    connect(ui->pushButton_fullscreen, SIGNAL(clicked()), this, SLOT(toggleFullscreen()));

    QShortcut *fullscreen = new QShortcut(QKeySequence("Esc"), ui->textBrowser_outputText);
    connect(fullscreen, SIGNAL(activated()), this, SLOT(toggleFullscreen()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::loadImage(QUrl url) {
    QGraphicsScene *scene = ui->graphicsView_inputImage->scene();
    scene->clear();

    this->input = QImage(url.toLocalFile());

    if(input.isNull()) {
        scene->addText("Could not load image\n" + url.toLocalFile());
    }
    else {
        scene->addPixmap(QPixmap::fromImage(this->input));
        fitInView();
        calculate();
    }
}

void MainWindow::calculate() {
    QImage inputImage = this->input;

    if(ui->checkBox_scale->isChecked() && !inputImage.isNull()) {
        inputImage = scaleImage(this->input);
    }

    QString lookupTable = ui->lineEdit_lookupTable->text();
    QString result = this->converter.convert(inputImage, lookupTable, ui->checkBox_coloredText->isChecked());
    ui->textBrowser_outputText->setText(result);
}

void MainWindow::setFontSize(int value) {
    ui->textBrowser_outputText->setFont(QFont("Monospace", value));
}

QImage MainWindow::scaleImage(QImage image) {
    int targetWidth = ui->spinBox_width->value();
    int targetHeight = ui->spinBox_height->value();

    return image.scaled(targetWidth, targetHeight, Qt::KeepAspectRatio);
}

void MainWindow::fitInView() {
   QGraphicsScene *scene = ui->graphicsView_inputImage->scene();
   ui->graphicsView_inputImage->scene()->setSceneRect(QRectF(0, 0, input.width(), input.height()));
   ui->graphicsView_inputImage->setSceneRect(scene->sceneRect());
   ui->graphicsView_inputImage->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);
}

void MainWindow::showBackgroundColorDialog() {
    QColorDialog *colorDialog = new QColorDialog(backgroundColor, this);
    colorDialog->setWindowFlags(Qt::SubWindow);
    connect(colorDialog, SIGNAL(currentColorChanged(QColor)), this, SLOT(setBackgroundColor(QColor)));
    colorDialog->show();
}

void MainWindow::setBackgroundColor(QColor color) {
    this->backgroundColor = color;
    QString css = "background-color: " + QString(color.name()) + ";";
    ui->textBrowser_outputText->setStyleSheet(css);
}

void MainWindow::toggleFullscreen() {
    if(ui->textBrowser_outputText->isFullScreen()) {
        ui->textBrowser_outputText->setParent(this);
        ui->textBrowser_outputText->setFrameShape(QFrame::StyledPanel);
        ui->textBrowser_outputText->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        ui->textBrowser_outputText->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        ui->gridLayout->addWidget(ui->textBrowser_outputText, 0, 1);

        QApplication::restoreOverrideCursor();
    }
    else {
        ui->textBrowser_outputText->setParent(0);
        ui->textBrowser_outputText->setFrameShape(QFrame::NoFrame);
        ui->textBrowser_outputText->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        ui->textBrowser_outputText->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        ui->textBrowser_outputText->showFullScreen();

        QApplication::setOverrideCursor(Qt::BlankCursor);
    }
}
