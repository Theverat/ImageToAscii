#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "graphicsscene.h"

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
    connect(ui->spinBox_fontSize, SIGNAL(valueChanged(int)), this, SLOT(setFontSize(int)));
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

    if(ui->checkBox_scale->isChecked()) {
        inputImage = scaleImage(this->input);
    }

    QString lookupTable = ui->lineEdit_lookupTable->text();
    QString result = this->converter.convert(inputImage, lookupTable);
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
