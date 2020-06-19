#include "config.h"
#include "common_utils.h"
#include "rc_ads1115.h"
#include "battery_voltage_sensor.h"

#define ads1115_reading_to_batt_voltage(_x) ((_x)*(2))

static void analyze_battery_voltage(uint16_t volt_data);
static char* prepare_msg(char* msg_prefix, float voltage);
static void log_data(char* data);
static void alert_user(char* msg);
static char* prepare_cmd(char* msg);

void* monitor_battery_voltage(void* th_args){
    struct batt_vol_th_args * args = (struct batt_vol_th_args*) th_args;
    struct ads1115_module* ads1115 = ads1115_init(ADS1115_BATT_SENSOR_ADDR, ADS1115_REG_CONFIG_PGA_6_144V);
    uint16_t volt_data = 0;

    while(args->runnable){
        volt_data = read_adc_single_ended(ads1115, ADS1115_BATT_SENSOR_CHANNEL);
        analyze_battery_voltage(volt_data);
        delay(VOLTAGE_POLLING_FREQ*1000);
    }
    pthread_exit (NULL);
}

static void analyze_battery_voltage(uint16_t volt_data){
    char* msg_to_usr;
    float voltage = (float)volt_data*ADS1115_PGA_6_144V_VAL/ADS1115_MAX_READING_COUNT;
    voltage = ads1115_reading_to_batt_voltage(voltage);

    if(voltage <= BATT_DEAD_VOLTAGE){
        msg_to_usr = prepare_msg("Battery dead, ", voltage);
        alert_user(msg_to_usr);
    }else if(voltage <= BATT_CRITICALLY_LOW_VOLTAGE){
        msg_to_usr = prepare_msg("Battery critically low, ", voltage);
        alert_user(msg_to_usr);
    }else if(voltage <= BATT_LOW_VOLTAGE){
        msg_to_usr = prepare_msg("Battery low, ", voltage);
        alert_user(msg_to_usr);
    }else{
        printf("Voltage: %f\n", voltage);
        msg_to_usr = prepare_msg("Battery Voltage: ", voltage);
    }
    log_data(msg_to_usr);
}

static void log_data(char* data){
    FILE *fp;
    fp = fopen(BATT_DATA_FILE, "w");
    fprintf(fp, data);
    fclose(fp);
}

static char* prepare_msg(char* msg_prefix, float voltage){
    int bufsize = 4; //No. of characters in voltage '0.00' until 2 decimal places including decimal
    int nullchar_len = 1; //'\0'
    char* msg_suffix = " volts";
    int msg_prefix_len = strlen(msg_prefix);
    int msg_suffix_len = strlen(msg_suffix);
    char buffer[bufsize];
    snprintf(buffer, bufsize, "%f", voltage);
    int msg_len = strlen(buffer);
    int final_msg_len = msg_prefix_len+bufsize+msg_suffix_len+nullchar_len;

    char* final_msg = (char*)malloc(sizeof(char)*final_msg_len);

    memcpy(final_msg, msg_prefix, msg_prefix_len);
    memcpy(final_msg+msg_prefix_len, buffer, msg_len);
    memcpy(final_msg+msg_prefix_len+msg_len, msg_suffix, msg_suffix_len+nullchar_len);

    return final_msg;
}


static void alert_user(char* msg){
    printf("%s: %s\n", __func__, msg);
    system(prepare_cmd(msg));
}

static char* prepare_cmd(char* msg){
    int nullchar_len = 1; //'\0'
    char* cmd_p1 = "mosquitto_pub -h ";
    char* cmd_p2 = MQTT_BROKER_IP;
    char* cmd_p3 = " -m \"";
    char* cmd_p4 = msg;
    char* cmd_p5 = "\" -t ";
    char* cmd_p6 = MQTT_TOPIC_SPEECH_SYNTH;
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