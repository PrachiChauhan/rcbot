#ifndef _CONFIG_H_
#define _CONFIG_H_

/* Firmware version information */
#define VERSION_MAJOR                   (1)
#define VERSION_MINOR                   (0)
#define VERSION_REV                     (0)
#define VERSION_SIZE                    (3)

/* MQTT related */
#define MQTT_BROKER_IP                  ("192.168.1.43")
#define MQTT_BROKER_PORT                (1883)
#define MQTT_CLIENT_ID                  ("id_batt")
#define MQTT_TOPIC_SUBSCRIBE            ("bot/sensor/batt")
#define MQTT_TOPIC_SPEECH_SYNTH         ("bot/speech_synthesizer")

/* Battery sensor related */
#define ADS1115_BATT_SENSOR_ADDR        (0x48)  // 1001 000 (ADDR = GND)
#define ADS1115_BATT_SENSOR_CHANNEL     (0)
#define ADS1115_PGA_6_144V_VAL          (6.144)
#define VOLTAGE_POLLING_FREQ            (10)//(60*2) // IN SECONDS
#define BATT_DATA_FILE                  ("/var/www/html/data/batt_volt_data.txt")
// Lipo Battery calibrations
#define LIPO_1_CELL_DEAD_VOLTAGE        (3.0)                           // 3.0v
#define LIPO_2_CELL_DEAD_VOLTAGE        (LIPO_1_CELL_DEAD_VOLTAGE*2)    // 6.0v
#define BATT_DEAD_VOLTAGE               (LIPO_2_CELL_DEAD_VOLTAGE)
#define BATT_CRITICALLY_LOW_VOLTAGE     (BATT_DEAD_VOLTAGE+0.8)         // 6.8v
#define BATT_LOW_VOLTAGE                (BATT_DEAD_VOLTAGE+1.1)         // 7.1v



#endif // _CONFIG_H_