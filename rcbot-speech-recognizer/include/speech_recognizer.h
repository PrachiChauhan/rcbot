#ifndef _SPEECH_RECOGNIZER_H_
#define _SPEECH_RECOGNIZER_H_

#include "common_utils.h"
#include "config.h"
#include <pocketsphinx.h>
#include <sphinxbase/ad.h>
#include <sphinxbase/err.h>
#include "command_processor.h"

int speech_recognizer_init();

#endif //_SPEECH_RECOGNIZER_H_