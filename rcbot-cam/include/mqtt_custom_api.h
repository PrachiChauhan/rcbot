#ifndef _MQTT_CUSTOM_API_H_
#define _MQTT_CUSTOM_API_H_

#include <mosquitto.h>
#include "config.h"
#include "common_utils.h"

struct MQTT_client{
    struct mosquitto* mosq;
    const char* broker_ip;
    int broker_port;
    const char* client_id;
    const char* topic;
    int keepalive;
    int runnable;
};

struct MQTT_client* new_mqtt_client(const char* id, const char* ip, int port, const char* topic, int keepalive);
int mqtt_client_init(struct MQTT_client* mqtt_client);
void mqtt_client_close(struct MQTT_client* mqtt_client);
void mqtt_on_connect(struct mosquitto *mosq, void *obj, int result);
void mqtt_on_disconnect(struct mosquitto *mosq, void *obj, int result);
void mqtt_on_message_received(struct mosquitto *mosq, void *obj, const struct mosquitto_message *message);
//int publish(struct MQTT_client* mqtt_client);

#endif //_MQTT_CUSTOM_API_H_