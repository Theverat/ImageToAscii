#ifndef GRAPHICSVIEW_H
#define GRAPHICSVIEW_H

#include <QGraphicsView>
#include <QUrl>

class GraphicsView : public QGraphicsView
{
    Q_OBJECT

public:
    GraphicsView(QGraphicsScene *scene, QWidget *parent = 0);
    GraphicsView(QWidget *parent = 0);
    void dragEnterEvent(QDragEnterEvent* event);
    void dragMoveEvent(QDragMoveEvent* event);
    void dropEvent(QDropEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);
    void wheelEvent(QWheelEvent *event);

signals:
    void singleImageDropped(QUrl url);
    void rightClick();
    void middleClick();
    void zoomIn();
    void zoomOut();
};

#endif // GRAPHICSVIEW_H
