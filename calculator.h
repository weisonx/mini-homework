#ifndef CALCULATOR_H
#define CALCULATOR_H
#include <vector>
#include "mainwidget.h"
/**
 * 使用克拉默法则计算方程组的解
 */
class Calculator
{
public:
    //计算行列式
    void swap(std::vector<int>& vec, int i, int j);//交换元素

    void Perm(std::vector<int>& vec, std::vector<std::vector<int> >& vec_seq, int current_index);
    //第一个参数表示初始的数列，在上例中，该vec中的元素为1，2，3
    //第二个参数表示最终得到的全排列集合

    std::vector<int> inivec(int n);      //根据n生成一个初始vector
    bool Iseven(int num);       //得出排列的逆排序数，并根据奇偶判读正负
    bool PowerIsPosition(std::vector<int>& vec);    //是否幂为正

    double calculate(double** array, int n);      //计算结果
                                                //第一个参数表示输入的行列式
                                                //第二个参数表示该行列式的阶数，在本例中n = 3，即一个3*3的行列式

    Calculator();

    void mem_det(double **array_, std::vector<std::vector<double>> *matrix_, int n);    //用于选择的存储行列式,不要n列
    void setDet(std::vector<std::vector<double>> *matrix_);
    double getDet();   //获取行列式值
    std::vector<double> getSult();
    void cal(std::vector<std::vector<double>> *matrix_); //计算方程,结果存在sult

private:
    double det;    //  行列式的结果
    std::vector<double> sult;  //解的结果



};

#endif // CALCULATOR_H
