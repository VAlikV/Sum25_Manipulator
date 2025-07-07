#include "point_control.hpp"

using namespace control;

PointControl::PointControl()
{

    current_rot_ << -1, 0, 0,
                    0, 1, 0,
                    0, 0, -1;

    // Задание начального положения
    current_pos_ << 0.5, 0.0, 0.6;

    command_ << 0,0,0,0,0,0,0,0,0,0,0,0;

    server1_.start();
}

PointControl::~PointControl()
{
    server1_.stop();
}

// ==================================================================================

void PointControl::loop()
{
    if (server1_.getMsg(command_))
    {
        current_pos_ << command_[0], command_[1], command_[2];
        current_rot_ << command_[3], command_[4], command_[5],
                        command_[6], command_[7], command_[8],
                        command_[9], command_[10], command_[11];

        std::cout << current_pos_.transpose() << std::endl;
        std::cout << current_rot_ << std::endl;
        std::cout << std::endl;

        // Обратная кинематика
        kinematic_.setPositionVector(current_pos_);
        kinematic_.setRotationMatrix(current_rot_);
        state_ = kinematic_.IK();
        thetta_ = kinematic_.getQ();

        std::cout << "State: " << state_ << std::endl;
        std::cout << "Thetta: " << thetta_.transpose() << std::endl;

        server1_.setMsg(thetta_);
    }
}

void PointControl::setConnection()
{
    
}

// ==================================================================================