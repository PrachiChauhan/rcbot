#include <time.h>
#include "cam_controller.h"

#define default_degree_change (10)
#define NOT_USED (0)

static int cam_stop(int not_used);
static int cam_start(int not_used);
static int send_system_cmd(char* cmd);
static int cam_pan_left(int not_used);
static int cam_pan_right(int not_used);
static int cam_tilt_up(int not_used);
static int cam_tilt_down(int not_used);
static int cam_pan_sweep(int not_used);
static int cam_tilt_sweep(int not_used);
static int reset(int not_used);
static int goto_position_smooth(int pin, int position);
static int goto_position_direct(int pin, int position);
static void stabilize();
static void update_servo_position(int pin, int positoin);

static int (*cam_fptr[])(int) = { cam_stop,
                                  cam_start,
                                  cam_pan_left,
                                  cam_pan_right,
                                  cam_tilt_up,
                                  cam_tilt_down,
                                  reset,
                                  cam_pan_sweep,
                                  cam_tilt_sweep};

servo_config_t tilt_servo;
servo_config_t pan_servo;

int cam_init(){

    tilt_servo.pwm_pin = servo_tilt_pwm_pin;
    tilt_servo.default_position = TILT_DEFAULT;
    tilt_servo.current_position = tilt_servo.default_position;
    pan_servo.pwm_pin = servo_pan_pwm_pin;
    pan_servo.default_position = PAN_DEFAULT;
    pan_servo.current_position = pan_servo.default_position;

    if (wiringPiSetup() == -1) {
        printf("Failed to setup Wiring Pi!\n");
        return 1;
    }

    pinMode(tilt_servo.pwm_pin, OUTPUT);
    pinMode(pan_servo.pwm_pin, OUTPUT);
    softPwmCreate (tilt_servo.pwm_pin, position_to_pwm(TILT_DEFAULT), 100);
    softPwmCreate (pan_servo.pwm_pin, position_to_pwm(PAN_DEFAULT), 100);
    stabilize(tilt_servo.pwm_pin);
    stabilize(pan_servo.pwm_pin);

    cam_start(NOT_USED);

    return 0;
}

int cam_operation(int command_len, char* command){
    int rc = 0;
    int op_index = 0;
    int op_val = 0;
    const char* delim = ".";

    op_index = atoi((const char*)strtok(command, delim));
    printf("    change_cam_status %d|%d\n", op_index, op_val);
    op_index = cam_op_enum_to_op_translate(op_index);
    op_val = atoi((const char*)strtok(NULL, delim));

    if (op_index < sizeof(cam_fptr)/sizeof(*cam_fptr)){
        cam_fptr[op_index](op_val);
    }

    return rc;
}

int cam_cleanup(){
    int rc = 0;
    cam_stop(0);
    return rc;
}

/* Functions specific to camera */

static int cam_stop(int not_used){
    int rc = 0;
    printf("Stopping the camera feed\n");
    char* cmd = "killall -9 mjpg_streamer";

    send_system_cmd(cmd);

    return rc;
}

static int cam_start(int not_used){
    int rc = 0;
    printf("Starting the camera feed\n");
    char* cmd = "/usr/local/bin/mjpg_streamer -i \
                \"/usr/local/lib/mjpg-streamer/input_raspicam.so -fps 11 -y 380 \
                -quality 10 -timestamp\" -o \"/usr/local/lib/mjpg-streamer/output_http.so \
                -w /var/www/html -p 8080\" & >/dev/null 2>&1";

    send_system_cmd(cmd);

    return rc;
}

static int send_system_cmd(char* cmd){
    int rc = 0;
    system(cmd);
    return rc;
}

/* Functions specific to servo motor control */
/* High level camera pan and tilt functions */
static int cam_pan_left(int precision_fine){
    printf("    %s \n", __func__);
    int rc = 0;
    int degree_change = default_degree_change;
    if (pan_servo.current_position >= PAN_MAX){
        return 0;
    }else{
        pan_servo.current_position += degree_change;
    }
    update_servo_position(pan_servo.pwm_pin, pan_servo.current_position);
    return rc;
}

static int cam_pan_right(int precision_fine){
    printf("    %s \n", __func__);
    int rc = 0;
    if (pan_servo.current_position <= PAN_MIN){
        return rc;
    }else{
        pan_servo.current_position -= default_degree_change;
    }
    update_servo_position(pan_servo.pwm_pin, pan_servo.current_position);

    return rc;
}

static int cam_tilt_up(int not_used){
    printf("    %s \n", __func__);
    int rc = 0;
    if (tilt_servo.current_position >= TILT_MAX){
        return rc;
    }else{
        tilt_servo.current_position += default_degree_change;
    }
    update_servo_position(tilt_servo.pwm_pin, tilt_servo.current_position);

    return rc;
}

static int cam_tilt_down(int not_used){
    printf("    %s \n", __func__);
    int rc = 0;
    if (tilt_servo.current_position <= TILT_MIN){
        return rc;
    }else{
        tilt_servo.current_position -= default_degree_change;
    }
    update_servo_position(tilt_servo.pwm_pin, tilt_servo.current_position);

    return rc;
}


static int cam_pan_sweep(int not_used){
    int rc = 0;

    return rc;
}

static int cam_tilt_sweep(int not_used){
    int rc = 0;

    return rc;
}

static int reset(int not_used) {
    printf("    %s \n", __func__);
    int rc = 0;
    update_servo_position(pan_servo.pwm_pin, pan_servo.default_position);
    update_servo_position(tilt_servo.pwm_pin, tilt_servo.default_position);
    return rc;
}

/* Low level servo controls */

#define NANO_TO_SEC (1e-9)
#define SEC_TO_NANO (1e9)

static int goto_position_smooth(int pin, int dest_pos){
    int rc = 0;
    double last_time, current_time;
    double elapsed_duration = 0;
    double total_tween_duration = 1 * SEC_TO_NANO;
    struct timespec spec;
    int interim_pos;
    int orig_pos = (pin == pan_servo.pwm_pin) ? pan_servo.current_position : tilt_servo.current_position;
    int diff_pos = dest_pos - orig_pos;

    clock_gettime(CLOCK_MONOTONIC, &spec);
    current_time = spec.tv_sec * SEC_TO_NANO; 
    current_time += spec.tv_nsec;
    last_time = current_time;
    while(elapsed_duration <= total_tween_duration) {
        //do easing
        //interim_pos = c * (t/d) + b;
        //from http://blog.moagrius.com/actionscript/jsas-understanding-easing/
        // @t is the current time (or position) of the tween. This can be seconds or frames, steps, seconds, ms, whatever – as long as the unit is the same as is used for the total time [3].
        // @b is the beginning value of the property.
        // @c is the change between the beginning and destination value of the property.
        // @d is the total time of the tween.
        interim_pos = (int)(diff_pos * (elapsed_duration/total_tween_duration) + orig_pos);

        softPwmWrite(pin, position_to_pwm(interim_pos));
        stabilize(pin);
        //update elapsed time
        clock_gettime(CLOCK_MONOTONIC, &spec);
        current_time = spec.tv_sec * SEC_TO_NANO; 
        current_time += spec.tv_nsec;
        elapsed_duration = current_time - last_time;
        last_time = current_time;
    }


    return rc;
}


static int goto_position_direct(int pin, int position){
    int rc = 0;
    int servo_pwm = position_to_pwm(position);
    printf("    %s: pwm: %d | pos: %d\n", __func__, servo_pwm , position);
    softPwmWrite(pin, servo_pwm);

    return rc;
}

static void update_servo_position(int pin, int position){
    goto_position_direct(pin, position);
    // goto_position_smooth(pin, position);
    stabilize(pin);
}

static void stabilize(int pin){
    delay(STEP_DELAY); //millisecs
    softPwmWrite(pin, 0);
}