#include "formcalibration.h"
#include "ui_formcalibration.h"
#include <QDebug>

FormCalibration::FormCalibration(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormCalibration)
{
    ui->setupUi(this);
    m_value = ui->line->x() /(this->geometry().width()/100);
    ui->lbvalue->setText(QString::number(m_value));
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
}

FormCalibration::~FormCalibration()
{
    delete ui;
}

void FormCalibration::mousePressEvent(QMouseEvent *event)
{
    int posX = event->pos().x();
    ui->line->move(posX,0);
    m_value = posX/(this->geometry().width()/100);
    ui->lbvalue->setText(QString::number(m_value));
    emit valueUpdate(m_value);
}

void FormCalibration::setBackGround(QString style)
{
    QString backString = style + ";border-radius: 5px";
    ui->lbBackground->setStyleSheet(backString);
    ui->lbTitle->setStyleSheet(style);
    ui->lbvalue->setStyleSheet(style);
}

void FormCalibration::setTitle(QString text)
{
    ui->lbTitle->setText(text);
}
