#include "AudioController.h"

#include <stdexcept>

AudioController::AudioController() : m_error(paNoError), m_stream(nullptr), m_streamParams(), m_recordedSamples(nullptr), m_totalFrames(0), m_frameIndex(0)
{
    this->m_error = Pa_Initialize();
    if (this->m_error != paNoError) {
        throw std::runtime_error("Failed to initialize PortAudio");
    }
}

AudioController::~AudioController()
{
    if (m_recordedSamples) {
        delete[] m_recordedSamples;
    }
    Pa_Terminate();
}

void AudioController::SetupInputDevice()
{
    this->m_streamParams.device = Pa_GetDefaultInputDevice();
    if (this->m_streamParams.device == paNoDevice) {
        throw std::runtime_error("Failed to get default input device");
    }

    this->m_streamParams.channelCount = NUM_CHANNELS;
    this->m_streamParams.sampleFormat = PA_SAMPLE_TYPE;
    this->m_streamParams.suggestedLatency = Pa_GetDeviceInfo(this->m_streamParams.device)->defaultLowInputLatency;
    this->m_streamParams.hostApiSpecificStreamInfo = nullptr;
}

void AudioController::SetupOutputDevice()
{
    this->m_streamParams.device = Pa_GetDefaultOutputDevice();
    if (this->m_streamParams.device == paNoDevice) {
        throw std::runtime_error("Failed to get default output device");
    }

    this->m_streamParams.channelCount = NUM_CHANNELS;
    this->m_streamParams.sampleFormat = PA_SAMPLE_TYPE;
    this->m_streamParams.suggestedLatency = Pa_GetDeviceInfo(this->m_streamParams.device)->defaultLowOutputLatency;
    this->m_streamParams.hostApiSpecificStreamInfo = nullptr;
}

void AudioController::StartRecording(unsigned long durationInSeconds)
{
    m_totalFrames = durationInSeconds * SAMPLE_RATE;
    m_recordedSamples = new SAMPLE[m_totalFrames * NUM_CHANNELS];
    std::memset(m_recordedSamples, 0, m_totalFrames * NUM_CHANNELS * sizeof(SAMPLE));

    this->m_error = Pa_OpenStream(&this->m_stream, &this->m_streamParams, nullptr, SAMPLE_RATE, FRAMES_PER_BUFFER, paClipOff, RecordCallback, this);
    if (this->m_error != paNoError) {
        throw std::runtime_error("Failed to open audio stream");
    }

    this->m_error = Pa_StartStream(this->m_stream);
    if (this->m_error != paNoError) {
        throw std::runtime_error("Failed to start audio stream");
    }
}

void AudioController::StopStream()
{
    if (m_stream) {
        this->m_error = Pa_StopStream(this->m_stream);
        if (this->m_error != paNoError) {
            throw std::runtime_error("Failed to stop audio stream");
        }
    }
}

void AudioController::CloseStream()
{
    if (m_stream) {
        this->m_error = Pa_CloseStream(this->m_stream);
        if (this->m_error != paNoError) {
            throw std::runtime_error("Failed to close audio stream");
        }
        m_stream = nullptr;
    }
}

void AudioController::PlayRecording()
{
    if (m_recordedSamples == nullptr) {
        throw std::runtime_error("No recorded samples available for playback");
    }

    this->m_frameIndex = 0; 

    this->m_error = Pa_OpenStream(&this->m_stream, nullptr, &this->m_streamParams, SAMPLE_RATE, FRAMES_PER_BUFFER, paClipOff, PlayCallback, this);
    if (this->m_error != paNoError) {
        throw std::runtime_error("Failed to open audio stream for playback");
    }

    this->m_error = Pa_StartStream(this->m_stream);
    if (this->m_error != paNoError) {
        throw std::runtime_error("Failed to start audio playback stream");
    }

    while (Pa_IsStreamActive(this->m_stream) == 1) {
        Pa_Sleep(100);
    }

    StopStream();
    CloseStream();
}

void AudioController::Release()
{
    this->StopStream();
    this->CloseStream();
}

int AudioController::RecordCallback(const void* inputBuffer, void* outputBuffer, unsigned long framesPerBuffer, const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags, void* userData)
{
    AudioController* controller = static_cast<AudioController*>(userData);
    SAMPLE* recordedSamples = controller->m_recordedSamples + controller->m_frameIndex * NUM_CHANNELS;
    const SAMPLE* rptr = static_cast<const SAMPLE*>(inputBuffer);

    if (inputBuffer == nullptr) {
        for (unsigned int i = 0; i < framesPerBuffer; i++) {
            *recordedSamples++ = 0;
        }
    }
    else {
        for (unsigned int i = 0; i < framesPerBuffer; i++) {
            *recordedSamples++ = *rptr++;
        }
    }

    controller->m_frameIndex += framesPerBuffer;

    if (controller->m_frameIndex >= controller->m_totalFrames) {
        return paComplete; 
    }

    return paContinue;
}

int AudioController::PlayCallback(const void* inputBuffer, void* outputBuffer, unsigned long framesPerBuffer, const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags, void* userData)
{
    AudioController* controller = static_cast<AudioController*>(userData);
    SAMPLE* out = static_cast<SAMPLE*>(outputBuffer);
    unsigned long framesLeft = controller->m_totalFrames - controller->m_frameIndex;
    unsigned long framesToPlay = (framesLeft < framesPerBuffer) ? framesLeft : framesPerBuffer;

    for (unsigned long i = 0; i < framesToPlay; i++) {
        *out++ = controller->m_recordedSamples[controller->m_frameIndex++];
    }

    if (framesLeft < framesPerBuffer) {
        for (unsigned long i = framesLeft; i < framesPerBuffer; i++) {
            *out++ = 0; 
        }
        return paComplete;
    }

    return paContinue;
}
