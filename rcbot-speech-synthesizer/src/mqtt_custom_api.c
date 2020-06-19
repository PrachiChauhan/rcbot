#include "mqtt_custom_api.h"

/*
 * Creates and returns a new MQTT_client structure
 * is is the caller's responsibility to free memory later.
 */
struct MQTT_client* new_mqtt_client(const char* id, const char* ip, int port, const char* topic, int keepalive){
    struct MQTT_client* tmp = malloc(sizeof(struct MQTT_client));
    tmp->broker_ip   = ip;
    tmp->broker_port = port;
    tmp->topic       = topic;
    tmp->keepalive   = keepalive;
    //memset(client_id, 0, 7*sizeof(char));
    //snprintf(client_id, 7, "clt_%d", getpid());
    tmp->client_id   = id;
    return tmp;
}

int mqtt_client_init(struct MQTT_client* mqtt_client){
    struct mosquitto* mosq;
    int rc = 0;


    mqtt_client->runnable = 1;
    mqtt_client->keepalive = 60;
    mosquitto_lib_init();
    mosq = mosquitto_new(mqtt_client->client_id, true, mqtt_client);
    mqtt_client->mosq = mosq;
    if(mqtt_client->mosq){
        mosquitto_connect_callback_set(mqtt_client->mosq, mqtt_on_connect);
        mosquitto_disconnect_callback_set(mqtt_client->mosq, mqtt_on_disconnect);
        mosquitto_message_callback_set(mqtt_client->mosq, mqtt_on_message_received);

        rc = mosquitto_connect(mqtt_client->mosq, mqtt_client->broker_ip,
                               mqtt_client->broker_port, mqtt_client->keepalive);
        mosquitto_subscribe(mqtt_client->mosq, NULL, mqtt_client->topic, 0);

        while(mqtt_client->runnable){
            rc = mosquitto_loop(mosq, -1, 1);
            if(mqtt_client->runnable && rc!=0){
                printf("rc: %d | Connection Error !!!\n", rc);
                sleep(10);
                mosquitto_reconnect(mosq);
            }
        }
        mqtt_client_close(mqtt_client);
    }
    return 0;
}

void mqtt_client_close(struct MQTT_client* mqtt_client){
    mosquitto_disconnect(mqtt_client->mosq);
    mosquitto_destroy(mqtt_client->mosq);
    mosquitto_lib_cleanup();
}

void mqtt_on_connect(struct mosquitto *mosq, void *obj, int failed){
    printf("Client '%s' connected %s\n",
        ((struct MQTT_client*)obj)->client_id, (failed)?"with error":"successfully");
    printf("MQTT client %s listening...\n", ((struct MQTT_client*)obj)->client_id);

}
void mqtt_on_disconnect(struct mosquitto *mosq, void *obj, int failed){
    printf("Client '%s' disconnected %s\n",
        ((struct MQTT_client*)obj)->client_id, (failed)?"with error":"successfully");
}
void mqtt_on_message_received(struct mosquitto *mosq, void *obj, const struct mosquitto_message *message){
    int rc = 0;
    printf("Message '%.*s' | Topic '%s'\n", message->payloadlen, (char*) message->payload, message->topic);
    rc = send_msg_to_speaker(message->payloadlen, (char*)message->payload);
    //rc = change_driving_direction(message->payloadlen, (char*) message->payload);
    if (rc != 0)
        printf("    Send message to speaker failed\n");
}

//int publish(struct MQTT_client* mqtt_client);