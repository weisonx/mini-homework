#include "mainwidget.h"
#include "ui_mainwidget.h"
#include <QIntValidator>
#include "calculator.h"
#include <QDebug>
using std::vector;

mainWidget::mainWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::mainWidget),
    matrix(new vector<vector<double>>(3,vector<double>(4)))   //创建存储系数a、b的矩阵的向量
{
    ui->setupUi(this);
   /* QValidator *validator = new QIntValidator(-2147483648,2147483647,this);
    ui -> a11 ->setValidator((validator));
    ui -> a12 ->setValidator((validator));
    ui -> a21 ->setValidator((validator));
    ui -> a22 ->setValidator((validator));
    ui -> b1 ->setValidator((validator));
    ui -> b2 ->setValidator((validator));*/
}

mainWidget::~mainWidget()
{
    delete ui;
    delete matrix;  //回收矩阵向量
}

vector<vector<double>> *mainWidget::setMatrix()
{
    unsigned col_len = 4;
    unsigned row_len = 3;
    double temp = 0;    //临时保存系数的值
    QLineEdit *line_edit =nullptr;

    for(unsigned row = 0; row != row_len; ++row) {  //  行列矩阵赋值，使用switch，事实上应该用id匹配
        for(unsigned col = 0; col != col_len; ++col) {
            switch (row) {
                case 0 :
                    switch (col) {
                        case 0:
                            line_edit = ui->a11;
                        break;
                        case 1:
                            line_edit = ui ->a12;
                        break;
                        case 2:
                            line_edit = ui ->a13;
                        break;
                        case 3:
                            line_edit = ui ->b1;
                        break;
                    }
                break;
                case 1 :
                    switch (col) {
                        case 0:
                            line_edit = ui->a21;
                        break;
                        case 1:
                            line_edit = ui ->a22;
                        break;
                        case 2:
                            line_edit = ui ->a23;
                        break;
                        case 3:
                            line_edit = ui ->b2;
                        break;
                    }
                break;
                case 2 :
                    switch (col) {
                        case 0:
                            line_edit = ui->a31;
                        break;
                        case 1:
                            line_edit = ui ->a32;
                        break;
                        case 2:
                            line_edit = ui ->a33;
                        break;
                        case 3:
                            line_edit = ui ->b3;
                        break;
                    }
                break;
            }
            if(! line_edit ->text().isEmpty()) {
                temp = line_edit ->text() .toDouble();
                (*matrix)[row][col] = temp;
            }
            else return nullptr;
        }
    }
    return matrix;
}

std::vector<std::vector<double> > *mainWidget::getMatrix()
{
    return matrix;
}








void mainWidget::on_cal_clicked()
{
    Calculator cal;

    if(!this->setMatrix())
        return;

    cal.setDet(this->getMatrix());
    cal.cal(this->getMatrix());
    if(cal.getDet() == 0) {
        this->ui->xx->setText("多解");
        this->ui->yy->setText("多解");
        this->ui->zz->setText("多解");

    }
    qDebug() << cal.getDet() << "#";
    if(cal.getSult().empty())
        return;

    QString s[3];
    for(unsigned i = 0; i != 3; ++i) {
        s[i] += QString::number(cal.getSult()[i]);
     }

    this->ui->xx->setText(s[0]);
    this->ui->yy->setText(s[1]);
    this->ui->zz->setText(s[2]);

}

void mainWidget::on_clear_clicked()
{
    ui->a11->setText("");
    ui->a12->setText("");
    ui->a13->setText("");
    ui->b1->setText("");

    ui->a21->setText("");
    ui->a22->setText("");
    ui->a23->setText("");
    ui->b2->setText("");

    ui->a31->setText("");
    ui->a32->setText("");
    ui->a33->setText("");
    ui->b3->setText("");

    ui->xx->setText("");
    ui->yy->setText("");
    ui->zz->setText("");
    return;
}
