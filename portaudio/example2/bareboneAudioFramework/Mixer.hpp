//
//  Mixer.h
//  portaudioExample2
//
//  Created by Alessandro Saccoia on 10/16/13.
//  Copyright (c) 2013 keplero.com. All rights reserved.
//

#ifndef __portaudioExample2__Mixer__
#define __portaudioExample2__Mixer__

#include "BareboneAudioNode.hpp"
#include "Config.hpp"
#include <vector>

class Mixer : public BareboneAudioNode {
public:
  void addSource(BareboneAudioNode* src);
  void Process(float* buf, int count);
protected:
  std::vector<BareboneAudioNode*> m_sources;
  float tempBuffer[BUFFERSIZE];
};

#endif /* defined(__portaudioExample2__Mixer__) */
