#include "framelesswindow.h"

FramelessWindow::FramelessWindow(QWindow * parent) : QQuickWindow(parent)
{
    this->setFlags(Qt::Window | Qt::FramelessWindowHint | Qt::WindowMinMaxButtonsHint);
}

void FramelessWindow::mousePressEvent(QMouseEvent *event)
{
    this->start_position = event->globalPosition();
    this->old_Position = this->position();
    this->old_size = this->size();
    event->ignore();

    QQuickWindow::mousePressEvent(event);
}

void FramelessWindow::mouseReleaseEvent(QMouseEvent *event)
{
    this->old_Position = this->position();
    QQuickWindow::mouseReleaseEvent(event);
}

void FramelessWindow::mouseMoveEvent(QMouseEvent *event)
{
    QPointF pos = event->position();
    if(event->buttons() & Qt::LeftButton){
    //TODO:改变大小
    } else {
        this->mouse_position = this->getMousePositon(pos);
        this->setCursorIcon();
    }

}

void FramelessWindow::setCursorIcon()
{
    static bool isSet = false;
    switch(this->mouse_position){
    case TOPLEFT:
    case BOTTOMRIGHT:
        this->setCursor(Qt::SizeFDiagCursor);
        isSet = true;
        break;
    case TOP:
    case BOTTOM:
        this->setCursor(Qt::SizeVerCursor);
        isSet = true;
        break;
    case TOPRIGHT:
    case BOTTOMLEFT:
        this->setCursor(Qt::SizeBDiagCursor);
        isSet = true;
        break;
    case LEFT:
    case RIGHT:
        this->setCursor(Qt::SizeHorCursor);
        isSet = true;
        break;
    default:
        if (isSet){
            isSet = false;
            this->unsetCursor();
        }
        break;
    }
}

FramelessWindow::MousePosition FramelessWindow::getMousePositon(QPointF &position)
{
    int x = position.x();
    int y = position.y();
    int w = this->width();
    int h = this->height();

    MousePosition mouse_pos = NORMAL;

    if( x >= 0 && x <= this->step && y>= 0 && y <= this->step){
        mouse_pos = TOPLEFT;
    } else if(x > this->step && x < (w - this->step) && y>= 0 && y <= this->step){
        mouse_pos = TOP;
    } else if(x >= (w - this->step) && x <= w && y >= 0 && y <= this->step){
        mouse_pos = TOPRIGHT;
    } else if(x >= 0 && x <= this->step && y > this->step && y < (h - this->step)){
        mouse_pos = LEFT;
    } else if(x >= (w - this->step) && x <= w && y > this->step && y < (h - this->step)){
        mouse_pos = RIGHT;
    } else if(x >= 0 && x <= this->step && y > (h - this->step) && y < h){
        mouse_pos = BOTTOMLEFT;
    } else if(x > this->step && x < (w - this->step) && y >= (h - this->step) && y <= h){
        mouse_pos = BOTTOM;
    } else if(x >= (w - this->step) && x <= w && y >= (h - this->step) && y <= h){
        mouse_pos = BOTTOMRIGHT;
    }

    return mouse_pos;
}
