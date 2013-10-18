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

#define SAMPLE_RATE 44100.0F;
#define PA_BUFFERSIZE 512

using namespace std;
volatile bool g_stop = false;

float frequency = 220.F;
float phase = 0.F;
const float PI_2 = 2.0F * M_PI;


int sleepMilliseconds = 1;
std::chrono::milliseconds sleepDuration(sleepMilliseconds);

static int paCallback(const void *inputBuffer
	, void *outputBuffer
	, unsigned long framesPerBuffer
	, const PaStreamCallbackTimeInfo* timeInfo
	, PaStreamCallbackFlags statusFlags
	, void *userData ) {
  
	if (!outputBuffer)
		return 0;
	
  float* output = (float*)outputBuffer;
  const float phaseIncrement = PI_2 * frequency / SAMPLE_RATE;
  
  for (int i = 0; i < framesPerBuffer; ++i) {
    output[i] = sin(phase);
    phase += phaseIncrement;
    if (phase >= PI_2) {
      phase -= PI_2;
    }
  }
  
  std::this_thread::sleep_for(sleepDuration);
  
	if (g_stop)
		return paComplete;
  return paContinue;
}


int main(int argc, const char * argv[]) {
  cout << "Portaudio example" << endl;
  
  //configure and starts the portaudio
	PaStream *v_stream;
	
  Pa_Initialize();

  Pa_OpenDefaultStream(
      &v_stream,
      0, 
      1, /* mono output */
      paFloat32,  
      44100.0,
      PA_BUFFERSIZE,
      paCallback,
      NULL);
  Pa_StartStream( v_stream );
  std::string line;
  while (std::getline(std::cin, line)) {
    if (line == "w") {
      sleepMilliseconds += 1;
      sleepDuration = std::chrono::milliseconds(sleepMilliseconds);
      std::cout << "Increased duration to" << sleepMilliseconds << std::endl;
    } else if (line == "s") {
      sleepMilliseconds = (sleepMilliseconds > 0) ? sleepMilliseconds - 1 : sleepMilliseconds;
      sleepDuration = std::chrono::milliseconds(sleepMilliseconds);
      std::cout << "Decreased duration to" << sleepMilliseconds << std::endl;
    } else {
      break;
    }
  }

  g_stop = true;

  while(Pa_IsStreamActive(v_stream))
    ;
}


void getCommands() {


}

