#!/bin/bash

echo '[UNINSTALLING] RCBOT-DRIVETRAIN-CONTROLLER'
cd /home/pi/Robot/2019/rcbot/rcbot-drivetrain-controller
sudo make uninstall
cd ..
echo ''

echo '[UNINSTALLING] RCBOT-SPEECH-SYNTHESIZER'
cd /home/pi/Robot/2019/rcbot/rcbot-speech-synthesizer
sudo make uninstall
cd ..
echo ''

echo '[UNINSTALLING] RCBOT-SENSORS-CONTROLLER'
cd /home/pi/Robot/2019/rcbot/rcbot-sensors-controller
sudo make uninstall
cd ..
echo ''

echo '[UNINSTALLING] RCBOT-CAM'
cd /home/pi/Robot/2019/rcbot/rcbot-cam
sudo make uninstall
cd ..
echo ''

exit 0