#pragma once
#include <JuceHeader.h>

class RouteMappingComponent : public Component,
                              private ComboBox::Listener,
                              private Slider::Listener,
                              private Button::Listener
{
public:
    RouteMappingComponent(int mappingId);
    ~RouteMappingComponent() override;

    void paint(Graphics& g) override;
    void resized() override;

private:
    void comboBoxChanged(ComboBox* comboBox) override;
    void sliderValueChanged(Slider* slider) override;
    void buttonClicked(Button* button) override;
    void updateDeviceLists();

    int mappingId;
    
    Label mappingLabel;
    ComboBox inputDeviceCombo;
    ComboBox outputDeviceCombo;
    TextButton muteButton { "MUTE" };
    TextButton eqButton { "EQ" };
    Slider volumeSlider;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(RouteMappingComponent)
};
