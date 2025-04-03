#ifndef MYPICTUREVIEW_H
#define MYPICTUREVIEW_H

#include <QWidget>
#include <QPixmap>
#include <QRect>

const double Zoom_Max = 5.0;
const double Zoom_Min = 0.1;

class MyPictureView : public QWidget
{
    Q_OBJECT
public:
    explicit MyPictureView(QWidget *parent = nullptr);
    void setPicture(QPixmap* picture);

protected:
    virtual void paintEvent(QPaintEvent *event) override;
    virtual void enterEvent(QEnterEvent *event) override;
    virtual void leaveEvent(QEvent *event) override;
    virtual void dragEnterEvent(QDragEnterEvent *event) override;
    virtual void dropEvent(QDropEvent *event) override;
    virtual void mouseMoveEvent(QMouseEvent *event) override;
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseReleaseEvent(QMouseEvent *event) override;
    virtual void wheelEvent(QWheelEvent *event) override;

private:
    QPixmap *m_pixmap;
    double m_zoom;
    bool m_mouseInTheWgt;
    bool m_isDragging;
    QPoint m_lastMousePos;
    QPoint m_offset;
    QPixmap m_scaledPixmap;  // 缓存缩放后的图片
};

#endif // MYPICTUREVIEW_H
