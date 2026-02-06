#include "AudioEngine.h"

AudioEngine::AudioEngine()
{
    deviceManager.addChangeListener(this);
    auto* deviceTypes = deviceManager.getAvailableDeviceTypes();
    
    // Prioritize CoreAudio on macOS
    for (auto* type : *deviceTypes)
    {
        if (type->getTypeName() == "CoreAudio")
        {
            deviceManager.setCurrentAudioDeviceType("CoreAudio", true);
            break;
        }
    }
}

AudioEngine::~AudioEngine()
{
    deviceManager.removeChangeListener(this);
    deviceManager.removeAudioCallback(this);
    deviceManager.closeAudioDevice();
}

juce::Array<juce::AudioIODeviceType*> AudioEngine::getAvailableDeviceTypes()
{
    return *deviceManager.getAvailableDeviceTypes();
}

juce::StringArray AudioEngine::getInputDeviceNames()
{
    if (auto* device = deviceManager.getCurrentAudioDevice())
        return device->getInputChannelNames();
    return {};
}

juce::StringArray AudioEngine::getOutputDeviceNames()
{
    if (auto* device = deviceManager.getCurrentAudioDevice())
        return device->getOutputChannelNames();
    return {};
}

void AudioEngine::setInputDevice(const juce::String& deviceName)
{
    auto setup = deviceManager.getAudioDeviceSetup();
    setup.inputDeviceName = deviceName;
    deviceManager.setAudioDeviceSetup(setup, true);
}

void AudioEngine::setOutputDevice(const juce::String& deviceName)
{
    auto setup = deviceManager.getAudioDeviceSetup();
    setup.outputDeviceName = deviceName;
    deviceManager.setAudioDeviceSetup(setup, true);
}

void AudioEngine::startAudio()
{
    deviceManager.addAudioCallback(this);
}

void AudioEngine::stopAudio()
{
    deviceManager.removeAudioCallback(this);
}

void AudioEngine::addRoute(int sourceChannel, int destChannel, float gain)
{
    juce::ScopedLock lock(routeLock);
    activeRoutes.add({sourceChannel, destChannel, gain, false});
}

void AudioEngine::removeRoute(int sourceChannel, int destChannel)
{
    juce::ScopedLock lock(routeLock);
    for (int i = activeRoutes.size() - 1; i >= 0; --i)
    {
        if (activeRoutes[i].sourceChannel == sourceChannel &&
            activeRoutes[i].destChannel == destChannel)
        {
            activeRoutes.remove(i);
        }
    }
}

void AudioEngine::clearAllRoutes()
{
    juce::ScopedLock lock(routeLock);
    activeRoutes.clear();
}

void AudioEngine::audioDeviceIOCallback(const float** inputChannelData,
                                        int numInputChannels,
                                        float** outputChannelData,
                                        int numOutputChannels,
                                        int numSamples)
{
    // Clear output buffers
    for (int channel = 0; channel < numOutputChannels; ++channel)
    {
        if (outputChannelData[channel] != nullptr)
            juce::FloatVectorOperations::clear(outputChannelData[channel], numSamples);
    }
    
    juce::ScopedLock lock(routeLock);
    
    // Apply routing
    for (const auto& route : activeRoutes)
    {
        if (route.mute) continue;
        
        int srcCh = route.sourceChannel;
        int dstCh = route.destChannel;
        
        if (srcCh < numInputChannels && dstCh < numOutputChannels &&
            inputChannelData[srcCh] != nullptr &&
            outputChannelData[dstCh] != nullptr)
        {
            juce::FloatVectorOperations::addWithMultiply(
                outputChannelData[dstCh],
                inputChannelData[srcCh],
                route.gain,
                numSamples
            );
        }
    }
}

void AudioEngine::audioDeviceAboutToStart(juce::AudioIODevice* device)
{
    processSpec.sampleRate = device->getCurrentSampleRate();
    processSpec.maximumBlockSize = device->getCurrentBufferSizeSamples();
    processSpec.numChannels = device->getActiveOutputChannels().countNumberOfSetBits();
}

void AudioEngine::audioDeviceStopped()
{
    processSpec.sampleRate = 0;
    processSpec.maximumBlockSize = 0;
    processSpec.numChannels = 0;
}

void AudioEngine::audioDeviceError(const juce::String& errorMessage)
{
    DBG("Audio device error: " + errorMessage);
}

void AudioEngine::changeListenerCallback(juce::ChangeBroadcaster* source)
{
    if (source == &deviceManager)
    {
        updateAudioSetup();
    }
}

void AudioEngine::updateAudioSetup()
{
    // Handle device changes if needed
}
