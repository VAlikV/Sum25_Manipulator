#include "tele_params/tele_params.hpp"

using namespace params;

HapicState haptic_state;    // Структура для хранения состояния haptic
TeleState teleoperation = TeleState(0); // Класс с методами для управления

/*******************************************************************************
 Haptic callback.  
*******************************************************************************/
HDCallbackCode HDCALLBACK Callback(void *data)
{
    hdBeginFrame(hdGetCurrentDevice());

    // =========================================================================== <
   
    // Code
    hdGetDoublev(HD_CURRENT_POSITION, haptic_state.position);
    hdGetDoublev(HD_CURRENT_JOINT_ANGLES, haptic_state.joint_angles);
    hdGetDoublev(HD_CURRENT_GIMBAL_ANGLES, haptic_state.wrist_angles);
    hdGetIntegerv(HD_CURRENT_BUTTONS, &(haptic_state.buttons));

    teleoperation.loop(haptic_state);
    // haptic_state.force = teleoperation.getForceVector();
    // End code

    // =========================================================================== >

    hdEndFrame(hdGetCurrentDevice());

    HDErrorInfo error;
    if (HD_DEVICE_ERROR(error = hdGetError()))
    {
        hduPrintError(stderr, &error, "Error during main scheduler callback\n");

        if (hduIsSchedulerError(&error))
        {
            return HD_CALLBACK_DONE;
        }        
    }

    return HD_CALLBACK_CONTINUE;
}

/******************************************************************************
 main function
******************************************************************************/
int main(int argc, char* argv[])
{
    // My code
    // End code

    HDErrorInfo error;
    // Initialize the default haptic device.
    HHD hHD = hdInitDevice(HD_DEFAULT_DEVICE);
    if (HD_DEVICE_ERROR(error = hdGetError()))
    {
        hduPrintError(stderr, &error, "Failed to initialize haptic device");
        fprintf(stderr, "\nPress any key to quit.\n");
        return -1;
    }

    // Start the servo scheduler and enable forces.
    hdEnable(HD_FORCE_OUTPUT);
    hdStartScheduler();
    if (HD_DEVICE_ERROR(error = hdGetError()))
    {
        hduPrintError(stderr, &error, "Failed to start scheduler");
        fprintf(stderr, "\nPress any key to quit.\n");
        return -1;
    }
        
    // Application loop - schedule our call to the main callback.
    HDSchedulerHandle hCallback = hdScheduleAsynchronous(
        Callback, 0, HD_DEFAULT_SCHEDULER_PRIORITY);

    printf("Sphere example.\n");
    printf("Move the device around to feel a frictionless sphere\n\n");
    printf("Press any key to quit.\n\n");

    while (1)
    {
        if (!hdWaitForCompletion(hCallback, HD_WAIT_CHECK_STATUS))
        {
            fprintf(stderr, "\nThe main scheduler callback has exited\n");
            fprintf(stderr, "\nPress any key to quit.\n");
            break;
        }
    }

    // For cleanup, unschedule our callbacks and stop the servo loop.
    hdStopScheduler();
    hdUnschedule(hCallback);
    hdDisableDevice(hHD);

    return 0;
}

/*****************************************************************************/

