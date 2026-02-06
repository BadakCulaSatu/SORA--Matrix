#include "MainComponent.h"

MainComponent::MainComponent()
{
    setSize(600, 400);
    
    addAndMakeVisible(titleLabel);
    titleLabel.setFont(juce::Font(20.0f, juce::Font::bold));
    titleLabel.setJustificationType(juce::Justification::centred);
    
    addAndMakeVisible(globalInputDeviceCombo);
    globalInputDeviceCombo.addListener(this);
    addAndMakeVisible(globalOutputDeviceCombo);
    globalOutputDeviceCombo.addListener(this);
    
    addAndMakeVisible(addMappingButton);
    addMappingButton.addListener(this);
    
    addAndMakeVisible(quitButton);
    quitButton.addListener(this);
    quitButton.setColour(juce::TextButton::buttonColourId, juce::Colours::darkred);
    
    refreshAudioDeviceLists();
    addNewMapping(); // Mapping awal #A
    
    setupAudio();
}

MainComponent::~MainComponent()
{
    audioDeviceManager.closeAudioDevice();
}

void MainComponent::paint(juce::Graphics& g)
{
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
}

void MainComponent::resized()
{
    auto area = getLocalBounds();
    titleLabel.setBounds(area.removeFromTop(40).reduced(10));
    
    auto topRow = area.removeFromTop(30);
    globalInputDeviceCombo.setBounds(topRow.removeFromLeft(200).reduced(5));
    globalOutputDeviceCombo.setBounds(topRow.removeFromLeft(200).reduced(5));
    
    addMappingButton.setBounds(area.removeFromBottom(30).removeFromRight(150).reduced(5));
    quitButton.setBounds(area.removeFromBottom(30).removeFromLeft(100).reduced(5));
    
    int y = 80;
    for (auto* mapping : mappings)
    {
        auto mappingArea = area.removeFromTop(40).reduced(10);
        if (mapping->inputDeviceCombo)
            mapping->inputDeviceCombo->setBounds(mappingArea.removeFromLeft(150));
        mappingArea.removeFromLeft(10);
        if (mapping->outputDeviceCombo)
            mapping->outputDeviceCombo->setBounds(mappingArea.removeFromLeft(150));
        mappingArea.removeFromLeft(10);
        if (mapping->volumeSlider)
            mapping->volumeSlider->setBounds(mappingArea.removeFromLeft(100));
        mappingArea.removeFromLeft(10);
        if (mapping->muteButton)
            mapping->muteButton->setBounds(mappingArea.removeFromLeft(60));
        mappingArea.removeFromLeft(10);
        if (mapping->eqButton)
            mapping->eqButton->setBounds(mappingArea.removeFromLeft(60));
        y += 45;
    }
}

void MainComponent::audioDeviceIOCallback(const float** inputChannelData,
                                          int numInputChannels,
                                          float** outputChannelData,
                                          int numOutputChannels,
                                          int numSamples)
{
    // Routing audio akan diimplementasi di sini
    juce::AudioBuffer<float> buffer(outputChannelData, numOutputChannels, numSamples);
    buffer.clear();
}

void MainComponent::audioDeviceAboutToStart(juce::AudioIODevice* device) {}
void MainComponent::audioDeviceStopped() {}
void MainComponent::audioDeviceError(const juce::String& errorMessage)
{
    DBG("Audio error: " + errorMessage);
}

void MainComponent::refreshAudioDeviceLists()
{
    auto* deviceType = audioDeviceManager.getCurrentDeviceTypeObject();
    if (deviceType)
    {
        deviceType->scanForDevices();
        auto deviceNames = deviceType->getDeviceNames();
        
        globalInputDeviceCombo.clear();
        globalOutputDeviceCombo.clear();
        
        for (auto& name : deviceNames)
        {
            globalInputDeviceCombo.addItem(name, globalInputDeviceCombo.getNumItems() + 1);
            globalOutputDeviceCombo.addItem(name, globalOutputDeviceCombo.getNumItems() + 1);
        }
    }
}

void MainComponent::comboBoxChanged(juce::ComboBox* comboBox)
{
    if (comboBox == &globalInputDeviceCombo || comboBox == &globalOutputDeviceCombo)
    {
        auto setup = audioDeviceManager.getAudioDeviceSetup();
        
        if (comboBox == &globalInputDeviceCombo)
            setup.inputDeviceName = globalInputDeviceCombo.getText();
        else
            setup.outputDeviceName = globalOutputDeviceCombo.getText();
        
        audioDeviceManager.setAudioDeviceSetup(setup, true);
    }
}

void MainComponent::sliderValueChanged(juce::Slider* slider)
{
    for (auto* mapping : mappings)
    {
        if (mapping->volumeSlider.get() == slider)
        {
            mapping->gain = (float)slider->getValue() / 100.0f;
            break;
        }
    }
}

void MainComponent::buttonClicked(juce::Button* button)
{
    if (button == &addMappingButton)
    {
        addNewMapping();
        resized();
    }
    else if (button == &quitButton)
    {
        JUCEApplication::getInstance()->systemRequestedQuit();
    }
    
    for (auto* mapping : mappings)
    {
        if (mapping->muteButton.get() == button)
        {
            mapping->isMuted = !mapping->isMuted;
            mapping->muteButton->setButtonText(mapping->isMuted ? "UNMUTE" : "MUTE");
            break;
        }
        else if (mapping->eqButton.get() == button)
        {
            // TODO: Buka window EQ
            DBG("EQ Button clicked for mapping");
        }
    }
}

void MainComponent::addNewMapping()
{
    auto* mapping = new Mapping();
    
    mapping->inputDeviceCombo = std::make_unique<juce::ComboBox>();
    mapping->outputDeviceCombo = std::make_unique<juce::ComboBox>();
    mapping->volumeSlider = std::make_unique<juce::Slider>();
    mapping->muteButton = std::make_unique<juce::TextButton>("MUTE");
    mapping->eqButton = std::make_unique<juce::TextButton>("EQ");
    
    addAndMakeVisible(mapping->inputDeviceCombo.get());
    addAndMakeVisible(mapping->outputDeviceCombo.get());
    addAndMakeVisible(mapping->volumeSlider.get());
    addAndMakeVisible(mapping->muteButton.get());
    addAndMakeVisible(mapping->eqButton.get());
    
    mapping->inputDeviceCombo->addListener(this);
    mapping->outputDeviceCombo->addListener(this);
    mapping->volumeSlider->addListener(this);
    mapping->muteButton->addListener(this);
    mapping->eqButton->addListener(this);
    
    mapping->volumeSlider->setRange(0.0, 100.0, 1.0);
    mapping->volumeSlider->setValue(100.0);
    mapping->volumeSlider->setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    
    mappings.add(mapping);
    
    // Copy device lists ke combo baru
    for (int i = 0; i < globalInputDeviceCombo.getNumItems(); ++i)
    {
        mapping->inputDeviceCombo->addItem(globalInputDeviceCombo.getItemText(i), i + 1);
        mapping->outputDeviceCombo->addItem(globalOutputDeviceCombo.getItemText(i), i + 1);
    }
}

void MainComponent::setupAudio()
{
    audioDeviceManager.initialise(2, 2, nullptr, true, "", nullptr);
    audioDeviceManager.addAudioCallback(this);
}
