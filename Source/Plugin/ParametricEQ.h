#pragma once
#include <JuceHeader.h>

class ParametricEQ
{
public:
    ParametricEQ();
    ~ParametricEQ() = default;

    void prepare(const juce::dsp::ProcessSpec& spec);
    void process(juce::AudioBuffer<float>& buffer);
    void reset();

    // Band management
    void setBandParameters(size_t index, float freq, float gain, float q);
    std::array<std::atomic<float>, 16>& getGains() { return bandGains; }
    std::array<std::atomic<float>, 16>& getFreqs() { return bandFreqs; }
    std::array<std::atomic<float>, 16>& getQs() { return bandQs; }

    // Preset management
    void savePreset(const juce::String& name);
    void loadPreset(const juce::String& name);
    juce::StringArray getPresetList();

private:
    std::array<juce::dsp::IIR::Filter<float>, 16> filters;
    std::array<std::atomic<float>, 16> bandGains;
    std::array<std::atomic<float>, 16> bandFreqs;
    std::array<std::atomic<float>, 16> bandQs;
    juce::dsp::ProcessSpec currentSpec;
    juce::File presetDir;

    void updateBand(size_t index);
    juce::File getPresetFile(const juce::String& name);
};
