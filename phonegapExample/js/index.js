
var phonegapApp = {
  initialize: function() {
    this.bindEvents();
  },
  
  // Bind any events that are required on startup. Common events are:
  // 'load', 'deviceready', 'offline', and 'online'.
  bindEvents: function() {
    document.addEventListener('deviceready', this.onDeviceReady, false);
  },
  // deviceready Event Handler
  onDeviceReady: function() {
    app.initialize();
    $.mobile.loading( "hide" );
  }
};


// optimization. this shit is slow!
// http://stackoverflow.com/questions/14097351/phonegap-jquery-mobile-slow-tap-response-time

$.mobile.defaultPageTransition   = 'none'
$.mobile.defaultDialogTransition = 'none'
$.mobile.buttonMarkup.hoverDelay = 0


// taken directly from http://api.jquerymobile.com/loader/

$(document).on( "ready", function() {
  var $this = $( this ),
  theme = $this.jqmData( "theme" ) || $.mobile.loader.prototype.options.theme,
  msgText = $this.jqmData( "msgtext" ) || $.mobile.loader.prototype.options.text,
  textVisible = $this.jqmData( "textvisible" ) || $.mobile.loader.prototype.options.textVisible,
  textonly = !!$this.jqmData( "textonly" );
  html = $this.jqmData( "html" ) || "";
$.mobile.loading( 'show', {
  text: msgText,
  textVisible: textVisible,
  theme: theme,
  textonly: textonly,
  html: html
  });
})


$('#functionList').on('vclick', 'li', function() {
  if (this.id == 'activateLoop') {
    app.toggleLoopSound();
    if (app.musicLooping) {
      $(this).html('Stop looping sound');
    } else {
      $(this).html('Activate looping sound');
    }
  } else if (this.id == 'playSynth') {
     app.playSynthNote();
  } else if (this.id == 'buttonSnd') {
     app.buttonClicked();
  }
  event.preventDefault();
  return false;
});


