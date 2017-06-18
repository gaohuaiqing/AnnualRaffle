#include "dialog.h"
#include "ui_dialog.h"


Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);


    isLeftPressDown = false;
    this->directionChanged = NONE;
    this->setMinimumHeight(100);
    this->setMinimumWidth(50);
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint);       // 设置成无边框对话框
    this->setMouseTracking(true);       //  追踪鼠标
    //this->setStyleSheet();

}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::MouseRegion(const QPoint &cursorGlobalPoint)
{
    // 获取窗体在屏幕上的位置，tl表示LeftTop位置，rb表示RightBottom位置
    QRect       rect = this->rect();
    QPoint      tl = mapToGlobal(rect.topLeft());
    QPoint      rb = mapToGlobal(rect.bottomRight());

    int     x = cursorGlobalPoint.x();
    int     y = cursorGlobalPoint.y();


    //if (((tl.x() + PADDING) >= x) && ((tl.x() <= x) && ((tl.y() + PADDING) >= y)) && (tl.y() <= y))
    if ((x <= tl.x()) && (y <= tl.y()))
    {
        // 左上角
        directionChanged = LEFTTOP;
        this->setCursor(QCursor(Qt::SizeFDiagCursor));      // 设置鼠标形状
    }
    //else if ((x >= ((rb.x() - PADDING))) && (x <= rb.x()) && (y > ((rb.y() - PADDING))) && (y <= rb.y()))
    else if ((x >= rb.x()) && (y >= rb.y()))
    {
        // 右下角
        directionChanged = RIGHTBOTTOM;
        this->setCursor(QCursor(Qt::SizeFDiagCursor));      // 设置鼠标形状
    }
    else if ((x <= (tl.x() + PADDING)) && (x >= tl.x()) && (y >= (tl.y() - PADDING)) && (y <= rb.y()))
    {
        // 左下角
        directionChanged = LEFTBOTTOM;
        this->setCursor(QCursor(Qt::SizeBDiagCursor));      // 设置鼠标形状
    }
    // else if ((x <= rb.x()) && (x >= (rb.x() - PADDING)) && (y >= tl.y()) && (y <= (tl.y() + PADDING)))
    else if ((x >= rb.x()) &&(y <= rb.y()))
    {
        // 右上角
        directionChanged = RIGHTTOP;
        this->setCursor(QCursor(Qt::SizeBDiagCursor));      // 设置鼠标形状
    }
    //else if ((x < (tl.x() + PADDING)) && (x >= tl.x()))
    else if (x < tl.x())
    {
        // 左边
        directionChanged = LEFT;
        this->setCursor(QCursor(Qt::SizeHorCursor));
    }
    //else if ((x < rb.x()) &&(x >= (rb.x() - PADDING)))
    else if ((x < rb.x()) && (x >= (rb.x() - PADDING)))
    {
        // 右边
        directionChanged = RIGHT;
        this->setCursor(QCursor(Qt::SizeHorCursor));
    }
    else if ((y >= tl.y()) && (y <= (tl.y() + PADDING)))
    {
        // 上边
        directionChanged = UP;
        this->setCursor(QCursor(Qt::SizeVerCursor));
    }
    else if ((y <= rb.y()) && (y >= (rb.y() - PADDING)))
    {
        // 下边
        directionChanged = DOWN;
        this->setCursor(QCursor(Qt::SizeVerCursor));
    }
    else {
        // 默认
        directionChanged = NONE;
        this->setCursor(QCursor(Qt::ArrowCursor));
    }
}

void Dialog::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        isLeftPressDown = false;
        if (directionChanged != NONE)
        {
            this->releaseMouse();
            this->setCursor(QCursor(Qt::ArrowCursor));
        }
    }
}

void Dialog::mousePressEvent(QMouseEvent *event)
{
    switch (event->button())
    {
    case Qt::LeftButton:
        isLeftPressDown = true;
        if (directionChanged != NONE)
        {
            this->mouseGrabber();
        }
        else
        {
            dragPosition = event->globalPos() - this->frameGeometry().topLeft();
        }

        break;

    case Qt::RightButton:
        `this->close();
        break;

    default:
        QDialog::mousePressEvent(event);
        break;
    }
}

void Dialog::mouseMoveEvent(QMouseEvent *event)
{
    QPoint      globalPoint = event->globalPos();
    QRect       rect = this->rect();
    QPoint      tl  = mapToGlobal(rect.topLeft());
    QPoint      rb  = mapToGlobal(rect.bottomRight());

    if (!isLeftPressDown)
    {
        this->MouseRegion(globalPoint);
    }
    else
    {
        if (directionChanged == NONE)
        {
            move(event->globalPos() - dragPosition);
            event->accept();
        }
        else
        {
            QRect       rMove(tl, rb);
            switch (directionChanged)
            {
                case LEFT:
                    if ((rb.x() - globalPoint.x()) <= this->minimumWidth())
                    {
                        rMove.setX(tl.x());
                    }
                    else
                    {
                        rMove.setX(globalPoint.x());
                    }
                    break;

                case RIGHT:
                    rMove.setWidth(globalPoint.x() - tl.x());
                    break;

                case UP:
                    if ((rb.y() - globalPoint.y()) <= this->minimumHeight() )
                    {
                        rMove.setY(tl.y());
                    }
                    else
                    {
                        rMove.setY(globalPoint.y());
                    }
                        break;

                case DOWN:
                    rMove.setHeight(globalPoint.y() - tl.y());
                    break;

                case LEFTTOP:
                    if ((rb.x() - globalPoint.x()) <= this->minimumWidth())
                    {
                        rMove.setX(tl.x());
                    }
                    else
                    {
                        rMove.setX(globalPoint.x());
                    }

                    if ((rb.y() - globalPoint.y()) <= this->minimumHeight())
                    {
                        rMove.setY(tl.y());
                    }
                    else
                    {
                        rMove.setY(globalPoint.y());
                    }
                    break;

                case RIGHTTOP:
                    rMove.setWidth(globalPoint.x() - tl.x());
                    rMove.setY(globalPoint.y());
                    break;

                case LEFTBOTTOM:
                    rMove.setX(globalPoint.x());
                    rMove.setHeight(globalPoint.y() - tl.y());
                    break;

                case RIGHTBOTTOM:
                    rMove.setWidth(globalPoint.x() - tl.x());
                    rMove.setHeight(globalPoint.y() - tl.y());
                    break;

                default:
                    break;
            }

            this->setGeometry(rMove);
        }
    }
}
























