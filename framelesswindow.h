#ifndef FRAMELESSWINDOW_H
#define FRAMELESSWINDOW_H

#include<QQuickWindow>

class FramelessWindow : public QQuickWindow
{
    Q_OBJECT

    enum MousePosition{
        TOPLEFT = 1, TOP, TOPRIGHT, LEFT, RIGHT, BOTTOMLEFT, BOTTOM, BOTTOMRIGHT, NORMAL
    };

public:
    FramelessWindow(QWindow * parent = nullptr);
protected:
    //鼠标事件
    void mousePressEvent(QMouseEvent * event);
    void mouseReleaseEvent(QMouseEvent * event);
    void mouseMoveEvent(QMouseEvent * event);
private:
    void setWindowGeometry(const QPointF &position);
    void setCursorIcon();
    MousePosition getMousePositon(QPointF &position);

    //缩放边距
    int step = 5;
    //鼠标位置
    MousePosition mousePosition = NORMAL;
    //起始位置
    QPointF startPosition;
    //旧位置
    QPointF oldPosition;
    //旧大小
    QSize oldSize;
};

#endif // FRAMELESSWINDOW_H
