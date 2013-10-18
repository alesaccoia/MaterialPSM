//
//  main.cpp
//  portaudioExample
//
//  Created by Alessandro Saccoia on 10/16/13.
//  Copyright (c) 2013 keplero.com. All rights reserved.
//

#include <iostream>
#include <cmath>
#include <thread>
#include "portaudio.h"

#include "Config.hpp"
#include "Mixer.hpp"
#include "Oscillator.hpp"

using namespace std;
volatile bool g_stop = false;
float destinationBuffer[BUFFERSIZE];

static int paCallback(const void *inputBuffer
	, void *outputBuffer
	, unsigned long framesPerBuffer
	, const PaStreamCallbackTimeInfo* timeInfo
	, PaStreamCallbackFlags statusFlags
	, void *userData ) {
  
	if (!outputBuffer)
		return 0;
  
  
  Mixer* finalNode = (Mixer*)userData;
  finalNode->Process((float*)outputBuffer, BUFFERSIZE);
  
	if (g_stop)
		return paComplete;
  return paContinue;
}


int main(int argc, const char * argv[]) {
  cout << "Portaudio example" << endl;
  
  Oscillator* osc220 = new Oscillator(220);
  Oscillator* osc880 = new Oscillator(880);
  Mixer* finalNode = new Mixer();
  
  finalNode->addSource(osc220);
  finalNode->addSource(osc880);
  
  //configure and starts the portaudio
	PaStream *v_stream;
  Pa_Initialize();
  Pa_OpenDefaultStream(
      &v_stream,
      0, 
      1, /* mono output */
      paFloat32,  
      44100.0,
      BUFFERSIZE,
      paCallback,
      (void*)finalNode);
  Pa_StartStream( v_stream );
  
  std::string line;
  std::getline(std::cin, line);
  g_stop = true;

  while(Pa_IsStreamActive(v_stream))
    ;
}


