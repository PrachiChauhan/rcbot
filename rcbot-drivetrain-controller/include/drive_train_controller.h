#ifndef _DRIVE_TRAIN_CONTROLLER_H_
#define _DRIVE_TRAIN_CONTROLLER_H_

#include "config.h"
#include "common_utils.h"

/* Drivetrain direction enum */
typedef enum {
    stop       = 0,
    backward   = 1,
    forward    = 2,
    left_turn  = 3,
    right_turn = 4,
}drive_train_op;

typedef struct {
    int positive_pin;
    int negative_pin;
    int pwm_pin;
    int pwm_speed;
}wheel_config_t;

typedef struct {
    int direction;
}drive_train_config_t;

int drive_train_init();
int drive_train_cleanup();
int change_driving_direction(int command_len, char* command);
//int change_driving_direction();

#endif // _DRIVE_TRAIN_CONTROLLER_H_