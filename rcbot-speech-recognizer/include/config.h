#ifndef _CONFIG_H_
#define _CONFIG_H_

/* Firmware version information */
#define VERSION_MAJOR   1
#define VERSION_MINOR   0
#define VERSION_REV     0
#define VERSION_SIZE    3

/* MQTT related */
#define MQTT_BROKER_IP "192.168.1.43"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC_SPEECH_SYNTH         ("bot/speech_synthesizer")
#define MQTT_TOPIC_DRIVE_TRAIN          ("bot/drivetrain")
#define MQTT_TOPIC_CAM                  ("bot/cam")


/* Sphinx related */
//#define std_en_lm "/usr/local/share/pocketsphinx/model/en-us/en-us"   // path to the standard english language model
//#define std_en_lm "cmusphinx-en-in-5.2/en_in.cd_cont_5000/"   // path to the standard english language model
#define STD_EN_LM "cmusphinx-5prealpha-en-us-ptm-2.0/"
#define LANGUAGE_MODEL "language_model.lm"                                         // custom language model (file must be present)
#define DICTIONARY "dictionary.dic"                                      // custom dictionary (file must be present)
#define GRAMMAR "grammar.jgrf"


#endif // _CONFIG_H_