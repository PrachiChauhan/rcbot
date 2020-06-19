#include "mqtt_custom_api.h"
#include "battery_voltage_sensor.h"

struct MQTT_client* mqtt_client;
struct batt_vol_th_args* args;

void handle_signal(int s)
{
    args->runnable = 0;
    printf("[EXITING] RCBOT-SENSORS-CONTROLLER !!!\n");
    exit(0);
}

int main(void){
    printf("[INITIALIZING] RCBOT-SENSORS-CONTROLLER...\n");

    /* Connect signals */
    signal(SIGINT, handle_signal);
    signal(SIGTERM, handle_signal);

    /* Initialize sensors */
    //battery voltage sensor
    args = malloc(sizeof(struct batt_vol_th_args));
    args->runnable = 1;
    pthread_t batt_voltage_sensor_th;
    pthread_create(&batt_voltage_sensor_th, NULL, monitor_battery_voltage, (void*)args);

    pthread_join(batt_voltage_sensor_th, NULL);

    return 0;
}