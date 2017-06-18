#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QMouseEvent>

#define PADDING     2
enum Direction
{
    UP = 0,
    DOWN,
    LEFT,
    RIGHT,
    LEFTTOP,
    LEFTBOTTOM,
    RIGHTTOP,
    RIGHTBOTTOM,
    NONE
};

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();

private:
    Ui::Dialog *ui;

    Direction   directionChanged;       // 窗口大小发生改变时，记录改变的方向
    bool        isLeftPressDown;        // 判断左键是否被按下
    QPoint      dragPosition;           // 窗口移动拖动时，需要记住的点

    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);


    void MouseRegion(const QPoint &cursorGlobalPoint);
};

#endif // DIALOG_H
