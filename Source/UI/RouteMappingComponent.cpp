#include "RouteMappingComponent.h"

RouteMappingComponent::RouteMappingComponent(int mappingId)
    : mappingIndex(mappingId)
{
    // Label Mapping
    mappingLabel.setText("Mapping #" + juce::String(char('A' + mappingId)), 
                        juce::dontSendNotification);
    mappingLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(mappingLabel);
    
    // Input Device
    inputLabel.setText("Input:", juce::dontSendNotification);
    addAndMakeVisible(inputLabel);
    
    inputDeviceCombo.addListener(this);
    addAndMakeVisible(inputDeviceCombo);
    
    // Output Device
    outputLabel.setText("Output:", juce::dontSendNotification);
    addAndMakeVisible(outputLabel);
    
    outputDeviceCombo.addListener(this);
    addAndMakeVisible(outputDeviceCombo);
    
    // Mute Button
    muteButton.setButtonText("MUTE");
    muteButton.setClickingTogglesState(true);
    muteButton.addListener(this);
    addAndMakeVisible(muteButton);
    
    // EQ Button
    eqButton.setButtonText("EQ");
    eqButton.addListener(this);
    addAndMakeVisible(eqButton);
    
    // Volume Slider
    volumeLabel.setText("Vol:", juce::dontSendNotification);
    addAndMakeVisible(volumeLabel);
    
    volumeSlider.setRange(0.0, 100.0, 1.0);
    volumeSlider.setTextBoxStyle(juce::Slider::TextBoxRight, false, 50, 20);
    volumeSlider.setValue(100.0);
    volumeSlider.addListener(this);
    addAndMakeVisible(volumeSlider);
    
    // Populate device lists
    populateDeviceLists();
    
    setSize(800, 60);
}

RouteMappingComponent::~RouteMappingComponent() {}

void RouteMappingComponent::resized()
{
    auto bounds = getLocalBounds().reduced(5);
    
    // Mapping Label (kiri)
    mappingLabel.setBounds(bounds.removeFromLeft(100));
    
    // Input Device
    inputLabel.setBounds(bounds.removeFromLeft(50));
    inputDeviceCombo.setBounds(bounds.removeFromLeft(150));
    bounds.removeFromLeft(10);
    
    // Output Device
    outputLabel.setBounds(bounds.removeFromLeft(60));
    outputDeviceCombo.setBounds(bounds.removeFromLeft(150));
    bounds.removeFromLeft(10);
    
    // Mute Button
    muteButton.setBounds(bounds.removeFromLeft(80));
    bounds.removeFromLeft(10);
    
    // EQ Button
    eqButton.setBounds(bounds.removeFromLeft(80));
    bounds.removeFromLeft(10);
    
    // Volume
    volumeLabel.setBounds(bounds.removeFromLeft(40));
    volumeSlider.setBounds(bounds.removeFromLeft(150));
}

void RouteMappingComponent::populateDeviceLists()
{
    inputDeviceCombo.clear();
    outputDeviceCombo.clear();
    
    // Tambah default option
    inputDeviceCombo.addItem("System Audio", 1);
    inputDeviceCombo.addItem("Blackhole 2ch", 2);
    
    // Ambil device audio dari system
    auto deviceManager = juce::AudioDeviceManager();
    deviceManager.initialise(0, 2, nullptr, true);
    
    auto deviceTypes = deviceManager.getAvailableDeviceTypes();
    for (auto* type : deviceTypes)
    {
        type->scanForDevices();
        auto inputNames = type->getDeviceNames(true);  // Input devices
        auto outputNames = type->getDeviceNames(false); // Output devices
        
        for (const auto& name : inputNames)
            inputDeviceCombo.addItem(name, inputDeviceCombo.getNumItems() + 100);
        
        for (const auto& name : outputNames)
            outputDeviceCombo.addItem(name, outputDeviceCombo.getNumItems() + 200);
    }
    
    // Set default selection
    if (inputDeviceCombo.getNumItems() > 0)
        inputDeviceCombo.setSelectedItemIndex(0);
    if (outputDeviceCombo.getNumItems() > 0)
        outputDeviceCombo.setSelectedItemIndex(0);
}

void RouteMappingComponent::comboBoxChanged(juce::ComboBox* comboBox)
{
    // Handle device selection changes
    if (comboBox == &inputDeviceCombo)
    {
        // Update audio routing
    }
    else if (comboBox == &outputDeviceCombo)
    {
        // Update audio routing
    }
}

void RouteMappingComponent::sliderValueChanged(juce::Slider* slider)
{
    if (slider == &volumeSlider)
    {
        // Apply volume gain
        float volume = (float)volumeSlider.getValue() / 100.0f;
        // Apply to audio routing
    }
}

void RouteMappingComponent::buttonClicked(juce::Button* button)
{
    if (button == &muteButton)
    {
        muted = muteButton.getToggleState();
        updateMuteButton();
        // Apply mute to audio routing
    }
    else if (button == &eqButton)
    {
        // Open EQ Editor
        auto* editor = new EQEditor16Band(eqProcessor);
        
        juce::DialogWindow::LaunchOptions options;
        options.content.setOwned(editor);
        options.dialogTitle = "EQ - Mapping #" + juce::String(char('A' + mappingIndex));
        options.escapeKeyTriggersCloseButton = true;
        options.useNativeTitleBar = true;
        options.resizable = true;
        
        auto* window = options.launchAsync();
        if (window) 
        {
            window->centreWithSize(600, 500);
            window->toFront(true);
        }
    }
}

void RouteMappingComponent::updateMuteButton()
{
    muteButton.setButtonText(muted ? "UNMUTE" : "MUTE");
    muteButton.setColour(juce::TextButton::buttonColourId, 
                        muted ? juce::Colours::red : juce::Colours::grey);
}

juce::String RouteMappingComponent::getInputDevice() const
{
    return inputDeviceCombo.getText();
}

juce::String RouteMappingComponent::getOutputDevice() const
{
    return outputDeviceCombo.getText();
}

bool RouteMappingComponent::isMuted() const
{
    return muted;
}

float RouteMappingComponent::getVolume() const
{
    return (float)volumeSlider.getValue() / 100.0f;
}