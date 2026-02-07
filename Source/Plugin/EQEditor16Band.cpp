#include "EQEditor16Band.h"

class BandTabContent : public juce::Component,
                       public juce::Slider::Listener,
                       public juce::ComboBox::Listener
{
public:
    BandTabContent(ParametricEQ& eq, int bandIndex)
        : eqRef(eq), bandIdx(bandIndex)
    {
        // Type Combo
        typeCombo.addItem("PEAK", 1);
        typeCombo.addItem("HPF", 2);
        typeCombo.addItem("LPF", 3);
        typeCombo.setSelectedId(eqRef.getBandType(bandIdx) + 1);
        typeCombo.addListener(this);
        addAndMakeVisible(typeCombo);
        
        // Freq
        freqSlider.setRange(0.0, 22000.0, 1.0);
        freqSlider.setSkewFactorFromMidPoint(1000.0);
        freqSlider.setTextValueSuffix(" Hz");
        freqSlider.setValue(eqRef.getBandFreq(bandIdx));
        freqSlider.addListener(this);
        addAndMakeVisible(freqSlider);
        
        freqLabel.setText("Frequency", juce::dontSendNotification);
        addAndMakeVisible(freqLabel);
        
        // Gain (hide untuk HPF/LPF)
        gainSlider.setRange(-22.0, 22.0, 0.1);
        gainSlider.setTextValueSuffix(" dB");
        gainSlider.setValue(eqRef.getBandGain(bandIdx));
        gainSlider.addListener(this);
        addAndMakeVisible(gainSlider);
        
        gainLabel.setText("Gain", juce::dontSendNotification);
        addAndMakeVisible(gainLabel);
        
        // Q Controls
        qSlider.setRange(0.4, 40.0, 0.1);
        qSlider.setSkewFactorFromMidPoint(1.0);
        qSlider.setValue(eqRef.getBandQ(bandIdx));
        qSlider.addListener(this);
        addAndMakeVisible(qSlider);
        
        qCombo.addItem("6 dB/oct", 1);
        qCombo.addItem("12 dB/oct", 2);
        qCombo.addItem("24 dB/oct", 3);
        qCombo.addItem("48 dB/oct", 4);
        qCombo.setSelectedId(2);
        qCombo.addListener(this);
        addAndMakeVisible(qCombo);
        
        qLabel.setText("Q/Slope", juce::dontSendNotification);
        addAndMakeVisible(qLabel);
        
        updateControlsVisibility();
    }
    
    void resized() override
    {
        auto bounds = getLocalBounds().reduced(10);
        
        auto topRow = bounds.removeFromTop(30);
        typeCombo.setBounds(topRow.removeFromLeft(100));
        topRow.removeFromLeft(20);
        qLabel.setBounds(topRow.removeFromLeft(80));
        qCombo.setBounds(topRow.removeFromLeft(100));
        qSlider.setBounds(topRow.removeFromLeft(250));
        
        bounds.removeFromTop(20);
        
        freqLabel.setBounds(bounds.removeFromTop(20));
        freqSlider.setBounds(bounds.removeFromTop(30));
        
        bounds.removeFromTop(10);
        
        gainLabel.setBounds(bounds.removeFromTop(20));
        gainSlider.setBounds(bounds.removeFromTop(30));
    }
    
    void updateControlsVisibility()
    {
        bool isPeak = eqRef.getBandType(bandIdx) == ParametricEQ::PEAK;
        gainSlider.setVisible(isPeak);
        gainLabel.setVisible(isPeak);
        qSlider.setVisible(isPeak);
        qCombo.setVisible(!isPeak);
        
        if (!isPeak)
        {
            float q = eqRef.getBandQ(bandIdx);
            int selected = 2;
            if (q >= 0.6f) selected = 1;
            else if (q <= 0.25f) selected = 4;
            else if (q <= 0.35f) selected = 3;
            qCombo.setSelectedId(selected);
        }
    }
    
private:
    void sliderValueChanged(juce::Slider* slider) override
    {
        if (slider == &freqSlider)
            eqRef.setBandParameters(bandIdx, (float)freqSlider.getValue(), 
                                  eqRef.getBandGain(bandIdx), eqRef.getBandQ(bandIdx));
        else if (slider == &gainSlider)
            eqRef.setBandParameters(bandIdx, eqRef.getBandFreq(bandIdx),
                                  (float)gainSlider.getValue(), eqRef.getBandQ(bandIdx));
        else if (slider == &qSlider)
            eqRef.setBandParameters(bandIdx, eqRef.getBandFreq(bandIdx),
                                  eqRef.getBandGain(bandIdx), (float)qSlider.getValue());
    }
    
    void comboBoxChanged(juce::ComboBox* comboBox) override
    {
        if (comboBox == &typeCombo)
        {
            auto type = (ParametricEQ::FilterType)(typeCombo.getSelectedId() - 1);
            eqRef.setBandType(bandIdx, type);
            updateControlsVisibility();
        }
        else if (comboBox == &qCombo)
        {
            if (eqRef.getBandType(bandIdx) != ParametricEQ::PEAK)
            {
                float qValue = 0.5f;
                switch (qCombo.getSelectedId())
                {
                    case 1: qValue = 0.707f; break;
                    case 2: qValue = 0.5f; break;
                    case 3: qValue = 0.3f; break;
                    case 4: qValue = 0.2f; break;
                }
                eqRef.setBandParameters(bandIdx, eqRef.getBandFreq(bandIdx),
                                      eqRef.getBandGain(bandIdx), qValue);
            }
        }
    }
    
    ParametricEQ& eqRef;
    int bandIdx;
    
    juce::ComboBox typeCombo;
    juce::Slider freqSlider, gainSlider, qSlider;
    juce::ComboBox qCombo;
    juce::Label freqLabel, gainLabel, qLabel;
};

// Main Editor
EQEditor16Band::EQEditor16Band(ParametricEQ& eq)
    : eqRef(eq), tabbedComponent(juce::TabbedButtonBar::TabsAtTop)
{
    // Create tabs for 16 bands
    for (int i = 0; i < 16; ++i)
    {
        createBandTab(i);
    }
    
    addAndMakeVisible(tabbedComponent);
    
    // Preset Management
    presetCombo.addListener(this);
    addAndMakeVisible(presetCombo);
    
    loadButton.setButtonText("Load");
    loadButton.addListener(this);
    addAndMakeVisible(loadButton);
    
    saveButton.setButtonText("Save");
    saveButton.addListener(this);
    addAndMakeVisible(saveButton);
    
    deleteButton.setButtonText("Delete");
    deleteButton.addListener(this);
    addAndMakeVisible(deleteButton);
    
    presetNameEditor.setText("New Preset");
    addAndMakeVisible(presetNameEditor);
    
    updatePresetList();
    
    setSize(600, 500);
}

void EQEditor16Band::resized()
{
    auto bounds = getLocalBounds().reduced(10);
    
    auto presetArea = bounds.removeFromTop(30);
    presetCombo.setBounds(presetArea.removeFromLeft(200));
    presetArea.removeFromLeft(10);
    presetNameEditor.setBounds(presetArea.removeFromLeft(150));
    presetArea.removeFromLeft(10);
    loadButton.setBounds(presetArea.removeFromLeft(60));
    presetArea.removeFromLeft(5);
    saveButton.setBounds(presetArea.removeFromLeft(60));
    presetArea.removeFromLeft(5);
    deleteButton.setBounds(presetArea.removeFromLeft(60));
    
    bounds.removeFromTop(10);
    tabbedComponent.setBounds(bounds);
}

void EQEditor16Band::createBandTab(int bandIndex)
{
    auto* tabContent = new BandTabContent(eqRef, bandIndex);
    bandTabs.add(tabContent);
    tabbedComponent.addTab("Band " + juce::String(bandIndex + 1), 
                          juce::Colours::darkgrey, tabContent, true);
}

void EQEditor16Band::updatePresetList()
{
    presetCombo.clear();
    presetCombo.addItem("- Select Preset -", 1);
    auto presets = eqRef.getPresetList();
    for (int i = 0; i < presets.size(); ++i)
        presetCombo.addItem(presets[i], i + 2);
    presetCombo.setSelectedId(1);
}

void EQEditor16Band::saveCurrentPreset()
{
    auto name = presetNameEditor.getText();
    if (name.isNotEmpty())
    {
        eqRef.savePreset(name);
        updatePresetList();
        presetNameEditor.setText("");
    }
}

void EQEditor16Band::loadSelectedPreset()
{
    int id = presetCombo.getSelectedId();
    if (id > 1)
    {
        auto name = presetCombo.getItemText(id - 1);
        eqRef.loadPreset(name);
        
        // Refresh all tabs
        for (int i = 0; i < 16; ++i)
        {
            if (auto* tab = dynamic_cast<BandTabContent*>(bandTabs[i]))
            {
                // Update UI components dengan nilai baru
                // (Perlu tambah method refresh di BandTabContent)
            }
        }
    }
}

void EQEditor16Band::deleteSelectedPreset()
{
    int id = presetCombo.getSelectedId();
    if (id > 1)
    {
        auto name = presetCombo.getItemText(id - 1);
        eqRef.getPresetFile(name).deleteFile();
        updatePresetList();
    }
}

void EQEditor16Band::sliderValueChanged(juce::Slider*) {}
void EQEditor16Band::comboBoxChanged(juce::ComboBox* combo)
{
    if (combo == &presetCombo)
    {
        // Bisa tambah auto-load jika mau
    }
}

void EQEditor16Band::buttonClicked(juce::Button* button)
{
    if (button == &loadButton) loadSelectedPreset();
    else if (button == &saveButton) saveCurrentPreset();
    else if (button == &deleteButton) deleteSelectedPreset();
}