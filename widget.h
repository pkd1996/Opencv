#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <formcalibration.h>
#include <opencv2/opencv.hpp>
#include <QFileDialog>
#include <QLabel>
#include <QTextCodec>
#include "puttext.h"
#include "textwidget.h"
#include <QVBoxLayout>

#define MAX_CONTRAST 2.0
namespace Ui {
class Widget;
}

typedef struct _textSet_
{
    int m_textSize;
    QColor  m_textColor;
    QString m_textFont;
}TextSet;

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
    void initBrightForm();
    void initContrastForm();
    void updateShow(QImage &showImage);
    void matToImage(cv::Mat &srcMat, QImage & dstMat);
    void changePicBright(cv::Mat srcMat, cv::Mat & dstMat, int value);
    void changePicContrast(cv::Mat srcMat, cv::Mat & dstMat, double value);
    QByteArray strToGBK(const QString & text);
private slots:
    void on_btnBeauti_clicked();
    void toUpdatePicBright(int value);
    void on_btnOpenPic_clicked();
    void toUpdatePicContrast(int value);
    void on_btnPutText_clicked();

    void on_pushButton_clicked();

    void on_btnSavePic_clicked();

private:
    Ui::Widget *ui;
    FormCalibration * m_brightForm;
    FormCalibration * m_contrastForm;
    cv::Mat m_srcPicMat;
    cv::Mat m_showPicMat;
    QLabel * m_showLb;
    int m_curBrightValue;
    double m_curContrast;
    PutText * m_putText;
    TextWidget * m_textWidget;
    //显示图片大小
    int m_showPicWidth;
    int m_showPicHeight;
    //原图片大小
    int m_srcPicWidth;
    int m_srcPicHeight;
};

#endif // WIDGET_H
