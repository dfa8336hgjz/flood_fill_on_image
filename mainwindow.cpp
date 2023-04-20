#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "poshandle.h"
#include <QFileDialog>
#include <QPixmap>
#include <math.h>

void scaleSize(int& w, int& h) {
    if (double(h) / w <= double(max_h) / max_w) {
        h = h / double(w) * max_w ;
        w = max_w ;
    }
    else {
        w = w / double(h) * max_h ;
        h = max_h;
    }
}

bool inRange(cv::Vec3b cColor, int oR, int oG, int oB) {
    int cB = cColor[0];
    int cG = cColor[1];
    int cR = cColor[2];
    return sqrt(((float)cB - oB) * (cB - oB) + (cG - oG) * (cG - oG) + (cR - oR) * (cR - oR)) <= 70;
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    r=g=b=0;
    m_x=m_y=0;
    checked=false;
    connect(ui->pic, SIGNAL(sendMousePosition(QPoint&)), this, SLOT(change_pixel_pos(QPoint&)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_openbtn_clicked()
{
    QString filename= QFileDialog::getOpenFileName(this, "Choose a picture file","", "Image(*.png *.jpg *.jpeg *.bmp *.gif)");
    if(QString::compare(filename, QString())!=0){
        picMat = cv::imread(filename.toStdString(),1);
        int c = picMat.cols;
        int r = picMat.rows;
        scaleSize(c,r);
        cv::resize(picMat, picMat, cv::Size(c,r));;
        QImage curpic((uchar*)picMat.data, picMat.cols, picMat.rows, picMat.step, QImage::Format_BGR888);
        ui->pic->setPixmap(QPixmap::fromImage(curpic));
        ui->pic->changeRect(c, r);
    }
}

void MainWindow::on_r_slider_valueChanged(int value)
{
    this->r=value;
    ui->label_r->setText(QString("%1").arg(value));
    ui->color_widget->setStyleSheet(QString("border: 5px solid rgb(108, 108, 108); background-color: rgb(%1, %2, %3);").arg(this->r).arg(this->g).arg(this->b));
}

void MainWindow::on_g_slider_valueChanged(int value)
{
    this->g=value;
    ui->label_g->setText(QString("%1").arg(value));
    ui->color_widget->setStyleSheet(QString("border: 5px solid rgb(108, 108, 108); background-color: rgb(%1, %2, %3);").arg(this->r).arg(this->g).arg(this->b));
}

void MainWindow::on_b_slider_valueChanged(int value)
{
    this->b=value;
    ui->label_b->setText(QString("%1").arg(value));
    ui->color_widget->setStyleSheet(QString("border: 5px solid rgb(108, 108, 108); background-color: rgb(%1, %2, %3);").arg(this->r).arg(this->g).arg(this->b));
}

void MainWindow::change_pixel_pos(QPoint &pos)
{
    if(checked){
        m_x=pos.x();
        m_y=pos.y();
        floodFill(m_x, m_y, r, g, b);
        ui->pic->setPic(picMat);
    }
}

void MainWindow::on_pushButton_clicked()
{
    if(!checked && !picMat.empty()) {
        checked=true;
        ui->pic->setCursor(QCursor(QPixmap(":/brush.cur"),0,0));
        ui->pushButton->setText("UNFILL");
    }
    else {
        ui->pic->setCursor(QCursor(Qt::ArrowCursor));
        checked=false;
        ui->pushButton->setText("FILL");
    }
}


bool MainWindow::valid(int x, int y, int r, int g, int b) {
    return x >= 0 && y >= 0 && x < picMat.cols && y < picMat.rows && inRange(picMat.at<cv::Vec3b>(y, x), r, g, b);
}

void MainWindow::scan(int lx, int rx, int y, std::stack<std::pair<int, int>>& s, int r, int g, int b) {
    bool span_added = false;
    for (int x = lx; x <= rx; x++) {
        if (!valid(x, y, r, g, b)) {
            span_added = false;
        }

        else if (!span_added) {
            s.push({ x,y });
            span_added = true;
        }
    }
}

void MainWindow::floodFill(int x, int y, int r, int g, int b) {
    int oB = picMat.at<cv::Vec3b>(y, x)[0];
    int oG = picMat.at<cv::Vec3b>(y, x)[1];
    int oR = picMat.at<cv::Vec3b>(y, x)[2];
    if (inRange(picMat.at<cv::Vec3b>(y, x), r, g, b)) return;

    std::stack<std::pair<int, int>> s;
    s.push({ x,y });
    while (!s.empty()) {
        std::pair<int, int> cur = s.top();
        s.pop();
        int lx = cur.first;
        while (valid(lx - 1, cur.second, oR, oG, oB)) {
            picMat.at<cv::Vec3b>(cur.second, lx-1)[0] = b;
            picMat.at<cv::Vec3b>(cur.second, lx-1)[1] = g;
            picMat.at<cv::Vec3b>(cur.second, lx-1)[2] = r;
            lx--;
        }

        while (valid(cur.first, cur.second, oR, oG, oB)) {
            picMat.at<cv::Vec3b>(cur.second, cur.first)[0] = b;
            picMat.at<cv::Vec3b>(cur.second, cur.first)[1] = g;
            picMat.at<cv::Vec3b>(cur.second, cur.first)[2] = r;
            cur.first++;
        }
        scan(lx, cur.first - 1, cur.second + 1, s, oR, oG, oB);
        scan(lx, cur.first - 1, cur.second - 1, s, oR, oG, oB);
    }

}

