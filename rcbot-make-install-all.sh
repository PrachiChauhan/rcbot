#!/bin/bash

echo '[BUILDING] RCBOT-DRIVETRAIN-CONTROLLER'
cd /home/pi/Robot/2019/rcbot/rcbot-drivetrain-controller
make clean && make 
echo '[INSTALLING] RCBOT-DRIVETRAIN-CONTROLLER'
sudo make install
cd ..
echo '--------------------------------------------------------------'
echo ''

echo '[BUILDING] RCBOT-SPEECH-SYNTHESIZER'
cd /home/pi/Robot/2019/rcbot/rcbot-speech-synthesizer
make clean && make 
echo '[INSTALLING] RCBOT-SPEECH-SYNTHESIZER'
sudo make install
cd ..
echo '--------------------------------------------------------------'
echo ''

echo '[BUILDING] RCBOT-SPEECH-RECOGNIZER'
cd /home/pi/Robot/2019/rcbot/rcbot-speech-recognizer
make clean && make 
echo '[INSTALLING] RCBOT-SPEECH-RECOGNIZER'
sudo make install
cd ..
echo '--------------------------------------------------------------'
echo ''

echo '[BUILDING] RCBOT-SENSORS-CONTROLLER'
cd /home/pi/Robot/2019/rcbot/rcbot-sensors-controller
make clean && make 
echo '[INSTALLING] RCBOT-SENSORS-CONTROLLER'
sudo make install
cd ..
echo '--------------------------------------------------------------'
echo ''

echo '[BUILDING] RCBOT-CAM'
cd /home/pi/Robot/2019/rcbot/rcbot-cam
make clean && make 
echo '[INSTALLING] RCBOT-CAM'
sudo make install
cd ..
echo '--------------------------------------------------------------'
echo ''

exit 0