#include "command_processor.h"

static char* prepare_cmd(char* msg, char* mqtt_topic);
static void send_cmd(char* msg, char* mqtt_topic);
static void voice_input_to_cmd(char const* voice_input, char** cmd, char** mqtt_topic);

void process_voice_input(char const* input){
    char* cmd = "";
    char* mqtt_topic = "";
    voice_input_to_cmd(input, &cmd, &mqtt_topic);
    printf("Voice_Input: %s | cmd: %s | topic: %s\n", input, cmd, mqtt_topic);                               // send decoded speech to screen
    if (strcmp(cmd, "no_cmd") != 0){
        send_cmd(cmd, mqtt_topic);
    }
    // int max_substrings = 10;
    // int i = 0;
    // char* delimiter = " ";
    // char* substring[max_substrings];
    // char* token = strtok(input, delimiter);
    // substring[0] = token;
    // while(token != NULL || i < max_substrings){
    //  token = strok(NULL, delimiter);
    //  substring[i++] = token;
    // }
}

// static char* prepare_msg(char* msg_prefix, float voltage){
//     int bufsize = 4; //No. of characters in voltage '0.00' until 2 decimal places including decimal
//     int nullchar_len = 1; //'\0'
//     char* msg_suffix = " volts";
//     int msg_prefix_len = strlen(msg_prefix);
//     int msg_suffix_len = strlen(msg_suffix);
//     char buffer[bufsize];
//     snprintf(buffer, bufsize, "%f", voltage);
//     int msg_len = strlen(buffer);
//     int final_msg_len = msg_prefix_len+bufsize+msg_suffix_len+nullchar_len;

//     char* final_msg = (char*)malloc(sizeof(char)*final_msg_len);

//     memcpy(final_msg, msg_prefix, msg_prefix_len);
//     memcpy(final_msg+msg_prefix_len, buffer, msg_len);
//     memcpy(final_msg+msg_prefix_len+msg_len, msg_suffix, msg_suffix_len+nullchar_len);

//     return final_msg;
// }

static void voice_input_to_cmd(char const* voice_input, char** cmd, char** mqtt_topic){
    char* tmpchar = "";
    if (strcmp(voice_input, "GO RIGHT")==0){
        tmpchar = "4.45";
    }else if (strcmp(voice_input, "GO LEFT")==0){
        tmpchar = "3.45";
    }else if (strcmp(voice_input, "GO FORWARD")==0){
        tmpchar = "2.0";
    }else if (strcmp(voice_input, "GO BACKWARD")==0){
        tmpchar = "1.0";
    }else if (strcmp(voice_input, "STOP")==0){
        tmpchar = "0.0";
    }
    else{
        tmpchar = "no_cmd";
    }
    *cmd = tmpchar;
    *mqtt_topic = MQTT_TOPIC_DRIVE_TRAIN;            
}

static void send_cmd(char* msg, char* mqtt_topic){
    printf("%s: %s \n", __func__, msg);
    char* final_msg = prepare_cmd(msg, mqtt_topic);
    printf("%s: %s\n", __func__, final_msg);
    system(final_msg);
}

static char* prepare_cmd(char* msg, char* mqtt_topic){
    int nullchar_len = 1; //'\0'
    char* cmd_p1 = "mosquitto_pub -h ";
    char* cmd_p2 = MQTT_BROKER_IP;
    char* cmd_p3 = " -m \"";
    char* cmd_p4 = msg;
    char* cmd_p5 = "\" -t ";
    char* cmd_p6 = mqtt_topic;
    int p1_len = strlen(cmd_p1);
    int p2_len = strlen(cmd_p2);
    int p3_len = strlen(cmd_p3);
    int p4_len = strlen(cmd_p4);
    int p5_len = strlen(cmd_p5);
    int p6_len = strlen(cmd_p6);
    char* cmd = (char*)malloc(p1_len + p2_len + p3_len + p4_len + p5_len + p6_len+nullchar_len);

    memcpy(cmd, cmd_p1, p1_len);
    memcpy(cmd+p1_len, cmd_p2, p2_len);
    memcpy(cmd+p1_len+p2_len, cmd_p3, p3_len);
    memcpy(cmd+p1_len+p2_len+p3_len, cmd_p4, p4_len);
    memcpy(cmd+p1_len+p2_len+p3_len+p4_len, cmd_p5, p5_len);
    memcpy(cmd+p1_len+p2_len+p3_len+p4_len+p5_len, cmd_p6, p6_len+nullchar_len);

    return cmd;
}