#include "speech_recognizer.h"
#include "command_processor.h"

ps_decoder_t *ps;                  // create pocketsphinx decoder structure
cmd_ln_t *sr_config;               // create configuration structure
ad_rec_t *ad;                      // create audio recording structure - for use with ALSA functions

int16 adbuf[4096];                 // buffer array to hold audio data
uint8 utt_started, in_speech;      // flags for tracking active speech - has speech started? - is speech currently happening?
int32 k;                           // holds the number of frames in the audio buffer
char const *decoded_speech;        // pointer to "hypothesis" (best guess at the decoded result)

static void recognize_from_microphone();

int speech_recognizer_init(){
    sr_config = cmd_ln_init(NULL, ps_args(), TRUE,  // Load the configuration structure - ps_args() passes the default values
                    "-hmm", STD_EN_LM,
                    "-lm", LANGUAGE_MODEL,
                    "-dict", DICTIONARY,
                    // "-jsgf", GRAMMAR,
                    "-logfn", "/dev/null",          // suppress log info from being sent to screen
                    NULL);

    if (sr_config == NULL) {
        printf("Failed to create config object, see log for details\n");
        return -1;
    }

    ps = ps_init(sr_config);
    if (ps == NULL) {
        printf("Failed to create recognizer, see log for details\n");
        return -1;
    }
    ad = ad_open_dev("plughw:0", (int) cmd_ln_float32_r(sr_config, "-samprate")); // open default microphone at default samplerate
    // ad = ad_open_dev("sysdefault", (int) cmd_ln_float32_r(sr_config, "-samprate")); // open default microphone at default samplerate

    while(1){
        recognize_from_microphone();                 // call the function to capture and decode speech
        printf("You Said: %s\n", decoded_speech);    // send decoded speech to screen
        process_voice_input(decoded_speech);
    }

    ad_close(ad);                                    // close the microphone
}

static void recognize_from_microphone(){
    ad_start_rec(ad);                                // start recording
    ps_start_utt(ps);                                // mark the start of the utterance
    utt_started = FALSE;                             // clear the utt_started flag

    printf("Speech Recognizer listening...\n");
    while(1) {
        k = ad_read(ad, adbuf, 4096);                // capture the number of frames in the audio buffer
        ps_process_raw(ps, adbuf, k, FALSE, FALSE);  // send the audio buffer to the pocketsphinx decoder

        in_speech = ps_get_in_speech(ps);            // test to see if speech is being detected

        if (in_speech && !utt_started) {             // if speech has started and utt_started flag is false
            printf("Speech detected\n");
            utt_started = TRUE;                      // then set the flag
        }

        if (!in_speech && utt_started) {             // if speech has ended and the utt_started flag is true
            ps_end_utt(ps);                          // then mark the end of the utterance
            ad_stop_rec(ad);                         // stop recording
            printf("Getting hypothesis now\n");
            decoded_speech = ps_get_hyp(ps, NULL );  // query pocketsphinx for "hypothesis" of decoded statement
            break;                                   // exit the while loop and return to main
        }
    }
}
