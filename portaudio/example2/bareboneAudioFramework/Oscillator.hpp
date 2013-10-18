
#ifndef __portaudioExample2__Oscillator__
#define __portaudioExample2__Oscillator__

#include "BareboneAudioNode.hpp" 



class Oscillator:
  public BareboneAudioNode {
public:
  Oscillator(float startFrequency);
  void Process(float* buf, int count);
private:
  float m_frequency;
  float m_phase;
  static const float PI_2;
};

#endif /* defined(__portaudioExample2__Oscillator__) */
