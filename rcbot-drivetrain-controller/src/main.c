#include "mqtt_custom_api.h"
#include "drive_train_controller.h"

struct MQTT_client* mqtt_client;

void handle_signal(int s)
{
    mqtt_client->runnable = 0;
    drive_train_cleanup();
    printf("[EXITING] RCBOT-DRIVETRAIN-CONTROLLER !!!\n");
    exit(0);
}

int main(void){
    printf("[INITIALIZING] RCBOT-DRIVETRAIN-CONTROLLER...\n");

    /* Connect signals */
    signal(SIGINT, handle_signal);
    signal(SIGTERM, handle_signal);

    /* Initialize drive train */
    drive_train_init();

    /* Initialize and start listening on mqtt client */
    mqtt_client = new_mqtt_client(MQTT_CLIENT_ID, MQTT_BROKER_IP, MQTT_BROKER_PORT,
                                MQTT_TOPIC_SUBSCRIBE, 60);
    mqtt_client_init(mqtt_client);


    return 0;
}