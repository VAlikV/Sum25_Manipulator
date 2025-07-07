// #include "main_haptic/haptic_handling.hpp"
#include "point_control/point_control.hpp"

using namespace control;

PointControl contrololo = PointControl();

int main(int argc, char* argv[])
{
    while(1)
    {
        contrololo.loop();
    }
}

