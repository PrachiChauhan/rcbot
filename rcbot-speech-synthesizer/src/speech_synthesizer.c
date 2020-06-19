#include "speech_synthesizer.h"

/* Prepare for pico2wave to run
 * pico2wave accepts only .wav file as output
 * so to redirect it to stdout, have to create symlink
 */
int speech_synthesizer_init(){
    system("ln -sf /dev/stdout /tmp/pico2wave.wav");
    system("amixer set Speaker -- 100%");
    char* init_msg = "Hi Ruchir I am your robot";
    send_msg_to_speaker(strlen(init_msg), init_msg);
    return 0;
}

int speech_synth_cleanup(){
    const char* cmd = "rm -rf /tmp/pico2wave.wav";
    system(cmd);
    return 0;
}

int send_msg_to_speaker(int msg_len, char* msg){
    int rc = 0;
    int nullchar_len = 1; //'\0'
    /* Prepare message to send to speaker */
    char* msg_prefix = "pico2wave -w /tmp/pico2wave.wav \"";
    char* msg_suffix = "\" |aplay -D plughw:1,0";
    int msg_prefix_len = strlen(msg_prefix);
    int msg_suffix_len = strlen(msg_suffix);
    char* final_msg = (char*)malloc(msg_prefix_len+msg_len+msg_suffix_len+nullchar_len);

    memcpy(final_msg, msg_prefix, msg_prefix_len);
    memcpy(final_msg+msg_prefix_len, msg, msg_len);
    memcpy(final_msg+msg_prefix_len+msg_len, msg_suffix, msg_suffix_len+nullchar_len);

    printf("Cmd: %s\n", final_msg);

    system(final_msg);

    return rc;
}