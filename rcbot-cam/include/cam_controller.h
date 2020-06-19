#ifndef _CAM_CONTROLLER_H_
#define _CAM_CONTROLLER_H_

#include "config.h"
#include "common_utils.h"

#define cam_op_enum_base                    (100)
#define cam_op_enum_to_op_translate(_x)     ((_x)-(cam_op_enum_base))
#define position_to_pwm(_x)                 (((_x)*17/180) + 4)       // Found by solving linear equations description in doc

/* Cam Op enum 
 * At all times sequence in this enum
 * should be in sync with cam_fptr function pointer
 */
typedef enum {
    stop_op           = 100,
    start_op          = 101,
    cam_pan_left_op   = 102,
    cam_pan_right_op  = 103,
    cam_tilt_up_op    = 104,
    cam_tilt_down_op  = 105,
    cam_pos_reset_op  = 106,
    cam_pan_sweep_op  = 107,
    cam_tilt_sweep_op = 108
}cam_op;

typedef struct {
    int pwm_pin;
    int current_position;
    int default_position;
}servo_config_t;

int cam_init();
int cam_operation(int command_len, char* command);
int cam_cleanup();

#endif // _CAM_CONTROLLER_H_