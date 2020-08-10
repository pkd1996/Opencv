#ifndef FORMCALIBRATION_H
#define FORMCALIBRATION_H

#include <QWidget>
#include <QMouseEvent>

namespace Ui {
class FormCalibration;
}

class FormCalibration : public QWidget
{
    Q_OBJECT

public:
    explicit FormCalibration(QWidget *parent = 0);
    ~FormCalibration();
    void setBackGround(QString style);
    void setTitle(QString text);
public:
    void mousePressEvent(QMouseEvent *event);
signals:
    void valueUpdate(int);
private:
    Ui::FormCalibration *ui;
    int m_value;
};

#endif // FORMCALIBRATION_H
