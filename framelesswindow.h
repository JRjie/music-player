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
    void setCursorIcon();
    MousePosition getMousePositon(QPointF &position);

    //缩放边距
    int step = 8;
    //鼠标位置
    MousePosition mouse_position = NORMAL;
    //起始位置
    QPointF start_position;
    //旧位置
    QPointF old_Position;
    //旧大小
    QSize old_size;
};

#endif // FRAMELESSWINDOW_H
