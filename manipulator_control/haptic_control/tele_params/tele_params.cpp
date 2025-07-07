#include "tele_params.hpp"

using namespace params;

params::TeleState::TeleState(int mode)
{
    current_rot_ << -1, 0, 0,
                    0, 1, 0,
                    0, 0, -1;

    // Задание начального положения
    initial_pos_ << 0.5, 0.0, 0.6;
    current_pos_ << 0.5, 0.0, 0.6;

    server_.start();

    last_time_ = std::chrono::steady_clock::now();
}

TeleState::~TeleState()
{
    server_.stop();
}

// ==================================================================================

void params::TeleState::loop(const HapicState& haptic_state)
{
    
    btn_1 = (haptic_state.buttons & HD_DEVICE_BUTTON_1) ? HD_TRUE : HD_FALSE;
    btn_2 = (haptic_state.buttons & HD_DEVICE_BUTTON_2) ? HD_TRUE : HD_FALSE;

    position_ = haptic_state.position;
    joint_angles_ = haptic_state.joint_angles;
    wrist_angles_ = haptic_state.wrist_angles;

    time_ = std::chrono::steady_clock::now();
    // =================================================================================================================
    if ((btn_1) && (std::chrono::duration_cast<std::chrono::microseconds>(time_ - last_time_).count() >= 25000))    // =================================
    {   // =============================================================================================================

        delta_position_ = position_ - previous_position_;   // Смещение хаптика отнисительно предыдущего положения

        // Изменение положения на смещение 
        current_pos_.x() = current_pos_.x() - delta_position_[2]/trans_factor_;
        current_pos_.y() = current_pos_.y() - delta_position_[0]/trans_factor_;
        current_pos_.z() = current_pos_.z() + delta_position_[1]/trans_factor_;

        std::cout << "Текущая позиция:\n" << current_pos_.x() << "\t" << current_pos_.y() << "\t" << current_pos_.z() << std::endl;
        
        previous_position_ = position_;     // Сохраннение предыдущей позиции (для расчета смещения)

        std::cout << std::endl << "Текущая матрица:\n" << current_rot_ << std::endl;

        // ---------------------------------------------------------------------------------------------------

        message_ << current_pos_[0], current_pos_[1], current_pos_[2], current_rot_(0,0), current_rot_(0,1),current_rot_(0,2),current_rot_(1,0),current_rot_(1,1),current_rot_(1,2),current_rot_(2,0),current_rot_(2,1),current_rot_(2,2);
        std::cout << std::endl << "Сообщение:\n" << message_.transpose() << std::endl;

        // ---------------------------------------------------------------------------------------------------

        server_.setMsg(message_);    //Отправка углов на контроллер

        // ---------------------------------------------------------------------------------------------------

        last_time_ = std::chrono::steady_clock::now();
        std::cout << "Время: " << std::chrono::duration_cast<std::chrono::microseconds>(last_time_ - time_).count() << std::endl << std::endl;

    }   // =============================================================================================================
    else if ((btn_2) && (std::chrono::duration_cast<std::chrono::microseconds>(time_ - last_time_).count() >= 25000))   // =============================
    {   // =============================================================================================================

        delta_position_ = position_ - previous_position_;       // Смещение хаптика отнисительно предыдущего положения

        // Изменение положения на смещение 
        current_pos_.x() = current_pos_.x() - delta_position_[2]/trans_factor_*2;
        current_pos_.y() = current_pos_.y() - delta_position_[0]/trans_factor_*2;
        current_pos_.z() = current_pos_.z() + delta_position_[1]/trans_factor_*2;

        std::cout << "Текущая позиция:\n" << current_pos_.x() << "\t" << current_pos_.y() << "\t" << current_pos_.z() << std::endl;
        
        previous_position_ = position_;     // Сохраннение предыдущей позиции (для расчета смещения)

        current_rot_ = hapticFK(joint_angles_, wrist_angles_);      // Расчет ориентации

        std::cout << std::endl << "Текущая матрица:\n" << current_rot_ << std::endl;

        // ---------------------------------------------------------------------------------------------------

        message_ << current_pos_[0], current_pos_[1], current_pos_[2], current_rot_(0,0), current_rot_(0,1),current_rot_(0,2),current_rot_(1,0),current_rot_(1,1),current_rot_(1,2),current_rot_(2,0),current_rot_(2,1),current_rot_(2,2);
        std::cout << std::endl << "Сообщение:\n" << message_.transpose() << std::endl;

        // ---------------------------------------------------------------------------------------------------

        server_.setMsg(message_);    //Отправка углов на контроллер

        // ---------------------------------------------------------------------------------------------------

        last_time_ = std::chrono::steady_clock::now();;
        std::cout << "Время: " << std::chrono::duration_cast<std::chrono::microseconds>(last_time_ - time_).count() << std::endl << std::endl;

    }   // =============================================================================================================
    else if (!(btn_1) && !(btn_2))   // ================================================================================
    {   // =============================================================================================================

        previous_position_ = position_;
        
    }
}

// ==================================================================================

bool params::TeleState::checkPos()
{
    if ((current_pos_ - initial_pos_).norm() <= radius_)
    {
        return true;
    }
    else
    {
        return false;
    }
}

Eigen::Matrix<double,3,3> params::TeleState::R(double theta, double alpha)         // Rotation matrix
{
    Eigen::Matrix<double,3,3> R;
    R << cos(theta), -sin(theta)*cos(alpha),  sin(theta)*sin(alpha),
         sin(theta),  cos(theta)*cos(alpha), -cos(theta)*sin(alpha),
                  0,             sin(alpha),             cos(alpha);
    return R;
}

Eigen::Matrix<double,3,3> params::TeleState::hapticFK(hduVector3Dd& joint_angles, hduVector3Dd& wrist_angles)      // Forvard kinematics
{
    Eigen::Matrix<double,3,3> rotation = Eigen::Matrix<double,3,3>::Identity(3,3);

    for (int8_t i = 0; i < 3; ++i)
    {
        rotation = rotation * R(dh_theta[i]+joint_angles[i]*dh_m[i], dh_alpha[i]);
    }
    for (int8_t i = 3; i < 6; ++i)
    {
        rotation = rotation * R(dh_theta[i]+wrist_angles[i-3]*dh_m[i], dh_alpha[i]);
    }
    return rotation;
}