#include "graphicsview.h"
#include <QMimeData>
#include <QDropEvent>

GraphicsView::GraphicsView(QGraphicsScene *scene, QWidget *parent) : QGraphicsView(scene, parent)
{
}

GraphicsView::GraphicsView(QWidget *parent) : QGraphicsView(parent)
{
}

void GraphicsView::dragEnterEvent(QDragEnterEvent* event) {
    QGraphicsView::dragEnterEvent(event);
}

void GraphicsView::dragMoveEvent(QDragMoveEvent* event) {
    QGraphicsView::dragMoveEvent(event);
}

void GraphicsView::dropEvent(QDropEvent* event) {
    QGraphicsView::dropEvent(event);

    if(event->mimeData()->hasUrls()) {
        QList<QUrl> urls = event->mimeData()->urls();
        if(urls.size() == 1) {
            emit singleImageDropped(urls.at(0));
        }
    }
}

void GraphicsView::mouseReleaseEvent(QMouseEvent *event) {
    QGraphicsView::mouseReleaseEvent(event);

    if(event->button() == Qt::RightButton) {
        emit rightClick();
    }
    else if(event->button() == Qt::MiddleButton) {
        emit middleClick();
    }
}

void GraphicsView::wheelEvent(QWheelEvent *event) {
    if(event->delta() > 0) {
        emit zoomIn();
    }
    else {
        emit zoomOut();
    }
}
