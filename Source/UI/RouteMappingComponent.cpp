#include "RouteMappingComponent.h"

RouteMappingComponent::RouteMappingComponent(const juce::String& mappingName)
    : mappingName(mappingName)
{
    eqProcessor = std::make_unique<ParametricEQ>();
    
    // Input device selector
    inputDeviceCombo = std::make_unique<juce::ComboBox>();
    inputDeviceCombo->setTextWhenNoChoicesAvailable("No Input Devices");
    addAndMakeVisible(inputDeviceCombo.get());
    
    // Output device selector
    outputDeviceCombo = std::make_unique<juce::ComboBox>();
    outputDeviceCombo->setTextWhenNoChoicesAvailable("No Output Devices");
    addAndMakeVisible(outputDeviceCombo.get());
    
    // Mute button
    muteButton = std::make_unique<juce::ToggleButton>("MUTE");
    muteButton->setToggleState(isMute, juce::dontSendNotification);
    muteButton->onClick = [this] { isMute = muteButton->getToggleState(); };
    addAndMakeVisible(muteButton.get());
    
    // EQ button
    eqButton = std::make_unique<juce::TextButton>("EQ");
    eqButton->onClick = [this] { showEQEditor(); };
    addAndMakeVisible(eqButton.get());
    
    // Volume slider
    volumeSlider = std::make_unique<juce::Slider>(juce::Slider::LinearHorizontal, juce::Slider::TextBoxRight);
    volumeSlider->setRange(0.0, 100.0, 1.0);
    volumeSlider->setValue(100.0, juce::dontSendNotification);
    volumeSlider->onValueChange = [this] { volume = (float)volumeSlider->getValue() / 100.0f; };
    addAndMakeVisible(volumeSlider.get());
    
    setSize(400, 100);
}

RouteMappingComponent::~RouteMappingComponent()
{
}

void RouteMappingComponent::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colour(50, 50, 55));
    g.setColour(juce::Colours::white);
    g.drawRect(getLocalBounds(), 1);
    
    // Draw mapping name
    g.setFont(16.0f);
    g.drawText(mappingName, getLocalBounds().removeFromTop(25).reduced(5), 
               juce::Justification::centredLeft);
}

void RouteMappingComponent::resized()
{
    auto bounds = getLocalBounds().reduced(5);
    bounds.removeFromTop(25); // Remove space for title
    
    auto topRow = bounds.removeFromTop(25);
    inputDeviceCombo->setBounds(topRow.removeFromLeft(150).reduced(2));
    outputDeviceCombo->setBounds(topRow.removeFromLeft(150).reduced(2));
    
    auto bottomRow = bounds.removeFromTop(25);
    muteButton->setBounds(bottomRow.removeFromLeft(80).reduced(2));
    eqButton->setBounds(bottomRow.removeFromLeft(80).reduced(2));
    volumeSlider->setBounds(bottomRow.reduced(2));
}

void RouteMappingComponent::setInputDevice(const juce::String& deviceName)
{
    inputDeviceCombo->setText(deviceName);
}

void RouteMappingComponent::setOutputDevice(const juce::String& deviceName)
{
    outputDeviceCombo->setText(deviceName);
}

void RouteMappingComponent::showEQEditor()
{
    if (eqEditor == nullptr)
    {
        eqEditor = std::make_unique<EQEditor16Band>(*eqProcessor);
        eqEditor->setSize(600, 400);
        
        juce::DialogWindow::LaunchOptions options;
        options.content.setOwned(eqEditor.release());
        options.dialogTitle = "Parametric EQ - " + mappingName;
        options.componentToCentreAround = this;
        options.dialogBackgroundColour = juce::Colour(30, 30, 32);
        options.escapeKeyTriggersCloseButton = true;
        options.useNativeTitleBar = false;
        options.resizable = true;
        
        auto* dw = options.launchAsync();
        dw->setVisible(true);
    }
}