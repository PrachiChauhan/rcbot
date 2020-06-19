#!/bin/bash

echo "RCBOT BOOTING UP ..."

echo "[STARTING] RCBOT-DRIVETRAIN-CONTROLLER"
rcbot-drivetrain-controller &

echo "[STARTING] RCBOT-SENSORS-CONTROLLER"
rcbot-sensors-controller &

# echo "[STARTING] RCBOT-SPEECH-RECOGNIZER"
# rcbot-speech-recognizer &

echo "[STARTING] RCBOT-SPEECH-SYNTHESIZER"
rcbot-speech-synthesizer &

echo "[STARTING] RCBOT-CAM"
rcbot-cam &>/dev/null &
