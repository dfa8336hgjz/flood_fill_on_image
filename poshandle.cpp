#include "poshandle.h"

int max_w=780;
int max_h=440;

poshandle::poshandle(QWidget* parent): QLabel(parent)
{
    this->setMouseTracking(true);
}

void poshandle::mousePressEvent(QMouseEvent *mouseE)
{
    QPoint mouse_pos = mouseE->pos();
    if(mouseE->button() == Qt::LeftButton)
        if(mouse_pos.x() <= this->size().width() && mouse_pos.y() <= this->size().height() && mouse_pos.x() > 0 && mouse_pos.y() > 0){
            emit sendMousePosition(mouse_pos);
    }
}

void poshandle::changeRect(int w, int h)
{
    int x=(max_w-w)/2;
    int y=(max_h-h)/2;
    this->setGeometry(x+46,y+130,w,h);
}

void poshandle::setPic(cv::Mat picMat)
{
    QImage curpic((uchar*)picMat.data, picMat.cols, picMat.rows, picMat.step, QImage::Format_BGR888);
    setPixmap(QPixmap::fromImage(curpic));
}
