#include "textwidget.h"
#include <QDebug>
TextWidget::TextWidget(QWidget *parent, int w, int h)
    : QLabel(parent),m_width(w),m_height(h),m_textCanMove(true)
{
    m_canResize = false;
    m_mouseClick = false;
    m_iMarginWidth = 5;
    m_curResizeType = OTHER;
    setWindowOpacity(1);
    setAttribute(Qt::WA_TranslucentBackground);
    this->setAutoFillBackground(true);
    this->setMouseTracking(true);  //鼠标不按下触发事件
}

void TextWidget::paintEvent(QPaintEvent *event)
{
    QLabel::paintEvent(event);  //缺少文字不能显示
    QPainter painter(this); //不能申请多个，
    QPen rect;
    rect.setWidth(3);
    rect.setColor(QColor(255,0,0));
    painter.setPen(rect);
    //画出框体矩形框
    painter.drawRect(m_iMarginWidth, m_iMarginWidth, m_width-2*m_iMarginWidth, m_height-2*m_iMarginWidth);
    painter.setPen(QPen(Qt::red));
    painter.setBrush(QBrush(QColor(255,255,0), Qt::SolidPattern));
    //画出用于拖拽框体变型的八个点
    painter.drawEllipse(QPoint(m_iMarginWidth, m_iMarginWidth), m_iMarginWidth, m_iMarginWidth);
    painter.drawEllipse(QPoint(m_iMarginWidth,this->height()/2), m_iMarginWidth, m_iMarginWidth);
    painter.drawEllipse(QPoint(m_iMarginWidth,this->height() - m_iMarginWidth), m_iMarginWidth,
                                                                                    m_iMarginWidth);
    painter.drawEllipse(QPoint(this->width()/2,m_iMarginWidth), m_iMarginWidth, m_iMarginWidth);
    painter.drawEllipse(QPoint(this->width()/2,this->height() - m_iMarginWidth), m_iMarginWidth,
                                                                                    m_iMarginWidth);
    painter.drawEllipse(QPoint(this->width() - m_iMarginWidth, m_iMarginWidth), m_iMarginWidth,
                                                                                    m_iMarginWidth);
    painter.drawEllipse(QPoint(this->width() - m_iMarginWidth, this->height()/2), m_iMarginWidth,
                                                                                    m_iMarginWidth);
    painter.drawEllipse(QPoint(this->width() - m_iMarginWidth, this->height() - m_iMarginWidth),
                                                                m_iMarginWidth, m_iMarginWidth);
}

void TextWidget::mouseMoveEvent(QMouseEvent *event)
{
    QPoint point = event->globalPos();
    QPoint tmpPoint = event->pos();

    if(m_textCanMove){
        m_curResizeType = OTHER;
        if(tmpPoint.x() > 0 && tmpPoint.x() < m_iMarginWidth){
            if(tmpPoint.y() > 0 && tmpPoint.y() < m_iMarginWidth){
                m_curResizeType = LEFTTOP;
                this->setCursor(Qt::SizeFDiagCursor); //改变鼠标图标
            }
            else if(tmpPoint.y() > (this->height()/2 - m_iMarginWidth)
                    && tmpPoint.y() < (this->height()/2 + m_iMarginWidth)){
                m_curResizeType = LEFTMID;
                this->setCursor(Qt::SizeHorCursor); //改变鼠标图标
            }
            else if(tmpPoint.y() > (this->height() - m_iMarginWidth) && tmpPoint.y() < this->height()){
                m_curResizeType = LEFTBUTTOM;
                this->setCursor(Qt::SizeBDiagCursor); //改变鼠标图标
            }
        }
        else if(tmpPoint.x() > (this->width()/2 - m_iMarginWidth) &&
                tmpPoint.x() < (this->width()/2 + m_iMarginWidth)){
            if(tmpPoint.y() > 0 && tmpPoint.y() < m_iMarginWidth){
                m_curResizeType = TOP;
                this->setCursor(Qt::SizeVerCursor); //改变鼠标图标
            }
            else if(tmpPoint.y() > (this->height() - m_iMarginWidth) &&
                    tmpPoint.y() < this->height()){
                m_curResizeType = BUTTOM;
                this->setCursor(Qt::SizeVerCursor); //改变鼠标图标
            }
        }
        else if(tmpPoint.x() > (this->width()-m_iMarginWidth) &&
                tmpPoint.x() < this->width()){
            if(tmpPoint.y() > 0 && tmpPoint.y() < m_iMarginWidth){
                m_curResizeType = RIGHTTOP;
                this->setCursor(Qt::SizeBDiagCursor); //改变鼠标图标
            }
            else if(tmpPoint.y() > (this->height()/2 - m_iMarginWidth)
                    && tmpPoint.y() < (this->height()/2 + m_iMarginWidth)){
                m_curResizeType = RIGHTMID;
                this->setCursor(Qt::SizeHorCursor); //改变鼠标图标
            }
            else if(tmpPoint.y() > (this->height() - m_iMarginWidth) && tmpPoint.y() < this->height()){
                m_curResizeType = RIGHTBUTTOM;
                this->setCursor(Qt::SizeFDiagCursor); //改变鼠标图标
            }
        }

        if(m_curResizeType == OTHER){
            this->setCursor(Qt::ClosedHandCursor); //改变鼠标图标
        }
    }

    if(m_curResizeType == OTHER){
        if(m_mouseClick){
            this->move(point - m_movePoint);
        }
    }
    else if(m_curResizeType == LEFTTOP){
        if(m_mouseClick){
            QPoint tmpPoint = point - m_lastPoint;
            int tmpWidth = m_lastSize.width() - tmpPoint.x();
            int tmpHeight = m_lastSize.height() - tmpPoint.y();
            if(tmpWidth <= 1){
                tmpWidth = 1;
            }

            if(tmpHeight <= 1){
               tmpHeight = 1;
            }
            this->resize(tmpWidth, tmpHeight);
            m_width = this->width();
            m_height = this->height();
            this->move(point - m_movePoint);
        }
    }
    else if(m_curResizeType == LEFTMID){
        if(m_mouseClick){
            QPoint tmpPoint = point - m_lastPoint;
            int tmpWidth = m_lastSize.width() - tmpPoint.x();

            if(tmpWidth <= 1){
                tmpWidth = 1;
            }
            this->resize(tmpWidth, this->height());
            m_width = this->width();
            m_height = this->height();
            this->move((point - m_movePoint).x(),this->pos().y());
        }
    }
    else if(m_curResizeType == LEFTBUTTOM){
        if(m_mouseClick){
            QPoint tmpPoint = point - m_lastPoint;
            int tmpWidth = m_lastSize.width() - tmpPoint.x();
            int tmpHeight = m_lastSize.height() + tmpPoint.y();
            if(tmpWidth <= 1){
                tmpWidth = 1;
            }

            if(tmpHeight <= 1){
               tmpHeight = 1;
            }
            this->resize(tmpWidth, tmpHeight);
            m_width = this->width();
            m_height = this->height();
            this->move((point - m_movePoint).x(),this->pos().y());
        }
    }
    else if(m_curResizeType == TOP){
        if(m_mouseClick){
            QPoint tmpPoint = point - m_lastPoint;
            int tmpHeight = m_lastSize.height() - tmpPoint.y();

            if(tmpHeight <= 1){
               tmpHeight = 1;
            }
            this->resize(this->width(), tmpHeight);
            m_width = this->width();
            m_height = this->height();
            this->move(this->pos().x(),(point - m_movePoint).y());
        }
    }
    else if(m_curResizeType == BUTTOM){
        if(m_mouseClick){
            QPoint tmpPoint = point - m_lastPoint;
            int tmpHeight = m_lastSize.height() + tmpPoint.y();

            if(tmpHeight <= 1){
               tmpHeight = 1;
            }
            this->resize(this->width(), tmpHeight);
            m_width = this->width();
            m_height = this->height();
        }
    }
    else if(m_curResizeType == RIGHTTOP){
        if(m_mouseClick){
            QPoint tmpPoint = point - m_lastPoint;
            int tmpWidth = m_lastSize.width() + tmpPoint.x();
            int tmpHeight = m_lastSize.height() - tmpPoint.y();
            if(tmpWidth <= 1){
                tmpWidth = 1;
            }

            if(tmpHeight <= 1){
               tmpHeight = 1;
            }
            this->resize(tmpWidth, tmpHeight);
            m_width = this->width();
            m_height = this->height();
            this->move(this->pos().x(), (point - m_movePoint).y());
        }
    }
    else if(m_curResizeType == RIGHTMID){
        if(m_mouseClick){
            QPoint tmpPoint = point - m_lastPoint;
            int tmpWidth = m_lastSize.width() + tmpPoint.x();

            if(tmpWidth <= 1){
                tmpWidth = 1;
            }

            this->resize(tmpWidth, this->height());
            m_width = this->width();
            m_height = this->height();
        }
    }
    else if(m_curResizeType == RIGHTBUTTOM){
        if(m_mouseClick){
            QPoint tmpPoint = point - m_lastPoint;
            int tmpWidth = m_lastSize.width() + tmpPoint.x();
            int tmpHeight = m_lastSize.height() + tmpPoint.y();
            if(tmpWidth <= 1){
                tmpWidth = 1;
            }

            if(tmpHeight <= 1){
               tmpHeight = 1;
            }
            this->resize(tmpWidth, tmpHeight);
            m_width = this->width();
            m_height = this->height();
        }
    }

}

void TextWidget::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton){
        m_mouseClick = true;
        //记下原窗口位置跟鼠标位置的差值，当前鼠标位置减去差值，即可得到当前窗口显示位置
        m_movePoint = event->globalPos() - this->pos();
        m_lastPoint = event->globalPos();
        m_lastSize = this->size();
        if(m_curResizeType == OTHER){
            m_textCanMove = true;
        }
        else{
            m_textCanMove = false;
        }
    }
}

void TextWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton){
        m_mouseClick = false;
        m_textCanMove = true;
    }
}
