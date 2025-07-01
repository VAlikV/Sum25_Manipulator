#ifndef FUNC
#define FUNC

#include <Eigen/Dense>
#include <list>
#include <iostream>

const double V = 0.002;

// Функция для расчета смещения на каждом шагу
Eigen::Array<double,7,1> getDelta(const Eigen::Array<double,7,1> &next_thetta, const Eigen::Array<double,7,1> &current_thetta)
{
    Eigen::Array<double,7,1> vel;

    vel = Eigen::sign(next_thetta - current_thetta) * V; // Можно использовать Eigen::sign(...)

    return vel;
}

// Функция для проверки "равенства" массивов
bool eigenArrayEqual(const Eigen::Array<double,7,1> &arr1, const Eigen::Array<double,7,1> &arr2, const Eigen::Array<double,7,1> &eps)
{
    for(int i = 0; i < 7; ++i)
    {
        if(std::abs(arr1(i) - arr2(i)) > eps(i)) // Можно воспользоваться std::abs(...)
        {
            return false;
        }
    }
    return true;
}


#endif