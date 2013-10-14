
var app = {
  musicLooping: false,

  initialize: function() {
    window.plugins.libPd.init(
      app.pdInited,
      function () { alert('Pure Data initialization failed!') }
    );
  },

  pdInited: function() {
    console.log('pdLib initialized OK');
    window.plugins.libPd.addPath('/www/pd/pd');
    window.plugins.libPd.openPatch(
      'pd/pd/example.pd',
      app.patchLoaded,
      function () { alert('PD patch failed loading!'); }
    );
  },
  
  patchLoaded: function() {
    console.log('patch loaded OK');
  },
  
  // Interactive audio functions
  buttonClicked: function() {
    window.plugins.libPd.sendBang('buttonClicked');
  },
  
  toggleLoopSound : function() {
    window.plugins.libPd.sendBang('musicOn');
    this.musicLooping = !this.musicLooping;
  },
  
  playSynthNote : function() {
    window.plugins.libPd.sendFloat(Math.floor(Math.random() * 127) + 1, 'playNote');
  }
};
