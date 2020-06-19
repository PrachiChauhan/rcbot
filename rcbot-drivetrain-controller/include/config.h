#ifndef _CONFIG_H_
#define _CONFIG_H_

/* Firmware version information */
#define VERSION_MAJOR   1
#define VERSION_MINOR   0
#define VERSION_REV     0
#define VERSION_SIZE    3

/* Drive motor information */
#define LOWEST_PWM_ALLOWED 20
#define HIGHEST_PWM_ALLOWED 100
#define wheel_pos_pin_rt 4   // BCM-23 in1
#define wheel_neg_pin_rt 5   // BCM-24 in2
#define wheel_pwm_pin_rt 6   // BCM-25 enA
#define wheel_pos_pin_lt 3   // BCM-22 in4
#define wheel_neg_pin_lt 2   // BCM-27 in3
#define wheel_pwm_pin_lt 0   // BCM-17 enB
#define wheel_pwm_default_duty_cycle (LOWEST_PWM_ALLOWED+5)
#define wheel_pwm_for_turn (HIGHEST_PWM_ALLOWED-20)

/* MQTT related */
#define MQTT_BROKER_IP "192.168.1.43"
#define MQTT_BROKER_PORT 1883
#define MQTT_CLIENT_ID "id_dt"
#define MQTT_TOPIC_SUBSCRIBE "bot/drivetrain"

#endif // _CONFIG_H_