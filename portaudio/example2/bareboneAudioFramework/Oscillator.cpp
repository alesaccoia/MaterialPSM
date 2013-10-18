//
//  Oscillator.cpp
//  portaudioExample2
//
//  Created by Alessandro Saccoia on 10/16/13.
//  Copyright (c) 2013 keplero.com. All rights reserved.
//

#include "Oscillator.hpp"
#include "Config.hpp"
#include <cmath>

const float Oscillator::PI_2 = 2.0F * M_PI;

Oscillator::Oscillator(float freq_) :
  m_frequency(freq_),
  m_phase(.0F) {

}

void Oscillator::Process(float* buf_, int count_) {
  float* output = (float*)buf_;
  const float phaseIncrement = PI_2 * m_frequency / SAMPLE_RATE;
  
  for (int i = 0; i < count_; ++i) {
    output[i] = 0.5 * sin(m_phase);
    m_phase += phaseIncrement;
    if (m_phase >= PI_2) {
      m_phase -= PI_2;
    }
  }
  
}

