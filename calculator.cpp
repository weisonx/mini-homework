#include "calculator.h"
#include <iostream>
#include <QDebug>
Calculator::Calculator() :
  det(0),sult()
{

}
using std::vector;

void Calculator::mem_det(double **array_, vector<vector<double>> *matrix_, int n) {
    if(matrix_->empty())
         return;
    for(unsigned row = 0; row != 3; ++row) {
        array_[row] = new double[3];    //内存泄漏
        for(unsigned col = 0, temp = 0; col != 4; ++col) {    //temp防止越界
            if(col != n) {  //  事实上应该是用b替代某项，但可以利用行列式的性质来处理，这里3阶，把A2的行列式取反即可
                array_[row][temp] =(*matrix_)[row][col];
                temp++;

            }
        }
    }
    return;
}

void Calculator::setDet(vector<vector<double>> *matrix_)
{
    double **array = new double*[3];  //计算det的系数
    this->mem_det(array, matrix_, 3);

/*   for(unsigned row = 0; row != 3; ++row) {
        for(unsigned col = 0; col != 3; ++col) {
            qDebug() << array[row][col] << " ";
        }
        qDebug() << endl;
    }
*/
    this->det = this->calculate(array, 3);
     qDebug() << "det" << det;
    delete[] array;
    return;
}

double Calculator::getDet()
{
    return this->det;
}

std::vector<double> Calculator::getSult()
{
    return sult;
}

void Calculator::cal(vector<vector<double>> *matrix_)
{
    if(det == 0)    //多解
        return;
    double A[3];
    int n = 0;  //用来控制不要哪列
    double **array = new double*[3];  //计算det的系数
    for(; n != 3; ++n){
        this->mem_det(array, matrix_, n);
        A[n] = this->calculate(array, 3);
        if(n == 1)
            A[n] = -A[n];
    }

    for(int i = 0; i != 3; ++i) {
        sult.push_back(A[i] / det);
    }
    delete[] array;
    return;

}


//计算行列式

void Calculator::swap(vector<int>& vec, int i, int j)
{
    int temp = vec[i];
    vec[i] = vec[j];
    vec[j] = temp;
}


void Calculator::Perm(vector<int>& vec, vector<vector<int> >& vec_seq, int current_index = 0)
{
    if (current_index == ((int)vec.size() - 1))
    {
        vec_seq.push_back(vec);
    }
    else
    {
        for (int i = current_index; i < (int)vec.size(); i++)
        {
            swap(vec, i, current_index);
            Perm(vec, vec_seq, current_index + 1);
            swap(vec, i, current_index);
        }
    }
}

vector<int> Calculator::inivec(int n)
{
    vector<int> vec;
    for (int i = 0; i < n; i++)
        vec.push_back(i);
    return vec;
}

bool Calculator::Iseven(int num)
{
    //用位与运算来判断奇偶（最快的判断奇偶的方法）
    return ((num & 1) == 0);
}
//是否幂为正
bool Calculator::PowerIsPosition(vector<int>& vec)
{
    //count即为逆序数，初始化为0
    int count = 0;
    for (int i = 0; i < (int)vec.size(); i++)
    {
        for (int j = i + 1; j < (int)vec.size(); j++)
        {
            if (vec[i] > vec[j])
            {
                count += 1;
            }
        }
    }
    return (Iseven(count));
}

double Calculator::calculate(double** array, int n)
{
    vector<vector<int> > vec_que;
    vector<int> vec = inivec(n);
    vector<int> vec_elem;
    Perm(vec, vec_que);
    //最终结果，初始化为0
    int result = 0;
    //依次为vec_que中取出行列式
    for (int i = 0; i < (int)vec_que.size();i++)
    {
        vec_elem = vec_que[i];
        //mi即为前面(-1)的n次幂，最后结果为-1或者1
        int mi = PowerIsPosition(vec_elem) ? 1 : (-1);
        double temp = mi;
        //row号初始化为0之后依次加1
        int row = 0;
        //col号依次从vec_elem中取出
        for (int j = 0; j < (int)vec_elem.size();j++)
        {
            int col = vec_elem[j];
            temp *= array[row++][col];
        }
        result += temp;

    }
    qDebug() << "result" << result;
    return result;
}
