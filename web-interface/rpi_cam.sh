#!/bin/bash

[ -z "$1" ] && echo 'Enter start/stop' && exit 1

if [ $1 == "start" ]; then
    echo 'Start video' >>/tmp/1.log
    mjpg_streamer -i "input_raspicam.so -fps 10 -quality 95 -timestamp" -o "output_http.so -w /var/www/html -p 8080"
fi

if [ $1 == "stop" ]; then
    echo 'Stop video' >>/tmp/1.log
    killall -9 mjpg_streamer
fi


#mjpg_streamer -i "input_uvc.so -f 10 -r 320x240 -d /dev/video0 -y" -o "output_http.so -w /var/www/html -p 8080"

#./mjpg_streamer -o "output_http.so -w ./www" -i "input_raspicam.so"

#mjpg_streamer -i "input_raspicam.so" -o "output_http.so -w /var/www/html -p 8080"