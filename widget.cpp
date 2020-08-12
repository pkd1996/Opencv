#include "widget.h"
#include "ui_widget.h"
#include <QDebug>
#include <QColorDialog>

Widget::Widget(QWidget *parent) :
    QWidget(parent),m_curBrightValue(0),m_curContrast(1.0),
    m_showPicWidth(0),m_showPicHeight(0),m_srcPicWidth(0),
    m_srcPicHeight(0),m_textWidget(nullptr),m_textSize(12),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    initBrightForm();
    initContrastForm();
    m_putText = new PutText(this);
    initPicShowWidget();
    //m_showLb = new QLabel(ui->picShowWidget);
    ui->stackedWidget->setCurrentWidget(ui->homeWidget);
    ui->spinBox->setValue(m_textSize);
    m_textColor = QColor(0,0,0);
    updateTextColor(m_textColor);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_btnBeauti_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->dealWidget);
}

void Widget::initBrightForm()
{
    m_brightForm = new FormCalibration(ui->dealWidget);
    m_brightForm->setObjectName(QStringLiteral("brightForm"));
    m_brightForm->setGeometry(QRect(15,70,200,60));
    m_brightForm->setBackGround(QString("background-color: rgb(63, 190, 93)"));
    m_brightForm->setTitle(tr("亮度"));
    connect(m_brightForm, SIGNAL(valueUpdate(int)), this, SLOT(toUpdatePicBright(int)));
}

void Widget::initContrastForm()
{
    m_contrastForm = new FormCalibration(ui->dealWidget);
    m_contrastForm->setObjectName(QStringLiteral("contrastForm"));
    m_contrastForm->setGeometry(QRect(15,150,200,60));
    m_contrastForm->setBackGround(QString("background-color: rgb(63, 190, 93)"));
    m_contrastForm->setTitle(tr("对比度"));
    connect(m_contrastForm, SIGNAL(valueUpdate(int)), this, SLOT(toUpdatePicContrast(int)));
}
void Widget::on_btnOpenPic_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("打开图片"),
                                                    QApplication::applicationDirPath(),
                                                    "File(*.png *.jpg *.bmp *.tif *.GIF)");
    if(fileName.isEmpty()){
        qDebug()<<"file name empty";
        return;
    }

    m_srcPicMat = cv::imread(fileName.toStdString());
    if(m_srcPicMat.empty()){
        qDebug()<<"imread file failed";
        return;
    }
    /*****************2019-10-8**************************/
    int srcWidth = m_srcPicMat.cols;
    int srcHeight = m_srcPicMat.rows;

    //宽大于高，以宽为基准缩放
    if(srcWidth > srcHeight){
        //计算宽高分辨率比
        float scale = (float)srcWidth/(float)srcHeight;
        qDebug()<<"src:"<<srcWidth<<srcHeight<<scale;
        //需要进行缩放
        if(srcWidth > PIC_MAX_WIDTH){
            m_showPicWidth = PIC_MAX_WIDTH;
            m_showPicHeight = (int)((float)m_showPicWidth/scale);
            //重新计算高度，如果高度超出
            if(m_showPicHeight > PIC_MAX_HEIGHT){
                m_showPicHeight = PIC_MAX_HEIGHT;
                m_showPicWidth =  (int)((float)m_showPicHeight * scale);
            }
        }
        else{
            m_showPicWidth = srcWidth;
            m_showPicHeight = srcHeight;
            if(m_showPicHeight > PIC_MAX_HEIGHT){
                m_showPicHeight = PIC_MAX_HEIGHT;
                m_showPicWidth =  (int)((float)m_showPicHeight * scale);
            }
        }
    }
    else{
        float scale = (float)srcHeight/(float)srcWidth;
        //需要进行缩放
        if(srcHeight > PIC_MAX_HEIGHT){
            m_showPicHeight = PIC_MAX_HEIGHT;
            m_showPicWidth = (int)((float)m_showPicHeight/scale);
        }
        else{
            m_showPicWidth = srcWidth;
            m_showPicHeight = srcHeight;
        }
    }
    m_srcPicWidth = srcWidth;
    m_srcPicHeight = srcHeight;
    //设置要显示的大小、缩放的大小统一为 (width/8)*8（显示区域大小需要对齐，32bit需要4对齐，64bit需要8对齐）
    m_showPicWidth = (m_showPicWidth / 8 ) * 8;
    m_showPicHeight = (m_showPicHeight / 8 ) * 8;
    /*****************2019-10-8**************************/
    //qDebug()<<ui->picShowWidget->x()<<ui->picShowWidget->y();
    //ui->picShowWidget->resize(m_showPicWidth, m_showPicHeight);
    //qDebug()<<"22"<<ui->picShowWidget->x()<<ui->picShowWidget->y();
    qDebug()<<"m_showPicWidth:"<<m_showPicWidth<<m_showPicHeight;
    m_showPicMat = cv::Mat::zeros(m_showPicHeight,m_showPicWidth,m_srcPicMat.type());
    cv::Mat tempSrcMat = cv::Mat::zeros(m_showPicMat.size(),m_showPicMat.type());
    //缩放原图像
    if(m_srcPicWidth > m_showPicWidth || m_srcPicHeight > m_showPicHeight){
       cv::resize(m_srcPicMat, m_showPicMat, m_showPicMat.size());
    }
    else{
        m_showPicMat = m_srcPicMat;
    }
    cv::cvtColor(m_showPicMat, tempSrcMat, CV_BGR2RGB);
    QImage showImage;
    matToImage(tempSrcMat, showImage);
    updateShow(showImage);
    m_curBrightValue = 0;
    m_curContrast = 1.0;
}

void Widget::updateShow(QImage &showImage)
{
    QPixmap pixmap = QPixmap::fromImage(showImage);
    m_showLb->setPixmap(pixmap);
    m_showLb->resize(showImage.width(), showImage.height());
}

void Widget::matToImage(cv::Mat & srcMat, QImage & dstMat)
{
    cv::resize(srcMat, srcMat, cv::Size(m_showPicWidth,m_showPicHeight));
    dstMat = QImage((const unsigned char *)(srcMat.data), srcMat.cols,
                                            srcMat.rows, QImage::Format_RGB888);
}

void Widget::toUpdatePicBright(int value)
{
    if(!m_showPicMat.empty()){
        cv::Mat dstMat = cv::Mat::zeros(m_showPicMat.size(),m_showPicMat.type());
        changePicBright(m_showPicMat, dstMat, value);

        cv::cvtColor(dstMat, dstMat, CV_BGR2RGB);
        QImage showImage;
        matToImage(dstMat, showImage);
        updateShow(showImage);
    }
}

void Widget::changePicBright(cv::Mat srcMat, cv::Mat & dstMat, int value)
{
    int width = srcMat.cols;
    int height = srcMat.rows;
    int channels = srcMat.channels();
    cv::Mat tempSrcMat;
    srcMat.convertTo(tempSrcMat, CV_32F);
    for(int row = 0; row < height; row++){
        for(int col = 0; col < width; col++){
            if(channels == 3){
                double b = tempSrcMat.at<cv::Vec3f>(row,col)[0];
                double g = tempSrcMat.at<cv::Vec3f>(row,col)[1];
                double r = tempSrcMat.at<cv::Vec3f>(row,col)[2];

                dstMat.at<cv::Vec3b>(row,col)[0] = cv::saturate_cast<uchar>(b*m_curContrast + value);
                dstMat.at<cv::Vec3b>(row,col)[1] = cv::saturate_cast<uchar>(g*m_curContrast + value);
                dstMat.at<cv::Vec3b>(row,col)[2] = cv::saturate_cast<uchar>(r*m_curContrast + value);
            }
            else if(channels == 1){
                int y = tempSrcMat.at<uchar>(row,col);
                dstMat.at<uchar>(row,col) = cv::saturate_cast<uchar>(y*m_curContrast + value);
            }
        }
    }
    m_curBrightValue = value;
}

void Widget::toUpdatePicContrast(int value)
{
    float updateValue = 1+ value * (MAX_CONTRAST / 100) ;
    if(!m_showPicMat.empty()){
        cv::Mat dstMat = cv::Mat::zeros(m_showPicMat.size(),m_showPicMat.type());
        changePicContrast(m_showPicMat, dstMat, updateValue);

        cv::cvtColor(dstMat, dstMat, CV_BGR2RGB);
        QImage showImage;
        matToImage(dstMat, showImage);
        updateShow(showImage);
    }
}

void Widget::changePicContrast(cv::Mat srcMat, cv::Mat & dstMat, double value)
{
    int width = srcMat.cols;
    int height = srcMat.rows;
    int channels = srcMat.channels();
    cv::Mat tempSrcMat;
    srcMat.convertTo(tempSrcMat, CV_32F);
    for(int row = 0; row < height; row++){
        for(int col = 0; col < width; col++){
            if(channels == 3){
                double b = tempSrcMat.at<cv::Vec3f>(row,col)[0];
                double g = tempSrcMat.at<cv::Vec3f>(row,col)[1];
                double r = tempSrcMat.at<cv::Vec3f>(row,col)[2];

                dstMat.at<cv::Vec3b>(row,col)[0] = cv::saturate_cast<uchar>(b*value + m_curBrightValue);
                dstMat.at<cv::Vec3b>(row,col)[1] = cv::saturate_cast<uchar>(g*value + m_curBrightValue);
                dstMat.at<cv::Vec3b>(row,col)[2] = cv::saturate_cast<uchar>(r*value + m_curBrightValue);
            }
            else if(channels == 1){
                int y = tempSrcMat.at<uchar>(row,col);
                dstMat.at<uchar>(row,col) = cv::saturate_cast<uchar>(y*value + m_curBrightValue);
            }
        }
    }
    m_curContrast = value;
}

void Widget::on_btnPutText_clicked()
{
    QString text = ui->textEdit->toPlainText();
    /*if(!text.isEmpty() && !m_srcPicMat.empty()){
        //转换字体，转换成GBK格式，
        QByteArray showText = strToGBK(text);
        m_putText->putTextZH(m_srcPicMat, showText.data(), cv::Point(30, 30), cv::Scalar(0, 0, 0), 80, "Arial");
        cv::cvtColor(m_srcPicMat, m_srcPicMat, CV_BGR2RGB);
        QImage showImage;
        matToImage(m_srcPicMat, showImage);
        updateShow(showImage);
    }*/
    if(text.isEmpty()){
        return;
    }
    m_text = text;
    if(m_textWidget == nullptr){
        m_textWidget = new TextWidget(m_showLb,200,50);
        m_textWidget->setGeometry(100,100,m_textWidget->getWidth(), m_textWidget->getHeight());
        m_textWidget->setContentsMargins(10,0,0,0);
        m_textWidget->updateText(m_text, m_textColor, m_textSize);
        m_textWidget->show();
    }
    else{
        m_textWidget->updateText(m_text, m_textColor, m_textSize);
    }
}

void Widget::on_pushButton_clicked()
{
  /*  QTextCodec *utf8 = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForLocale(utf8);
    QTextCodec* gbk = QTextCodec::codecForName("gbk");
    QString str1="您好";
    QString ziti1 = "微软雅黑";
    QString ziti2 = "楷体";
    QString strUnicode= utf8->toUnicode(str1.toLocal8Bit().data());
    QString strUnicode2= utf8->toUnicode(ziti1.toLocal8Bit().data());
    QString strUnicode3= utf8->toUnicode(ziti2.toLocal8Bit().data());
    QByteArray gb_bytes= gbk->fromUnicode(strUnicode);
     QByteArray gb_bytes2= gbk->fromUnicode(strUnicode2);
      QByteArray gb_bytes3= gbk->fromUnicode(strUnicode3);
   cv::Mat img = cv::imread("D:\\OpencvEnvironment\\Study\\class3\\test.jpg");
   //QString testText = QString::fromLocal8Bit("Arial字体换...\n行显示!");
   //QByteArray testText = QString::fromLocal8Bit("Arial字体换...\n行显示!");
   m_putText->putTextZH(img, gb_bytes.data(), cv::Point(50, 50), cv::Scalar(0, 0, 255), 30, "Arial");
   m_putText->putTextZH(img, gb_bytes.data(), cv::Point(50, 100), cv::Scalar(0, 255, 0), 30, gb_bytes2.data());
   m_putText->putTextZH(img, gb_bytes.data(), cv::Point(50, 200), cv::Scalar(128, 255, 0), 30, gb_bytes3.data());

   cv::imshow("test", img);*/
    //m_textWidget = new TextWidget(ui->homeWidget,100,100,100,100);
    //m_textWidget->show();
   // m_textWidget->repaint();
    if(m_textWidget == nullptr)
    {
        m_textWidget = new TextWidget(ui->homeWidget,100,100);
        m_textWidget->setGeometry(100,100,m_textWidget->getWidth(), m_textWidget->getHeight());
        m_textWidget->show();
    }
    else{
        m_textWidget->update();
    }
}

QByteArray Widget::strToGBK(const QString & text)
{
    QTextCodec *utf8 = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForLocale(utf8);
    QTextCodec* gbk = QTextCodec::codecForName("gbk");
    QString str1=text;
    QString strUnicode= utf8->toUnicode(str1.toLocal8Bit().data());
    QByteArray gb_bytes= gbk->fromUnicode(strUnicode);
    return gb_bytes;
}

void Widget::on_btnSavePic_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("打开图片"),
                                                    QApplication::applicationDirPath(),
                                                    "File(*.png *.jpg *.bmp *.tif *.GIF)");
    if(fileName.isEmpty()){
        qDebug()<<"file name empty";
        return;
    }
    cv::Mat tmpDstMat = cv::Mat::zeros(m_showPicMat.size(),m_showPicMat.type());
    //保存亮度，对比度设置
    if(m_curContrast > 0){
        if(!m_showPicMat.empty()){
            changePicContrast(m_showPicMat, tmpDstMat, m_curContrast);
        }
    }
    else if(m_curBrightValue > 0){
        if(!m_showPicMat.empty()){
            changePicBright(m_showPicMat, tmpDstMat, m_curBrightValue);
        }
    }
    else{
        tmpDstMat = m_showPicMat;
    }

    if(m_textWidget){
        if(!m_text.isEmpty() && !tmpDstMat.empty()){
            //转换字体，转换成GBK格式，
            QByteArray showText = strToGBK(m_text);
            cv::Scalar textColor = cv::Scalar(m_textColor.red(), m_textColor.green(), m_textColor.blue());
            m_putText->putTextZH(tmpDstMat, showText.data(), cv::Point(m_textWidget->x()+10,
                                      m_textWidget->y()+5), textColor, m_textSize, "Arial");
        }
    }
    cv::Mat tmpMat = cv::Mat::zeros(m_srcPicHeight,m_srcPicWidth, m_showPicMat.type());
    cv::resize(tmpDstMat, tmpMat, tmpMat.size());
    cv::imwrite(fileName.toStdString(), tmpMat);
}

void Widget::on_btnTextColorSet_clicked()
{
    QColor color = QColorDialog::getColor(QPalette::Base, this, tr("color dialog"));
    m_textColor = color;
    updateTextColor(m_textColor);
}

void Widget::updateTextColor(QColor textColor)
{
    m_textColor = textColor;
    QString style = QString("background-color: rgb(%1, %2, %3);border-radius: 5px;")
                     .arg(m_textColor.red()).arg(m_textColor.green()).arg(m_textColor.blue());
    ui->btnTextColorSet->setStyleSheet(style);

    if(m_textWidget != nullptr){
        m_textWidget->updateText(m_text, m_textColor, m_textSize);
    }
}

void Widget::on_spinBox_valueChanged(int arg1)
{
    if(arg1 > 0){
        m_textSize = arg1;
        if(m_textWidget != nullptr){
            m_textWidget->updateText(m_text, m_textColor, m_textSize);
        }
    }
}

void Widget::initPicShowWidget()
{
    QVBoxLayout *vLaout = new QVBoxLayout();
    QHBoxLayout *hLaout = new QHBoxLayout();
    m_showLb = new QLabel();
    vLaout->addWidget(m_showLb);
    vLaout->setMargin(0);
    vLaout->setSpacing(1);
    // 添加伸缩
    hLaout->addStretch();
    hLaout->addLayout(vLaout);
    hLaout->addStretch();

    hLaout->setMargin(0);
    hLaout->setSpacing(10);
    hLaout->setSizeConstraint(QLayout::SetMinimumSize);
    ui->picShowWidget->setLayout(hLaout);
}

