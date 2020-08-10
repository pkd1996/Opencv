#ifndef TEXTWIDGET_H
#define TEXTWIDGET_H

#include <QLabel>
#include <QPainter>
#include <QMouseEvent>

class TextWidget : public QLabel
{
    Q_OBJECT
public:
    explicit TextWidget(QWidget *parent = nullptr,int w = 10, int h = 10);
    void paintEvent(QPaintEvent *event);
    void mouseMoveEvent(QMouseEvent * event);
    void mousePressEvent(QMouseEvent * event);
    void mouseReleaseEvent(QMouseEvent * event);
    int getWidth(){return m_width;}
    int getHeight(){return m_height;}
    void setWidth(int w){this->m_width = w;}
    void setHeigth(int h){this->m_height = h;}

    enum ResizeType{
        LEFTTOP,
        TOP,
        RIGHTTOP,
        LEFTMID,
        RIGHTMID,
        LEFTBUTTOM,
        BUTTOM,
        RIGHTBUTTOM,
        OTHER
    };
signals:

public slots:

private:
   int m_posx;
   int m_posy;
   int m_width;
   int m_height;
   int m_iMarginWidth;
   int m_canResize;
   QPoint m_movePoint;
   QPoint m_lastPoint;
   QSize m_lastSize;
   bool m_mouseClick;
   ResizeType m_curResizeType;
   bool m_textCanMove;
};

#endif // TEXTWIDGET_H
