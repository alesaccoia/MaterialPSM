#include <CoreFoundation/CoreFoundation.h>
#include <AudioToolbox/AudioToolbox.h>
#include "Debugging++.h"
#include <unistd.h>

AUGraph		mGraph;

AudioUnit	m_filePlayerAu;
AudioUnit	m_mixerAu;
AudioUnit	m_reverbAu;
AudioUnit m_outputAu;

AUNode m_filePlayerNode;
AUNode m_mixerNode;
AUNode m_reverbNode;
AUNode m_outputNode;

int m_fileDuration;
  
void openFile(const char* path) {

  AudioStreamBasicDescription inputFormat;
  AudioFileID inputFile;
  
  CFStringRef pathRef = CFStringCreateWithCString(kCFAllocatorDefault, path, kCFStringEncodingMacRoman);
  
  CFURLRef inputFileURL = CFURLCreateWithFileSystemPath(
                            kCFAllocatorDefault,
                            pathRef,
                            kCFURLPOSIXPathStyle,
                            false);
  RequireNoErrString(AudioFileOpenURL(inputFileURL,
                            kAudioFileReadPermission,
                            0,
                            &inputFile),
                            "AudioFileOpenURL failed");
  CFRelease(inputFileURL);
  // Get the audio data format from the file
  UInt32 propSize = sizeof(inputFormat);
  RequireNoErrString(AudioFileGetProperty(inputFile,
                                kAudioFilePropertyDataFormat,
                                &propSize,
                                &inputFormat),
           "Couldn't get file's data format");
  
  // now set the output format in the mixer unit
  // Tell the file player unit to load the file we want to play
  RequireNoErrString(AudioUnitSetProperty(m_filePlayerAu,
                                  kAudioUnitProperty_ScheduledFileIDs,
                                  kAudioUnitScope_Global,
                                  0,
                                  &inputFile,
                                  sizeof(inputFile)),
         "AudioUnitSetProperty[kAudioUnitProperty_ScheduledFileIDs] failed");
         
  UInt64 nPackets;
  UInt32 propsize = sizeof(nPackets);
  RequireNoErrString(AudioFileGetProperty(inputFile,
                                  kAudioFilePropertyAudioDataPacketCount,
                                  &propsize,
                                  &nPackets),
        "AudioFileGetProperty[kAudioFilePropertyAudioDataPacketCount] failed");

  // Tell the file player AU to play the entire file
  ScheduledAudioFileRegion rgn;
  memset (&rgn.mTimeStamp, 0, sizeof(rgn.mTimeStamp));
  rgn.mTimeStamp.mFlags = kAudioTimeStampSampleTimeValid;
  rgn.mTimeStamp.mSampleTime = 0;
  rgn.mCompletionProc = NULL;
  rgn.mCompletionProcUserData = NULL;
  rgn.mAudioFile = inputFile;
  rgn.mLoopCount = INT_MAX;
  rgn.mStartFrame = 0;
  rgn.mFramesToPlay = nPackets * inputFormat.mFramesPerPacket;

  RequireNoErrString(AudioUnitSetProperty(m_filePlayerAu,
                                  kAudioUnitProperty_ScheduledFileRegion,
                                  kAudioUnitScope_Global,
                                  0,
                                  &rgn,
                                  sizeof(rgn)), "Impossible to set scheduled file region");

	// prime the file player AU with default values
	UInt32 defaultVal = 0;
	RequireNoErrString(AudioUnitSetProperty(m_filePlayerAu, kAudioUnitProperty_ScheduledFilePrime, 
									kAudioUnitScope_Global, 0, &defaultVal, sizeof(defaultVal)),
			   "AudioUnitSetProperty[kAudioUnitProperty_ScheduledFilePrime] failed");
	
	// tell the file player AU when to start playing (-1 sample time means next render cycle)
	AudioTimeStamp startTime;
	memset (&startTime, 0, sizeof(startTime));
	startTime.mFlags = kAudioTimeStampSampleTimeValid;
	startTime.mSampleTime = -1;
	RequireNoErrString(AudioUnitSetProperty(m_filePlayerAu, kAudioUnitProperty_ScheduleStartTimeStamp, 
									kAudioUnitScope_Global, 0, &startTime, sizeof(startTime)),
			   "AudioUnitSetProperty[kAudioUnitProperty_ScheduleStartTimeStamp]");
         

  m_fileDuration = (nPackets * inputFormat.mFramesPerPacket) / inputFormat.mSampleRate;
}


// 1st arg is the path


int main (int argc, const char * argv[]) {  
  AudioComponentDescription output_desc = {0};
  output_desc.componentType = kAudioUnitType_Output;
  output_desc.componentSubType = kAudioUnitSubType_DefaultOutput;
  output_desc.componentManufacturer = kAudioUnitManufacturer_Apple;  
  output_desc.componentFlags = 0;
  output_desc.componentFlagsMask = 0;
  
  AudioComponentDescription mixer_desc  = {0};
  mixer_desc.componentType = kAudioUnitType_Mixer;
  mixer_desc.componentSubType = kAudioUnitSubType_StereoMixer;
  mixer_desc.componentManufacturer = kAudioUnitManufacturer_Apple;
  mixer_desc.componentFlags = 0;

  AudioComponentDescription fileplayer_desc = {0};
  fileplayer_desc.componentType = kAudioUnitType_Generator;
  fileplayer_desc.componentSubType = kAudioUnitSubType_AudioFilePlayer;
  fileplayer_desc.componentManufacturer = kAudioUnitManufacturer_Apple;

  // new AU graph
  RequireNoErr(NewAUGraph(&mGraph));  
    // Add the nodes to the graph,
  // The AudioComponentDescription object used to find and open the audio unit 
  // that you are adding as a new node.
	RequireNoErr(AUGraphAddNode(mGraph, &output_desc, &m_outputNode));
	RequireNoErr(AUGraphAddNode(mGraph, &mixer_desc, &m_mixerNode));
  RequireNoErr(AUGraphAddNode(mGraph, &fileplayer_desc, &m_filePlayerNode)); 

//

  
	RequireNoErr(AUGraphConnectNodeInput(	mGraph,
											m_filePlayerNode, // AUNode inSourceNode,
                      0, //UInt32 inSourceOutputNumber,
											m_mixerNode, // inDestNode
                      0 )); // inDestInputNumber

	RequireNoErr(AUGraphConnectNodeInput(	mGraph,
											m_mixerNode, // AUNode inSourceNode,
                      0, //UInt32 inSourceOutputNumber,
											m_outputNode, // inDestNode
                      0 )); // inDestInputNumber

  //Upon return from this function call, the audio units belonging to the graph 
  // are open but not initialized. Specifically, no resource allocation occurs.
	RequireNoErr(AUGraphOpen(mGraph));
  
  // Calling this function calls the AudioUnitInitialize function on each opened node 
  // or audio unit that is involved in a interaction. If a node is not involved, it 
  // is initialized after it becomes involved in an interaction.
  RequireNoErr(AUGraphInitialize(mGraph));
  
  
  // THE AU HAS TO BE INSTANTIATED BEFORE WE CAN FILL THE AU STRUCTURE

	RequireNoErr(AUGraphNodeInfo (
								 mGraph,
								 m_filePlayerNode,
								 NULL,
								 &m_filePlayerAu
								 ));

  openFile(argv[1]);

  RequireNoErrString(AUGraphStart(mGraph),
           "AUGraphStart failed");

  // Sleep until the file is finished
  usleep ((int)(m_fileDuration * 1000.0 * 1000.0));
  
  // insert code here...
  CFShow(CFSTR("Hello, World!\n"));
  return 0;
}
