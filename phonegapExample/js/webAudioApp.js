
// Utilities

var context = null;

var SubtractiveSynth = function() {
  this.vco = context.createOscillator();
  this.vco.type = 2;
  
  this.vcf = context.createBiquadFilter();
  this.vcf.Q = 3;
  this.vca = context.createGain();
  this.vca.gain.value = 0;
  
  this.vco.connect(this.vcf);
  this.vcf.connect(this.vca);
  this.vca.connect(context.destination);
  
  this.vco.start(0);
};

SubtractiveSynth.prototype.play = function(vel) {
  var currentTime = context.currentTime;
  // VCO Envelope
  var peakLevel = 1;
  var attackTime = 30 / 1000;
  var decayTime = 200 / 1000;
  var sustainPercent = 0 / 100;
  var releaseTime = 500 / 1000;
  
  this.vca.gain.linearRampToValueAtTime(peakLevel, currentTime + attackTime);
  this.vca.gain.linearRampToValueAtTime(peakLevel * sustainPercent, currentTime + attackTime + decayTime);
  this.vca.gain.linearRampToValueAtTime(0, currentTime + attackTime + decayTime + releaseTime);
  
  // VCF Envelope
  peakLevel = 1.2 * 4000 * vel / 127;
  attackTime = 30 / 1000;
  decayTime = 200 / 1000;
  sustainPercent = 50 / 100;
  releaseTime = 500 / 1000;
  this.vcf.frequency.linearRampToValueAtTime(peakLevel, currentTime + attackTime);
  this.vcf.frequency.linearRampToValueAtTime(peakLevel * sustainPercent, currentTime + attackTime + decayTime);
  this.vcf.frequency.linearRampToValueAtTime(0, currentTime + attackTime + decayTime + releaseTime);
};


function loadSample(destinationBuffer, url) {
  var request = new XMLHttpRequest();
  request.open("GET", url, false);
  request.responseType = "arraybuffer";

  request.onload = function() { 
    destinationBuffer.buf = context.createBuffer(request.response, false);
  }
  request.onerror = function() {
    alert('failed to fetch the file');
  }
  request.send();
}

// web audio application
var app = {

  musicLooping: false,
  isInitialized: false,
  context: null,
  
  btnSoundBuffer: { buf: null },
  loopSoundBuffer: { buf: null },
  loopingBufferSource: null,
  
  synth: null,
  
  initialize: function() {
    try {
      window.AudioContext = window.AudioContext || window.webkitAudioContext;
      context = new AudioContext();
      
      loadSample(this.btnSoundBuffer, './pd/assets/btnSnd.wav');      
      loadSample(this.loopSoundBuffer, './pd/assets/loop.wav');      
      this.synth = new SubtractiveSynth();
      
      isInitialized = true;
    }
    catch(e) {
      alert('Web Audio API is not supported in this browser');
    }
  },
  
  // Interactive audio functions
  buttonClicked: function() {
    if (!isInitialized)
      return;
    var source = context.createBufferSource(); // creates a sound source
    source.buffer = this.btnSoundBuffer.buf;       // tell the source which sound to play
    var gainNode = context.createGain();
    gainNode.gain = 0.5;
    source.connect(gainNode);
    gainNode.connect(context.destination);
    source.start(0);                           // play the source now
  },
  
  toggleLoopSound : function() {
    if (!isInitialized)
      return;
    if (this.musicLooping) {
      this.loopingBufferSource.stop(0);
    } else {
      this.loopingBufferSource = context.createBufferSource();
      this.loopingBufferSource.buffer = this.loopSoundBuffer.buf;
      var gainNode = context.createGain();
      gainNode.gain = 0.9;
      this.loopingBufferSource.connect(gainNode);
      gainNode.connect(context.destination);
      this.loopingBufferSource.start(0);
    }
    this.musicLooping = !this.musicLooping;
  },
  
  playSynthNote : function() {
    if (!isInitialized)
      return;
    this.synth.play(Math.floor(Math.random() * 70) + 20);
  }
  
};

