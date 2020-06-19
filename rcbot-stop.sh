#!/bin/bash

echo "[STOPPING] RCBOT-DRIVETRAIN-CONTROLLER"
kill -SIGTERM $(ps aux |grep '[r]cbot-drivetrain-controller' |awk '{print $2}')

echo ''
echo "[STOPPING] RCBOT-SENSORS-CONTROLLER"
kill -SIGTERM $(ps aux |grep '[r]cbot-sensors-controller' |awk '{print $2}')

# echo ''
# echo "[STOPPING] RCBOT-SPEECH-RECOGNIZER"
# kill -SIGTERM $(ps aux |grep '[r]cbot-speech-recognizer' |awk '{print $2}')

echo ''
echo "[STOPPING] RCBOT-SPEECH-SYNTHESIZER"
kill -SIGTERM $(ps aux |grep '[r]cbot-speech-synthesizer' |awk '{print $2}')

echo ''
echo "[STOPPING] RCBOT-CAM"
kill -SIGTERM $(ps aux |grep '[r]cbot-cam' |awk '{print $2}')