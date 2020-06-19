#ifndef _COMMON_UTILS_H_
#define _COMMON_UTILS_H_

#include <stdio.h>      // For printf()
#include <signal.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>     // For sleep()
#include <stdlib.h>     // For exit()
#include <wiringPi.h>
#include <softPwm.h>

#define MIN(a,b) ((a<b)?(a):(b))
#define MAX(a,b) ((a>b)?(a):(b))

//enum mybool {false = 0, true = 1};


#endif // _COMMON_UTILS_H_