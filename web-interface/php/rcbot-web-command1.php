<?php
$code       = $_POST['code'];
$mqtt_broker_ip = '192.168.1.43';
#$mqtt_broker_ip = '192.168.1.101';
$code_int = floor(((int)$code)/100);

switch($code_int){
    case "0": $mqtt_topic = 'bot/drivetrain'; # 0-99: drivetrain ops
    break;
    case "1": $mqtt_topic = 'bot/cam';        # 100-199: cam ops
    break;
    default:
}

system("mosquitto_pub -h $mqtt_broker_ip -m $code -t $mqtt_topic");

?>
