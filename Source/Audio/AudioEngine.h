#pragma once
#include <JuceHeader.h>

class AudioEngine : public juce::ChangeListener,
                    public juce::AudioIODeviceCallback
{
public:
    AudioEngine();
    ~AudioEngine() override;

    juce::Array<juce::AudioIODeviceType*> getAvailableDeviceTypes();
    juce::StringArray getInputDeviceNames();
    juce::StringArray getOutputDeviceNames();
    
    void setInputDevice(const juce::String& deviceName);
    void setOutputDevice(const juce::String& deviceName);
    
    void startAudio();
    void stopAudio();
    
    void addRoute(int sourceChannel, int destChannel, float gain = 1.0f);
    void removeRoute(int sourceChannel, int destChannel);
    void clearAllRoutes();
    
    // juce::AudioIODeviceCallback
    void audioDeviceIOCallback(const float** inputChannelData,
                               int numInputChannels,
                               float** outputChannelData,
                               int numOutputChannels,
                               int numSamples) override;
    void audioDeviceAboutToStart(juce::AudioIODevice* device) override;
    void audioDeviceStopped() override;
    void audioDeviceError(const juce::String& errorMessage) override;
    
    // juce::ChangeListener
    void changeListenerCallback(juce::ChangeBroadcaster* source) override;
    
private:
    juce::AudioDeviceManager deviceManager;
    juce::dsp::ProcessSpec processSpec;
    
    struct Route
    {
        int sourceChannel;
        int destChannel;
        float gain;
        bool mute;
    };
    
    juce::Array<Route> activeRoutes;
    juce::CriticalSection routeLock;
    
    void updateAudioSetup();
};
