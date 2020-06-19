#ifndef _BATTERY_VOLTAGE_SENSOR_H_
#define _BATTERY_VOLTAGE_SENSOR_H_

#include <pthread.h>

struct batt_vol_th_args{
    int runnable;
    pthread_mutex_t runnable_lock;
};

void* monitor_battery_voltage(void* th_args);



#endif //_BATTERY_VOLTAGE_SENSOR_H_