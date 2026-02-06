#pragma once
#include <JuceHeader.h>

class MainComponent : public juce::Component,
                      public juce::AudioIODeviceCallback,
                      public juce::ComboBox::Listener,
                      public juce::Slider::Listener,
                      public juce::Button::Listener
{
public:
    MainComponent();
    ~MainComponent() override;

    void paint(juce::Graphics&) override;
    void resized() override;

    void audioDeviceIOCallback(const float** inputChannelData,
                               int numInputChannels,
                               float** outputChannelData,
                               int numOutputChannels,
                               int numSamples) override;
    void audioDeviceAboutToStart(juce::AudioIODevice* device) override;
    void audioDeviceStopped() override;
    void audioDeviceError(const juce::String& errorMessage) override;

    void comboBoxChanged(juce::ComboBox* comboBoxThatHasChanged) override;
    void sliderValueChanged(juce::Slider* slider) override;
    void buttonClicked(juce::Button* button) override;

    void refreshAudioDeviceLists();
    void addNewMapping();
    void setupAudio();

private:
    juce::AudioDeviceManager audioDeviceManager;
    juce::Array<juce::AudioIODeviceType*> deviceTypes;
    
    struct Mapping
    {
        std::unique_ptr<juce::ComboBox> inputDeviceCombo;
        std::unique_ptr<juce::ComboBox> outputDeviceCombo;
        std::unique_ptr<juce::Slider> volumeSlider;
        std::unique_ptr<juce::TextButton> muteButton;
        std::unique_ptr<juce::TextButton> eqButton;
        bool isMuted = false;
        float gain = 1.0f;
    };
    
    juce::OwnedArray<Mapping> mappings;
    juce::ComboBox globalInputDeviceCombo, globalOutputDeviceCombo;
    juce::TextButton addMappingButton{"Add Mapping"};
    juce::TextButton quitButton{"QUIT"};
    
    juce::Label titleLabel{"SORA Matrix v0.1.0"};
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};
