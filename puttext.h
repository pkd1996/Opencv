#ifndef PUTTEXT_H
#define PUTTEXT_H

#include <QObject>
//#include <windows.h>
//#include <wingdi.h>
#include <qwindowdefs.h>
#include <QLibrary>
#include <windows.h>
#include <wingdi.h>
#include <string>
#include <opencv2/opencv.hpp>

class PutText : public QObject
{
    Q_OBJECT
public:
    explicit PutText(QObject *parent = nullptr);
    void GetStringSize(HDC hDC, const char* str, int* w, int* h);
    void putTextZH(cv::Mat &dst, const char* str, cv::Point org, cv::Scalar color, int fontSize,
        const char *fn = "Arial", bool italic = false, bool underline = false);

signals:

public slots:
};

#endif // PUTTEXT_H
