#pragma once

#include <JuceHeader.h>

class ParametricEQ : public juce::AudioProcessor
{
public:
    struct EQBand
    {
        enum Type { PEAK, HPF, LPF };
        
        Type type = PEAK;
        float frequency = 1000.0f;
        float gain = 0.0f;
        float q = 1.0f;
        int slope = 12; // For HPF/LPF: 6, 12, 24, 48 dB/oct
        
        bool isBypassed = false;
        juce::Point<float> nodePosition; // For visual editor
        
        bool operator==(const EQBand& other) const
        {
            return type == other.type &&
                   frequency == other.frequency &&
                   gain == other.gain &&
                   q == other.q &&
                   slope == other.slope;
        }
    };
    
    ParametricEQ();
    ~ParametricEQ() override;
    
    // AudioProcessor overrides
    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;
    void processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages) override;
    
    const juce::String getName() const override { return "ParametricEQ"; }
    bool acceptsMidi() const override { return false; }
    bool producesMidi() const override { return false; }
    double getTailLengthSeconds() const override { return 0.0; }
    
    int getNumPrograms() override { return 1; }
    int getCurrentProgram() override { return 0; }
    void setCurrentProgram(int index) override {}
    const juce::String getProgramName(int index) override { return {}; }
    void changeProgramName(int index, const juce::String& newName) override {}
    
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override { return true; }
    
    void getStateInformation(juce::MemoryBlock& destData) override;
    void setStateInformation(const void* data, int sizeInBytes) override;
    
    // EQ-specific methods
    void addBand(const EQBand& band);
    void removeBand(int index);
    void updateBand(int index, const EQBand& band);
    const juce::Array<EQBand>& getBands() const { return bands; }
    void clearBands() { bands.clear(); updateFilters(); }
    
    void loadPreset(const juce::String& presetName);
    void savePreset(const juce::String& presetName);
    juce::StringArray getPresetList() const;
    
private:
    void updateFilters();
    
    juce::Array<EQBand> bands;
    juce::OwnedArray<juce::IIRFilter> filters;
    double currentSampleRate = 44100.0;
    
    juce::CriticalSection lock;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ParametricEQ)
};