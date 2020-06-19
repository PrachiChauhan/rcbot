#include "drive_train_controller.h"

wheel_config_t wheel_rt;
wheel_config_t wheel_lt;
drive_train_config_t drive_train;

static int set_speed(int pwm_duty_cycle);
static int set_direction_stop(int not_used);
static int set_direction_forward(int not_used);
static int set_direction_backward(int not_used);
static int set_direction_left(int degrees);
static int set_direction_right(int degrees);
static int degrees_to_millisec(int degrees);
static int soft_start();
static int hard_stop();
static int soft_stop();

static int (*set_direction_fptr[])(int) = { set_direction_stop,
                                             set_direction_backward,
                                             set_direction_forward,
                                             set_direction_left,
                                             set_direction_right,
                                             set_speed };

int drive_train_init(){
    printf("drive_train_init\n");
    wheel_lt.positive_pin = wheel_pos_pin_lt;
    wheel_lt.negative_pin = wheel_neg_pin_lt;
    wheel_lt.pwm_pin      = wheel_pwm_pin_lt;
    wheel_rt.positive_pin = wheel_pos_pin_rt;
    wheel_rt.negative_pin = wheel_neg_pin_rt;
    wheel_rt.pwm_pin      = wheel_pwm_pin_rt;
    wheel_lt.pwm_speed    = wheel_pwm_default_duty_cycle;
    wheel_rt.pwm_speed    = wheel_pwm_default_duty_cycle;

    if (wiringPiSetup() == -1) {
        printf("Failed to setup Wiring Pi!\n");
        return 1;
    }

    pinMode(wheel_lt.positive_pin, OUTPUT);
    pinMode(wheel_lt.negative_pin, OUTPUT);
    pinMode(wheel_lt.pwm_pin, OUTPUT);
    pinMode(wheel_rt.positive_pin, OUTPUT);
    pinMode(wheel_rt.negative_pin, OUTPUT);
    pinMode(wheel_rt.pwm_pin, OUTPUT);
    set_direction_stop(0);
    delay(5000);
    softPwmCreate (wheel_lt.pwm_pin, wheel_lt.pwm_speed, 100);
    softPwmCreate (wheel_rt.pwm_pin, wheel_rt.pwm_speed, 100);
    return 0;
}

int drive_train_cleanup(){
    set_direction_stop(0);
    return 0;
}

/*
 * Calls other directional functions based on the direction index
 * that it gets from drive_train_op enum.
 */
int change_driving_direction(int command_len, char* command){
    int rc = 0;
    int op_index = 0;
    int op_val = 0;
    const char* delim = ".";

    op_index = atoi((const char*)strtok(command, delim));
    op_val = atoi((const char*)strtok(NULL, delim));
    printf("    change_driving_direction %d|%d\n", op_index, op_val);

    if (op_index < sizeof(set_direction_fptr)/sizeof(*set_direction_fptr))
    {
        set_direction_fptr[op_index](op_val);
    }

    return rc;
}

static int set_speed(int pwm_duty_cycle){
    /* Restrict the lowest value, as motors become unusable */
    printf("    change_speed: %d\n", pwm_duty_cycle);
    pwm_duty_cycle = (pwm_duty_cycle < LOWEST_PWM_ALLOWED)? LOWEST_PWM_ALLOWED:pwm_duty_cycle;
    pwm_duty_cycle = (pwm_duty_cycle > HIGHEST_PWM_ALLOWED)? HIGHEST_PWM_ALLOWED:pwm_duty_cycle;

    wheel_lt.pwm_speed = pwm_duty_cycle;
    wheel_rt.pwm_speed = pwm_duty_cycle;
    softPwmWrite(wheel_lt.pwm_pin, wheel_lt.pwm_speed);
    softPwmWrite(wheel_rt.pwm_pin, wheel_rt.pwm_speed);

    return 0;
}

static int set_direction_stop(int not_used){
    int rc = 0;
    hard_stop();
    //soft_stop();
    delay(120); // 30msec
    drive_train.direction = stop;
    return rc;
}

static int set_direction_forward(int not_used){
    printf("    forward\n");
    if (drive_train.direction == forward)
        return 0;
    set_direction_stop(0);
    digitalWrite(wheel_lt.positive_pin, HIGH);
    digitalWrite(wheel_lt.negative_pin, LOW);
    digitalWrite(wheel_rt.positive_pin, HIGH);
    digitalWrite(wheel_rt.negative_pin, LOW);
    soft_start();
    drive_train.direction = forward;
    printf("current pwm: [%d][%d]\n", wheel_lt.pwm_speed, wheel_rt.pwm_speed);

    return 0;
}

static int set_direction_backward(int not_used){
    printf("    backward\n");
    if (drive_train.direction == backward)
        return 0;
    set_direction_stop(0);
    digitalWrite(wheel_lt.positive_pin, LOW);
    digitalWrite(wheel_lt.negative_pin, HIGH);
    digitalWrite(wheel_rt.positive_pin, LOW);
    digitalWrite(wheel_rt.negative_pin, HIGH);
    soft_start();
    drive_train.direction = backward;

    return 0;
}

static int set_direction_left(int degrees){
    printf("    left: %d\n", degrees);
    int last_pwm = wheel_lt.pwm_speed;
    set_direction_stop(0);
    set_speed(wheel_pwm_for_turn);
    digitalWrite(wheel_lt.positive_pin, LOW);
    digitalWrite(wheel_lt.negative_pin, LOW);
    digitalWrite(wheel_rt.positive_pin, HIGH);
    digitalWrite(wheel_rt.negative_pin, LOW);
    delay(degrees_to_millisec(degrees));
    set_direction_stop(0);
    set_speed(last_pwm);
    drive_train.direction = left_turn;

    return 0;
}

static int set_direction_right(int degrees){
    printf("    right: %d\n", degrees);
    int last_pwm = wheel_lt.pwm_speed;
    set_direction_stop(0);
    set_speed(wheel_pwm_for_turn);
    digitalWrite(wheel_lt.positive_pin, HIGH);
    digitalWrite(wheel_lt.negative_pin, LOW);
    digitalWrite(wheel_rt.positive_pin, LOW);
    digitalWrite(wheel_rt.negative_pin, LOW);
    delay(degrees_to_millisec(degrees));
    set_direction_stop(0);
    set_speed(last_pwm);
    drive_train.direction = right_turn;

    return 0;
}

static int degrees_to_millisec(int degrees){
    int millis = 0;
    millis = (degrees < 89) ? 250:600;
    return millis;
}

static int hard_stop(){
    digitalWrite(wheel_lt.positive_pin, LOW);
    digitalWrite(wheel_lt.negative_pin, LOW);
    digitalWrite(wheel_rt.positive_pin, LOW);
    digitalWrite(wheel_rt.negative_pin, LOW);

    return 0;
}

static int soft_start(){
    printf("soft_start\n");
    int i;
    int target_pwm = wheel_lt.pwm_speed;
    for(i=LOWEST_PWM_ALLOWED; i<=target_pwm; i++){
        printf("soft_start i: %d | pwm_speed: %d\n", i, wheel_lt.pwm_speed);
        set_speed(i);
        printf("rc1\n");
        delay(30); // 30msec
    }

    return 0;
}

static int soft_stop(){
    printf("soft_stop\n");
    int i=MIN(wheel_lt.pwm_speed, wheel_rt.pwm_speed);
    //printf("rc1 i:%d\n", i);
    while(i>=LOWEST_PWM_ALLOWED){
        //printf("set_speed: %d\n", i);
        set_speed(i);
        delay(30); // 30msec
        i--;
    }
    //printf("rc2 i:%d\n", i);

    return 0;
}

