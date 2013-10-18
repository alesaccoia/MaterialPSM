//
//  Mixer.cpp
//  portaudioExample2
//
//  Created by Alessandro Saccoia on 10/16/13.
//  Copyright (c) 2013 keplero.com. All rights reserved.
//

#include "Mixer.hpp"

#include "BareboneAudioNode.hpp"
#include <functional>


void Mixer::addSource(BareboneAudioNode* src) {
  m_sources.push_back(src);
}


void Mixer::Process(float* buf, int count) {
  std::fill(buf, buf + BUFFERSIZE, 0);
  for (int i = 0; i < m_sources.size(); ++i) {
    m_sources[i]->Process(tempBuffer, BUFFERSIZE);
    std::transform(buf, buf + BUFFERSIZE, tempBuffer, buf, std::plus<float>());
  }
  
}
