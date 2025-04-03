#include "mypictureview.h"
#include <QPainter>
#include <QMessageBox>
#include <QThread>
#include <QRunnable>
#include <QThreadPool>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMimeData>

class ImageScaler : public QRunnable
{
public:
    ImageScaler(QPixmap *pixmap, QSize size, QPixmap *result)
        : m_pixmap(pixmap), m_size(size), m_result(result) {}

    void run() override
    {
        *m_result = m_pixmap->scaled(m_size, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    }

private:
    QPixmap *m_pixmap;
    QSize m_size;
    QPixmap *m_result;
};

MyPictureView::MyPictureView(QWidget *parent)
    : QWidget{parent}
{
    m_pixmap = nullptr;
    m_zoom = 1.0;
    m_mouseInTheWgt = false;
    m_isDragging = false;
    m_lastMousePos = QPoint(0, 0);
    m_offset = QPoint(0, 0);
}

void MyPictureView::setPicture(QPixmap* picture)
{
    m_pixmap = picture;
    m_zoom = 1.0;
    m_offset = QPoint(0, 0);
    setAcceptDrops(true);
    update();
}

void MyPictureView::paintEvent(QPaintEvent *event)
{
    if (m_pixmap == nullptr) {
        return;
    }
    QPainter painter(this);

    QSize scaledSize(m_pixmap->width() * m_zoom, m_pixmap->height() * m_zoom);
    if (m_scaledPixmap.size() != scaledSize) {
        ImageScaler *scaler = new ImageScaler(m_pixmap, scaledSize, &m_scaledPixmap);
        QThreadPool::globalInstance()->start(scaler);
        QThreadPool::globalInstance()->waitForDone();
    }

    painter.drawPixmap(m_offset, m_scaledPixmap);
}

void MyPictureView::enterEvent(QEnterEvent *event)
{
    m_mouseInTheWgt = true;
}

void MyPictureView::leaveEvent(QEvent *event)
{
    m_mouseInTheWgt = false;
}

void MyPictureView::dragEnterEvent(QDragEnterEvent *event)
{
    event->acceptProposedAction();
}

void MyPictureView::dropEvent(QDropEvent *event)
{
    const QMimeData *mimeData = event->mimeData();
    if (mimeData->hasUrls()) {
        QList<QUrl> urls = mimeData->urls();
        if (!urls.isEmpty()) {
            QString filePath = urls.first().toLocalFile();
            // 这里可以添加加载图片的逻辑
        }
    }
    event->acceptProposedAction();
}

void MyPictureView::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        m_isDragging = true;
        m_lastMousePos = event->pos();
    }
}

void MyPictureView::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        m_isDragging = false;
    }
}

void MyPictureView::mouseMoveEvent(QMouseEvent *event)
{
    if (m_isDragging) {
        QPoint delta = event->pos() - m_lastMousePos;
        m_offset += delta;
        m_lastMousePos = event->pos();
        update();
    }
}

void MyPictureView::wheelEvent(QWheelEvent *event)
{
    if (m_mouseInTheWgt) {
        double zoomFactor = 1.1;
        QPointF mousePos = event->position();

        double oldZoom = m_zoom;
        if (event->angleDelta().y() > 0) {
            if (m_zoom * zoomFactor <= Zoom_Max) {
                m_zoom *= zoomFactor;
            }
        } else {
            if (m_zoom / zoomFactor >= Zoom_Min) {
                m_zoom /= zoomFactor;
            }
        }

        QPointF oldRelativePos = (mousePos - m_offset) / oldZoom;
        QPointF newOffset = mousePos - oldRelativePos * m_zoom;
        m_offset = newOffset.toPoint();

        update();
    }
}
