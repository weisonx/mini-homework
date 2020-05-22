#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>

namespace Ui {
class mainWidget;
}

class mainWidget : public QWidget
{
    Q_OBJECT

public:
    explicit mainWidget(QWidget *parent = nullptr);
    ~mainWidget();
    std::vector<std::vector<double>> *setMatrix();   //返回指向矩阵的值，可能会返回空指针。
    std::vector<std::vector<double> > *getMatrix();
private slots:
    void on_cal_clicked();

    void on_clear_clicked();

private:
    Ui::mainWidget *ui;
    std::vector<std::vector<double>> *matrix;       //指向输入的系数a、b
};

#endif // MAINWIDGET_H
