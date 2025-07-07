#ifndef CONTROL
#define CONTROL

#include <stdio.h>
#include <stdlib.h>
#include <algorithm>

#include <Eigen/Dense>

#include "../ik/drake_kinematic.hpp"
#include "../udp/udp_server.hpp"
#include "../logger/logger.hpp"

#include <iostream>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>

#include <chrono>

using namespace iiwa_kinematics;

namespace control
{    
    class PointControl
    {
    private:

        // ================================================================================
        // ================================================================================    
        // ================================================================================

        Eigen::Matrix<double,3,3> current_rot_;      // Матрица ориентации

        Eigen::Vector3d current_pos_;       // Текущее положение

        // ================================================================================
        // ================================================================================    
        // ================================================================================    

        int state_;           // Статус решения обратной кинематики
        DrakeKinematic kinematic_ = DrakeKinematic("../robots/iiwa.urdf");    // Решатель кинематики

        Eigen::Array<double,12,1> command_;             // Команда
        Eigen::Array<double,7,1> thetta_;               // Рассчитанные углы в джоинтах

        // ================================================================================    
        // ================================================================================    
        // ================================================================================    

        server::UDPServer<12,7> server1_ = server::UDPServer<12,7>("127.0.0.1", 8081, "127.0.0.1", 8082);  // UDP

    public:
        PointControl();
        ~PointControl();
        void loop();
        void setConnection();

        bool checkPos();
    };   

    // ================================================================================    

}

#endif