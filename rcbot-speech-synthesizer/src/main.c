#include "mqtt_custom_api.h"
#include "speech_synthesizer.h"

struct MQTT_client* mqtt_client;

void handle_signal(int s)
{
    mqtt_client->runnable = 0;
    speech_synth_cleanup();
    printf("[EXITING] RCBOT-SPEECH-SYNTHESIZER !!!\n");
    exit(0);
}

int main(void){
    printf("[INITIALIZING] RCBOT-SPEECH-SYNTHESIZER...\n");

    /* Connect signals */
    signal(SIGINT, handle_signal);
    signal(SIGTERM, handle_signal);

    /* Initialize speech synthesizer */
    speech_synthesizer_init();

    /* Initialize and start listening on mqtt client */
    mqtt_client = new_mqtt_client(MQTT_CLIENT_ID, MQTT_BROKER_IP, MQTT_BROKER_PORT,
                                MQTT_TOPIC_SUBSCRIBE, 60);  //id_ss: id_speech_synthesizer
    mqtt_client_init(mqtt_client);


    return 0;
}