#pragma once
#include <JuceHeader.h>
#include "ParametricEQ.h"
#include "EQEditor16Band.h"

class RouteMappingComponent : public juce::Component,
                              public juce::ComboBox::Listener,
                              public juce::Slider::Listener,
                              public juce::Button::Listener
{
public:
    RouteMappingComponent(int mappingId);
    ~RouteMappingComponent();
    
    void resized() override;
    
    // Getters
    juce::String getInputDevice() const;
    juce::String getOutputDevice() const;
    bool isMuted() const;
    float getVolume() const;
    ParametricEQ& getEQ() { return eqProcessor; }
    
private:
    void comboBoxChanged(juce::ComboBox* comboBox) override;
    void sliderValueChanged(juce::Slider* slider) override;
    void buttonClicked(juce::Button* button) override;
    
    int mappingIndex;
    ParametricEQ eqProcessor;
    
    // UI Components
    juce::Label mappingLabel;
    
    juce::ComboBox inputDeviceCombo;
    juce::ComboBox outputDeviceCombo;
    
    juce::TextButton muteButton;
    juce::TextButton eqButton;
    juce::Slider volumeSlider;
    
    juce::Label inputLabel;
    juce::Label outputLabel;
    juce::Label volumeLabel;
    
    bool muted = false;
    
    void updateMuteButton();
    void populateDeviceLists();
};