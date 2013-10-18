
#ifndef portaudioExample2_bareboneAudioNode_hpp
#define portaudioExample2_bareboneAudioNode_hpp

class BareboneAudioNode {
public:
  virtual ~BareboneAudioNode() {};
  virtual void Process(float* buf, int count) = 0;
};

#endif
