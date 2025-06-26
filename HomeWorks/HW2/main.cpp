#include "kukafri/kukafricontroller.hpp"
#include <cmath>
#include <thread>
#include <iostream>
#include <iomanip>
#include <chrono>
#include <fstream>
#include <queue>

// #include "logger/jarraylogger.hpp"
#include "udp/udp_server.hpp"
#include "kukafri/helper_functions.hpp"
#include "func.hpp

using namespace KUKA_CONTROL;
using namespace server;

int main(int argc, char **argv)
{   

    // --------------------------- Инициализация UDP сервера

    std::cout << "Hello!\n";

    UDPServer<7,14> server("127.0.0.1", 8081, "127.0.0.1", 8080);

    // --------------------------- Инициализация переменных и векторов
    
    KukaFRIController kuka(KUKA_CONTROL::JOINT_POSITION);

    jarray initial_position;

    // Есть особенноесть. Манипулятор выдает и принимает сообщения в формате jarray (просто псевдоним для std::array)
    // Но вычислять что-то удобнее, используя Eigen::Array. По этой причине реализованы функции для перобразования из jarray
    // В Eigen::Array и обратно - это stdArrayToEigenArray и eigenArrayToStdArray, соответственно

    Eigen::Array<double,7,1> current_point;     // Для записи текущей позиции манипулятора
    Eigen::Array<double,7,1> initial_point;     // Для записи начальной позиции манипулятора
    Eigen::Array<double,7,1> next_point;        // Следующая точка в траектории

    Eigen::Array<double,7,1> temp;              // Переменная, которая будет передаваться в качестве управления
    Eigen::Array<double,7,1> dq;                // Смещение манипулятора на каждом шагу
    Eigen::Array<double,7,1> msg_thetta;        // Переменная для чтения приходящих значений

    std::queue<Eigen::Array<double,7,1>> points_queue;      // Очередь с точками для траекторного планирования (для CV)

    const double e = 0.1*M_PI/180;      // Доверительный интревал, попав в который, мы считаем, что пришли в точку
    Eigen::Array<double,7,1> eps;       
    eps << e, e, e, e, e, e, e;

    bool done = true;       // Флаг, дошли ли мы до следующей точки

    // --------------------------- Запуск

    kuka.start();
    server.start();

    initial_position = kuka.getMeasuredJointPosition();     // Пример чтения углов
    kuka.setTargetJointPosition(initial_position);          // Пример задания углов

    initial_point = stdArrayToEigenArray(initial_position); // Преобразование из jarray в Eigen::Array
    next_point = initial_point;
    temp = initial_point;

    std::cout << "Старт" << std::endl;

    while (true)
    {
        if (server.getMsg(msg_thetta))  // Чтение пришедших по UDP данных. Если сообщение есть getMsg возвращает true и перезаписывает переменную msg_thetta
        {
            <Реализовать запись msg_thetta в очередь точек (в траекторию) или замену next_point на msg_thetta>; // метод push
        };      

        // ========================================================================================

        current_point = <Чтение углов куки>; // Не забыть про преобразование типов

        if (done)   // Если манипулятор пришел в целевую точку
        {
            next_point = <Вытащить следуюущую точку из траектории> // метод front и pop (для телеуправления это не нужно, просто оставить в этой ветке done = false)
            done = false;
        }
        else
        {
            delta = getDelta(next_point, current_point); // Реализовать эту функцию в func.hpp
            temp = temp + delta;
            
            kuka.<Задание углов куки>;

            if (eigenArrayEqual(current_point,next_point,eps))  // Реализовать эту функцию в func.hpp
            {
                done = true;
            }
        }
        
        // ========================================================================================

        std::this_thread::sleep_for(std::chrono::microseconds(900));
    }

    server.stop();
    kuka.stop();

    return 0;
}