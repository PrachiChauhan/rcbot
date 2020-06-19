#ifndef _SPEECH_SYNTHESIZER_H_
#define _SPEECH_SYNTHESIZER_H_


#include "config.h"
#include "common_utils.h"

int speech_synthesizer_init();
int speech_synth_cleanup();
int send_msg_to_speaker(int msg_len, char* msg);

#endif //_SPEECH_SYNTHESIZER_H_