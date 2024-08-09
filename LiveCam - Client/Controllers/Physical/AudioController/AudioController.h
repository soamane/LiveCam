#ifndef AUDIOCONTROLLER_H
#define AUDIOCONTROLLER_H

#include <portaudio.h>

#define SAMPLE_RATE  (44100)
#define FRAMES_PER_BUFFER (512)
#define NUM_CHANNELS    (1)
#define PA_SAMPLE_TYPE  paInt16

typedef short SAMPLE;

class AudioController
{
public:
    AudioController();
    ~AudioController();

    void SetupInputDevice();  
    void SetupOutputDevice();

    void StartRecording(unsigned long durationInSeconds); 

    void PlayRecording(); 
    void Release();
private:
    PaError m_error;  
    PaStream* m_stream; 
    PaStreamParameters m_streamParams; 

    SAMPLE* m_recordedSamples; 
    unsigned long m_totalFrames;
    unsigned long m_frameIndex; 

    void StopStream();
    void CloseStream();

    static int RecordCallback(const void* inputBuffer, void* outputBuffer,
        unsigned long framesPerBuffer,
        const PaStreamCallbackTimeInfo* timeInfo,
        PaStreamCallbackFlags statusFlags,
        void* userData);

    static int PlayCallback(const void* inputBuffer, void* outputBuffer,
        unsigned long framesPerBuffer,
        const PaStreamCallbackTimeInfo* timeInfo,
        PaStreamCallbackFlags statusFlags,
        void* userData);
};

#endif // AUDIOCONTROLLER_H
