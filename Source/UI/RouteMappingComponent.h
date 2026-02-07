#pragma once

#include <JuceHeader.h>
#include "ParametricEQ.h"
#include "EQEditor16Band.h"

class RouteMappingComponent : public juce::Component
{
public:
    RouteMappingComponent(const juce::String& mappingName);
    ~RouteMappingComponent() override;
    
    void paint(juce::Graphics& g) override;
    void resized() override;
    
    void setInputDevice(const juce::String& deviceName);
    void setOutputDevice(const juce::String& deviceName);
    
    juce::String getMappingName() const { return mappingName; }
    bool isMuted() const { return isMute; }
    float getVolume() const { return volume; }
    
private:
    void showEQEditor();
    
    juce::String mappingName;
    
    std::unique_ptr<juce::ComboBox> inputDeviceCombo;
    std::unique_ptr<juce::ComboBox> outputDeviceCombo;
    std::unique_ptr<juce::ToggleButton> muteButton;
    std::unique_ptr<juce::TextButton> eqButton;
    std::unique_ptr<juce::Slider> volumeSlider;
    
    std::unique_ptr<ParametricEQ> eqProcessor;
    std::unique_ptr<EQEditor16Band> eqEditor;
    
    bool isMute = false;
    float volume = 1.0f;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(RouteMappingComponent)
};