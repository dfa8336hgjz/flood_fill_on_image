#ifndef POSHANDLE_H
#define POSHANDLE_H
#include <opencv2/opencv.hpp>
#include <QLabel>
#include <QMouseEvent>

extern int max_w, max_h;

class poshandle : public QLabel
{
    Q_OBJECT
public:
    poshandle(QWidget* parent=0);

    void changeRect(int w, int h);

    void setPic(cv::Mat picMat);

protected:
    void mousePressEvent(QMouseEvent * mouseE);

signals:
    void sendMousePosition(QPoint&);
};

#endif // POSHANDLE_H
