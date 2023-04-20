#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QtCore>
#include <opencv2/opencv.hpp>
#include <utility>
#include <stack>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    int r,g,b;
    int m_x, m_y;
    bool checked;
    cv::Mat picMat;

    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void floodFill(int x, int y, int r, int g, int b);
    bool valid(int x, int y, int r, int g, int b);
    void scan(int lx, int rx, int y, std::stack<std::pair<int, int>>& s, int r, int g, int b);

private slots:
    void on_openbtn_clicked();

    void on_r_slider_valueChanged(int value);

    void on_g_slider_valueChanged(int value);

    void on_b_slider_valueChanged(int value);

    void change_pixel_pos(QPoint& pos);

    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
