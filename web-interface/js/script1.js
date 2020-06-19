var speed_slider = document.getElementById("speed_control");
var speed_display = document.getElementById("speed_val");
speed_display.innerHTML = speed_slider.value;
var speed_min = parseInt(speed_slider.min);
var speed_max = parseInt(speed_slider.max);

var cam_stop = '100.0';
var cam_start = '101.0';

/* Update Battery voltage data */
// setInterval(function () {
//     $('#batt_data').load('/data/batt_volt_data.txt');
// }, 3000);

function change_speed(value){
  var value_int = parseInt(value);
  if (value_int == speed_min || value_int == speed_max){
    return;
  }
  if (value_int < speed_min) {
    value = speed_slider.min;
  }
  if (value_int > speed_max) {
    value = speed_slider.max;
  }

  speed_display.innerHTML = value;
  speed_slider.value = value;
  var val = "5."+value;
  send_user_input(val);
  console.info("Current speed|requested: ", speed_slider.value,"/",speed_display.innerHTML,"|",value);
}

function increase_speed(){
    var val = parseInt(speed_slider.value);
    val += 5;
    change_speed(val);
}
function decrease_speed(){
    var val = parseInt(speed_slider.value);
    val -= 5;
    change_speed(val);
} 

function sleep(ms) {
    return new Promise(resolve => setTimeout(resolve, ms));
}

var d = new Date();
var last_time = 0;
var current_time = 0;
var last_val = 0;
async function send_user_input(value) {
    var request = false;

    if(window.XMLHttpRequest){
        request = new XMLHttpRequest();
    }else if(window.ActiveXObject){
        try {
            request = new ActiveXObject("Msxml2.XMLHTTP");
        } catch(e) {
            request = new ActiveXObject("Msxml.XMLHTTP");
        }
    }
    //XMLHttpRequest failed
    if(!request){
        alert("XMLHttpRequest failed...");
        return false;
    }
    
    current_time = d.getTime();
    var elapsed_time = current_time - last_time;
    if (last_val == value && elapsed_time < 2000) {
      return;
    }
    //query
    var send = 'code=' + value;
    request.open('POST','/php/rcbot-web-command1.php',true);
    //HTML header
    request.setRequestHeader('Content-Type','application/x-www-form-urlencoded');
    // request.setRequestHeader('Content-Length',send.length);
    // request.setRequestHeader('Connection','close');
    request.send(send);
    last_time = d.getTime();
    last_val = value;

    if(value == cam_start){
        await sleep(3000);

        document.getElementById('video_load').style.visibility="visible";
        document.getElementById('video_load').src="php/cam.php";
    }
    // else if(value == cam_stop){
    //    document.getElementById('video_load').style.visibility="hidden";
        // console.log("cam_stop");
    // }

}

////////////////////////////
//                        //
// VOICE RECOGNITION PART //
//                        //
////////////////////////////

var final_transcript = '';
var start_timestamp;
var recognizing = false;
var recognition;
try {
    var SpeechRecognition = window.SpeechRecognition || window.webkitSpeechRecognition;
    recognition = new SpeechRecognition();
  }
  catch(e) {
    console.error(e);
    $('.no-browser-support').show();
    $('.app').hide();
}
console.info("Got speech recognition.");

recognition.onstart = function() { 
    recognizing = true;
    // instructions.text('Voice recognition activated. Try speaking into the microphone.');
}

recognition.onspeechend = function() {
  // instructions.text('You were quiet for a while so voice recognition turned itself off.');
}

recognition.onerror = function(event) {
  if(event.error == 'no-speech') {
    // instructions.text('No speech was detected. Try again.');  
  };
}

recognition.onend = function() {
  recognizing = false;
  start_img.src = 'images/mic.gif'
}

recognition.continuous = true;

// recognition.onresult = (event) => {
recognition.onresult = function(event) {
  // console.info("heard something");
  final_transcript = '';
  var interim_transcript = '';
  for (var i = event.resultIndex; i < event.results.length; ++i) {
    if (event.results[i].isFinal) {
      final_transcript += event.results[i][0].transcript;
    } else {
      interim_transcript += event.results[i][0].transcript;
    }
  }
  // final_transcript = capitalize(final_transcript);
  final_span.innerHTML = linebreak(final_transcript);
  interim_span.innerHTML = linebreak(interim_transcript);

  // console.info(final_transcript);

  if (final_transcript.includes('go forward')) {
    console.info("FORWARD");
    send_user_input('2.0');
  } else if (final_transcript.includes('go back')) {
    console.info("BACKWARD");
    send_user_input('1.0');
  } else if (final_transcript.includes('go left') ||
             final_transcript.includes('turn left')) {
    console.info("LEFT");
    send_user_input('3.45');
  } else if (final_transcript.includes('go right') ||
             final_transcript.includes('turn right')) {
    console.info("RIGHT");
    send_user_input('4.45')
  } else if (final_transcript.includes('stop')) {
    console.info("STOP");
    send_user_input('0.0')
  } else if (final_transcript.includes('decrease speed') || 
             final_transcript.includes('go slow')        ||
             final_transcript.includes('go slower')) {
    console.info("DECREASE SPEED");
    decrease_speed();
  } else if (final_transcript.includes('increase speed') || 
             final_transcript.includes('go fast')        ||
             final_transcript.includes('go faster')) {
    console.info("INCREASE SPEED");
    increase_speed();
  } else if (final_transcript.includes('look up')) {
    console.info("UP");
    send_user_input('104.0');
  } else if (final_transcript.includes('look down')) {
    console.info("DOWN");
    send_user_input('105.0');
  } else if (final_transcript.includes('look left')) {
    console.info("LOOK LEFT");
    send_user_input('102.1');
  } else if (final_transcript.includes('look right')) {
    console.info("LOOK RIGHT");
    send_user_input('103.1');
  } else if (final_transcript.includes('reset camera')) {
    console.info("RESET CAM");
    send_user_input('106.0')
  }

}
var two_line = /\n\n/g;
var one_line = /\n/g;
function linebreak(s) {
  return s.replace(two_line, '<p></p>').replace(one_line, '<br>');
}

var first_char = /\S/;
function capitalize(s) {
  return s.replace(first_char, function(m) { return m.toUpperCase(); });
}

function startSpeechToTxtButton(event) {
  if (recognizing) {
    recognition.stop();
    console.info("Stop recognition");
    final_span.innerHTML = linebreak('');
    return;
  }
  final_transcript = '';
  recognition.start();
  console.info("Start recognition");
  final_span.innerHTML = '';
  interim_span.innerHTML = '';
  start_img.src = 'images/mic-animate.gif';
  // start_timestamp = event.timeStamp;
}