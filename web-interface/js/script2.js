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

  } else if (final_transcript.includes('go back')) {
    console.info("BACKWARD");

  } else if (final_transcript.includes('go left')) {
    console.info("LEFT");
    
  } else if (final_transcript.includes('go right')) {
    console.info("RIGHT");
    
  } else if (final_transcript.includes('decrease speed')) {
    console.info("DECREASE SPEED");
    
  } else if (final_transcript.includes('increase speed')) {
    console.info("INCREASE SPEED");
    
  } else if (final_transcript.includes('look up')) {
    console.info("UP");
    
  } else if (final_transcript.includes('look down')) {
    console.info("DOWN");
    
  } else if (final_transcript.includes('reset camera')) {
    console.info("RESET CAM");
    
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