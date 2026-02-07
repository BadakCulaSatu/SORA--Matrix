#include "EQEditor16Band.h"

EQEditor16Band::EQEditor16Band(ParametricEQ& eqProcessor)
    : eqProcessor(eqProcessor)
{
    setSize(800, 600);
    
    addAndMakeVisible(viewport);
    viewport.setViewedComponent(&bandsContainer, false);
    
    updateBandControls();
}

EQEditor16Band::~EQEditor16Band()
{
}

void EQEditor16Band::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colour(30, 30, 32));
    
    // Draw frequency grid
    g.setColour(juce::Colour(60, 60, 62));
    const int numVerticalLines = 10;
    const int numHorizontalLines = 8;
    
    for (int i = 1; i < numVerticalLines; ++i)
    {
        float x = getWidth() * i / (float)numVerticalLines;
        g.drawVerticalLine((int)x, 0, getHeight());
    }
    
    for (int i = 1; i < numHorizontalLines; ++i)
    {
        float y = getHeight() * i / (float)numHorizontalLines;
        g.drawHorizontalLine((int)y, 0, getWidth());
    }
    
    // Draw center line
    g.setColour(juce::Colour(80, 80, 82));
    float centerY = getHeight() * 0.5f;
    g.drawHorizontalLine((int)centerY, 0, getWidth());
    
    // Draw frequency response curve
    g.setColour(juce::Colours::cyan);
    g.strokePath(frequencyResponsePath, juce::PathStrokeType(2.0f));
    
    // Draw nodes
    g.setColour(juce::Colours::white);
    for (const auto& pos : nodePositions)
    {
        g.fillEllipse(pos.x - 4, pos.y - 4, 8, 8);
        g.drawEllipse(pos.x - 6, pos.y - 6, 12, 12, 1.0f);
    }
}

void EQEditor16Band::resized()
{
    viewport.setBounds(getLocalBounds().reduced(10));
    bandsContainer.setSize(getWidth() - 20, 400);
    
    // Position band controls
    int y = 10;
    for (auto* control : bandControls)
    {
        control->setBounds(10, y, bandsContainer.getWidth() - 20, 60);
        y += 70;
    }
    
    bandsContainer.setSize(bandsContainer.getWidth(), y + 10);
}

void EQEditor16Band::mouseDoubleClick(const juce::MouseEvent& event)
{
    auto localPos = event.getPosition().toFloat();
    addBandAtPoint(localPos);
}

void EQEditor16Band::addBandAtPoint(juce::Point<float> position)
{
    // Convert position to EQ parameters
    float freq = std::pow(10.0f, position.x / (float)getWidth() * 4.0f + 1.0f); // 10Hz - 22kHz
    float gain = (0.5f - position.y / (float)getHeight()) * 44.0f - 22.0f; // -22 to +22 dB
    
    // Limit to specified ranges
    freq = juce::jlimit(0.0f, 22000.0f, freq);
    gain = juce::jlimit(-22.0f, 22.0f, gain);
    
    ParametricEQ::EQBand newBand;
    newBand.type = ParametricEQ::EQBand::PEAK;
    newBand.frequency = freq;
    newBand.gain = gain;
    newBand.q = 1.0f;
    newBand.slope = 12;
    newBand.nodePosition = position;
    
    eqProcessor.addBand(newBand);
    updateBandControls();
    repaint();
}

void EQEditor16Band::deleteBand(int index)
{
    eqProcessor.removeBand(index);
    updateBandControls();
    repaint();
}

void EQEditor16Band::updateBandControls()
{
    bandControls.clear();
    
    const auto& bands = eqProcessor.getBands();
    for (int i = 0; i < bands.size(); ++i)
    {
        auto* control = new BandControl(i, eqProcessor);
        bandControls.add(control);
        bandsContainer.addAndMakeVisible(control);
    }
    
    resized();
}

void EQEditor16Band::changeListenerCallback(juce::ChangeBroadcaster* source)
{
    repaint();
}

// BandControl implementation
EQEditor16Band::BandControl::BandControl(int index, ParametricEQ& eq)
    : bandIndex(index), eqProcessor(eq)
{
    typeCombo = std::make_unique<juce::ComboBox>();
    typeCombo->addItem("PEAK", 1);
    typeCombo->addItem("HPF", 2);
    typeCombo->addItem("LPF", 3);
    typeCombo->setSelectedId(1, juce::dontSendNotification);
    typeCombo->onChange = [this] { updateBand(); };
    addAndMakeVisible(typeCombo.get());
    
    freqSlider = std::make_unique<juce::Slider>(juce::Slider::LinearHorizontal, juce::Slider::TextBoxBelow);
    freqSlider->setRange(0.0, 22000.0, 1.0);
    freqSlider->setSkewFactorFromMidPoint(1000.0);
    freqSlider->onValueChange = [this] { updateBand(); };
    addAndMakeVisible(freqSlider.get());
    
    gainSlider = std::make_unique<juce::Slider>(juce::Slider::LinearVertical, juce::Slider::TextBoxLeft);
    gainSlider->setRange(-22.0, 22.0, 0.1);
    gainSlider->onValueChange = [this] { updateBand(); };
    addAndMakeVisible(gainSlider.get());
    
    qSlider = std::make_unique<juce::Slider>(juce::Slider::LinearHorizontal, juce::Slider::TextBoxBelow);
    qSlider->setRange(0.4, 40.0, 0.1);
    qSlider->onValueChange = [this] { updateBand(); };
    addAndMakeVisible(qSlider.get());
    
    slopeCombo = std::make_unique<juce::ComboBox>();
    slopeCombo->addItem("6 dB/oct", 6);
    slopeCombo->addItem("12 dB/oct", 12);
    slopeCombo->addItem("24 dB/oct", 24);
    slopeCombo->addItem("48 dB/oct", 48);
    slopeCombo->setSelectedId(12, juce::dontSendNotification);
    slopeCombo->onChange = [this] { updateBand(); };
    addAndMakeVisible(slopeCombo.get());
    
    deleteButton = std::make_unique<juce::TextButton>("X");
    deleteButton->onClick = [this] {
        if (auto* editor = findParentComponentOfClass<EQEditor16Band>())
            editor->deleteBand(bandIndex);
    };
    addAndMakeVisible(deleteButton.get());
    
    // Labels
    addAndMakeVisible(freqLabel);
    freqLabel.setText("Freq (Hz)", juce::dontSendNotification);
    freqLabel.setJustificationType(juce::Justification::centred);
    
    addAndMakeVisible(gainLabel);
    gainLabel.setText("Gain (dB)", juce::dontSendNotification);
    gainLabel.setJustificationType(juce::Justification::centred);
    
    addAndMakeVisible(qLabel);
    qLabel.setText("Q", juce::dontSendNotification);
    qLabel.setJustificationType(juce::Justification::centred);
    
    addAndMakeVisible(slopeLabel);
    slopeLabel.setText("Slope", juce::dontSendNotification);
    slopeLabel.setJustificationType(juce::Justification::centred);
    
    updateFromBand();
}

EQEditor16Band::BandControl::~BandControl()
{
}

void EQEditor16Band::BandControl::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colour(40, 40, 42));
    g.setColour(juce::Colours::white);
    g.drawRect(getLocalBounds(), 1);
}

void EQEditor16Band::BandControl::resized()
{
    auto bounds = getLocalBounds().reduced(5);
    
    typeCombo->setBounds(bounds.removeFromTop(20));
    bounds.removeFromTop(5);
    
    deleteButton->setBounds(bounds.removeFromRight(30).reduced(2));
    
    auto freqBounds = bounds.removeFromTop(40);
    freqLabel.setBounds(freqBounds.removeFromTop(15));
    freqSlider->setBounds(freqBounds);
    
    auto gainBounds = bounds.removeFromRight(60);
    gainLabel.setBounds(gainBounds.removeFromTop(15));
    gainSlider->setBounds(gainBounds);
    
    auto qBounds = bounds.removeFromTop(40);
    qLabel.setBounds(qBounds.removeFromTop(15));
    qSlider->setBounds(qBounds);
    
    auto slopeBounds = bounds.removeFromTop(40);
    slopeLabel.setBounds(slopeBounds.removeFromTop(15));
    slopeCombo->setBounds(slopeBounds);
}

void EQEditor16Band::BandControl::updateFromBand()
{
    const auto& bands = eqProcessor.getBands();
    if (bandIndex >= 0 && bandIndex < bands.size())
    {
        const auto& band = bands[bandIndex];
        
        typeCombo->setSelectedId(band.type + 1, juce::dontSendNotification);
        freqSlider->setValue(band.frequency, juce::dontSendNotification);
        gainSlider->setValue(band.gain, juce::dontSendNotification);
        qSlider->setValue(band.q, juce::dontSendNotification);
        slopeCombo->setSelectedId(band.slope, juce::dontSendNotification);
        
        // Enable/disable controls based on band type
        bool isFilter = (band.type == ParametricEQ::EQBand::HPF || 
                        band.type == ParametricEQ::EQBand::LPF);
        
        gainSlider->setEnabled(!isFilter);
        qSlider->setEnabled(!isFilter);
        slopeCombo->setEnabled(isFilter);
        
        // For filters, set gain to 0
        if (isFilter)
        {
            gainSlider->setValue(0.0, juce::dontSendNotification);
        }
    }
}

void EQEditor16Band::BandControl::updateBand()
{
    ParametricEQ::EQBand band;
    band.type = static_cast<ParametricEQ::EQBand::Type>(typeCombo->getSelectedId() - 1);
    band.frequency = (float)freqSlider->getValue();
    band.gain = (float)gainSlider->getValue();
    band.q = (float)qSlider->getValue();
    band.slope = slopeCombo->getSelectedId();
    
    eqProcessor.updateBand(bandIndex, band);
}