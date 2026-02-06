#include "RouteMappingComponent.h"
#include "../Audio/AudioEngine.h"

RouteMappingComponent::RouteMappingComponent(int id) : mappingId(id)
{
    mappingLabel.setText("Mapping #" + String(static_cast<char>('A' + id)), 
                         dontSendNotification);
    addAndMakeVisible(mappingLabel);
    
    addAndMakeVisible(inputDeviceCombo);
    inputDeviceCombo.addListener(this);
    
    addAndMakeVisible(outputDeviceCombo);
    outputDeviceCombo.addListener(this);
    
    addAndMakeVisible(muteButton);
    muteButton.addListener(this);
    muteButton.setClickingTogglesState(true);
    
    addAndMakeVisible(eqButton);
    eqButton.addListener(this);
    
    addAndMakeVisible(volumeSlider);
    volumeSlider.setRange(0.0, 100.0, 0.1);
    volumeSlider.setValue(100.0);
    volumeSlider.addListener(this);
    volumeSlider.setTextBoxStyle(Slider::TextBoxRight, false, 60, 20);
    
    updateDeviceLists();
}

RouteMappingComponent::~RouteMappingComponent() {}

void RouteMappingComponent::paint(Graphics& g)
{
    g.setColour(Colours::grey.withAlpha(0.3f));
    g.fillRoundedRectangle(getLocalBounds().toFloat(), 6.0f);
    g.setColour(Colours::white.withAlpha(0.1f));
    g.drawRoundedRectangle(getLocalBounds().toFloat(), 6.0f, 1.0f);
}

void RouteMappingComponent::resized()
{
    auto bounds = getLocalBounds().reduced(10);
    auto topRow = bounds.removeFromTop(30);
    
    mappingLabel.setBounds(topRow.removeFromLeft(100));
    inputDeviceCombo.setBounds(topRow.removeFromLeft(200));
    outputDeviceCombo.setBounds(topRow.removeFromLeft(200));
    
    auto bottomRow = bounds.removeFromTop(30);
    muteButton.setBounds(bottomRow.removeFromLeft(80));
    eqButton.setBounds(bottomRow.removeFromLeft(80));
    volumeSlider.setBounds(bottomRow);
}

void RouteMappingComponent::comboBoxChanged(ComboBox* comboBox)
{
    if (comboBox == &inputDeviceCombo)
    {
        AudioEngine::getInstance()->setInputDevice(mappingId, 
            comboBox->getText());
    }
    else if (comboBox == &outputDeviceCombo)
    {
        AudioEngine::getInstance()->setOutputDevice(mappingId, 
            comboBox->getText());
    }
}

void RouteMappingComponent::sliderValueChanged(Slider* slider)
{
    if (slider == &volumeSlider)
    {
        AudioEngine::getInstance()->setVolume(mappingId, 
            (float)slider->getValue());
    }
}

void RouteMappingComponent::buttonClicked(Button* button)
{
    if (button == &muteButton)
    {
        AudioEngine::getInstance()->setMute(mappingId, 
            button->getToggleState());
    }
    else if (button == &eqButton)
    {
        // TODO: Buka window EQ
    }
}

void RouteMappingComponent::updateDeviceLists()
{
    inputDeviceCombo.clear();
    outputDeviceCombo.clear();
    
    auto inputs = AudioEngine::getInstance()->getInputDevices();
    for (const auto& device : inputs)
        inputDeviceCombo.addItem(device, inputDeviceCombo.getNumItems() + 1);
    
    auto outputs = AudioEngine::getInstance()->getOutputDevices();
    for (const auto& device : outputs)
        outputDeviceCombo.addItem(device, outputDeviceCombo.getNumItems() + 1);
}
