#ifndef _CONFIG_H_
#define _CONFIG_H_

/* Firmware version information */
#define VERSION_MAJOR        (1)
#define VERSION_MINOR        (0)
#define VERSION_REV          (0)
#define VERSION_SIZE         (3)

/* Servo motor related */
#define LOWEST_PWM_ALLOWED   (0)
#define HIGHEST_PWM_ALLOWED  (100)
#define servo_tilt_pwm_pin   (15)   // BCM-14
#define servo_pan_pwm_pin    (16)   // BCM-15
#define TILT_DEFAULT         (2)   // All the following values in degrees
#define TILT_MIN             (0)
#define TILT_MAX             (180)
#define PAN_DEFAULT          (70)
#define PAN_MIN              (0)
#define PAN_MAX              (180)
#define STEP_DELAY			 (150) //(millis)found after experimenting with different values. at 500 motors jitter, at 50 not enough time to compete the step

/* MQTT related */
#define MQTT_BROKER_IP       ("192.168.1.43")
#define MQTT_BROKER_PORT     (1883)
#define MQTT_CLIENT_ID       ("id_cam")
#define MQTT_TOPIC_SUBSCRIBE ("bot/cam")


#endif // _CONFIG_H_