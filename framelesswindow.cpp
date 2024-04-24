#include "framelesswindow.h"

FramelessWindow::FramelessWindow(QWindow * parent) : QQuickWindow(parent)
{
    this->setFlags(Qt::Window | Qt::FramelessWindowHint | Qt::WindowMinMaxButtonsHint);
}

void FramelessWindow::mousePressEvent(QMouseEvent *event)
{
    this->startPosition = event->globalPosition();
    this->oldPosition = this->position();
    this->oldSize = this->size();
    event->ignore();

    QQuickWindow::mousePressEvent(event);
}

void FramelessWindow::mouseReleaseEvent(QMouseEvent *event)
{
    this->oldPosition = this->position();

    QQuickWindow::mouseReleaseEvent(event);
}

void FramelessWindow::mouseMoveEvent(QMouseEvent *event)
{
    QPointF pos = event->position();
    if(event->buttons() & Qt::LeftButton){
        this->setWindowGeometry(event->globalPosition());
    } else {
        this->mousePosition = this->getMousePositon(pos);
        this->setCursorIcon();
    }
}

void FramelessWindow::setWindowGeometry(const QPointF &position)
{
    QPointF offset = position - this->startPosition;
    if(offset.x() == 0 && offset.y() == 0) return;

    static auto set_geometry_func = [this](const QSize &size, const QPointF &pos){
        QPointF t_pos = this->oldPosition;
        QSize t_size = minimumSize();

        if(size.width() > minimumWidth()){
            t_pos.setX(pos.x());
            t_size.setWidth(size.width());
        }else if(this->mousePosition == LEFT || this->mousePosition == BOTTOMLEFT || this->mousePosition == TOPLEFT){
            t_pos.setX(this->oldPosition.x() + this->oldSize.width() - minimumWidth());
        }

        if(size.height() > minimumHeight()){
            t_pos.setY(pos.y());
            t_size.setHeight(size.height());
        }else if(this->mousePosition == TOP || this->mousePosition == TOPLEFT || this->mousePosition == TOPRIGHT){
            t_pos.setY(this->oldPosition.y() + this->oldSize.height() - minimumHeight());
        }
        this->setGeometry(t_pos.x(), t_pos.y(), t_size.width(), t_size.height());
        this->update();
    };

    switch (this->mousePosition) {
    case TOPLEFT:
        set_geometry_func(this->oldSize + QSize(-offset.x(), -offset.y()), this->oldPosition + offset);
        break;

    case TOP:
        set_geometry_func(this->oldSize + QSize(0, -offset.y()), this->oldPosition + QPointF(0, offset.y()));
        break;

    case TOPRIGHT:
        set_geometry_func(this->oldSize + QSize(offset.x(), -offset.y()), this->oldPosition + QPointF(0, offset.y()));
        break;

    case LEFT:
        set_geometry_func(this->oldSize + QSize(-offset.x(), 0), this->oldPosition + QPointF(offset.x(), 0));
        break;

    case RIGHT:
        set_geometry_func(this->oldSize + QSize(offset.x(), 0), this->oldPosition);
        break;

    case BOTTOMLEFT:
        set_geometry_func(this->oldSize + QSize(-offset.x(), offset.y()), this->oldPosition + QPointF(offset.x(), 0));
        break;

    case BOTTOM:
        set_geometry_func(this->oldSize + QSize(0, offset.y()), this->oldPosition);
        break;

    case BOTTOMRIGHT:
        set_geometry_func(this->oldSize + QSize(offset.x(), offset.y()), this->oldPosition);
        break;

    case NORMAL:
        break;
    }
}

void FramelessWindow::setCursorIcon()
{
    static bool isSet = false;
    switch(this->mousePosition){
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
