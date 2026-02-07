#pragma once
#include <JuceHeader.h>
#include "ParametricEQ.h"

class EQEditor16Band : public juce::Component,
                       public juce::Slider::Listener,
                       public juce::ComboBox::Listener,
                       public juce::Button::Listener
{
public:
    EQEditor16Band(ParametricEQ& eq);
    void resized() override;
    
private:
    void sliderValueChanged(juce::Slider* slider) override;
    void comboBoxChanged(juce::ComboBox* comboBox) override;
    void buttonClicked(juce::Button* button) override;
    
    ParametricEQ& eqRef;
    
    juce::TabbedComponent tabbedComponent;
    juce::OwnedArray<juce::Component> bandTabs;
    
    juce::ComboBox presetCombo;
    juce::TextButton loadButton, saveButton, deleteButton;
    juce::TextEditor presetNameEditor;
    
    void createBandTab(int bandIndex);
    void updatePresetList();
    void saveCurrentPreset();
    void loadSelectedPreset();
    void deleteSelectedPreset();
};