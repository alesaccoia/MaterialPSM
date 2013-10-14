
// Utilities

function loadSample(destinationBuffer, url) {
  var request = new XMLHttpRequest();
  request.open("GET", url, true);
  request.responseType = "arraybuffer";

  request.onload = function() { 
    destinationBuffer = DMAF.context.createBuffer(request.response, false);
  }
  request.send();
}

// web audio application
var app = {

  musicLooping: false,
  isInitialized: false,
  context: null,
  
  btnSoundBuffer: null,
  loopSoundBuffer: null,
  loopingBufferSource = null,
  8bitSynth: null,
  
  initialize: function() {
    try {
      window.AudioContext = window.AudioContext || window.webkitAudioContext;
      this.context = new AudioContext();
      loadSample(this.btnSoundBuffer, '../pd/assets/btnSnd.wav');
      
      // create the sound buffer for the looping sound
      loopSoundBuffer(this.loopSoundBuffer, '../pd/assets/btnSnd.wav');
      this.loopingBufferSource = this.context.createBufferSource();
      this.loopingBufferSource.loop = true;
      this.loopingBufferSource.loop.connect(this.context.destination);  
      
      isInitialized = true;
    }
    catch(e) {
      alert('Web Audio API is not supported in this browser');
    }
  },
  
  // Interactive audio functions
  buttonClicked: function() {
    var source = context.createBufferSource(); // creates a sound source
    source.buffer = this.btnSoundBuffer;       // tell the source which sound to play
    source.connect(this.context.destination);       // connect the source to the context's destination (the speakers)
    source.start(0);                           // play the source now                      
  },
  
  toggleLoopSound : function() {
    if (this.musicLooping) {
      this.loopingBufferSource.stop(0);
    } else {
      this.loopingBufferSource.start(0);
    }
    this.musicLooping = !this.musicLooping;
  },
  
  playSynthNote : function() {
    //window.plugins.libPd.sendFloat(Math.floor(Math.random() * 127) + 1, 'playNote');
  }
  
};

