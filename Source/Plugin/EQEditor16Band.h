#pragma once

#include <JuceHeader.h>
#include "ParametricEQ.h"

class EQEditor16Band : public juce::Component,
                       public juce::ChangeListener
{
public:
    EQEditor16Band(ParametricEQ& eqProcessor);
    ~EQEditor16Band() override;
    
    void paint(juce::Graphics& g) override;
    void resized() override;
    void mouseDown(const juce::MouseEvent& event) override;
    void mouseDoubleClick(const juce::MouseEvent& event) override;
    
    // ChangeListener
    void changeListenerCallback(juce::ChangeBroadcaster* source) override;
    
private:
    struct BandControl : public juce::Component
    {
        BandControl(int index, ParametricEQ& eq);
        ~BandControl() override;
        
        void paint(juce::Graphics& g) override;
        void resized() override;
        
        void updateFromBand();
        
        int bandIndex;
        ParametricEQ& eqProcessor;
        
        std::unique_ptr<juce::ComboBox> typeCombo;
        std::unique_ptr<juce::Slider> freqSlider;
        std::unique_ptr<juce::Slider> gainSlider;
        std::unique_ptr<juce::Slider> qSlider;
        std::unique_ptr<juce::ComboBox> slopeCombo;
        std::unique_ptr<juce::TextButton> deleteButton;
        
        juce::Label freqLabel, gainLabel, qLabel, slopeLabel;
        
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BandControl)
    };
    
    void addBandAtPoint(juce::Point<float> position);
    void deleteBand(int index);
    void updateBandControls();
    
    ParametricEQ& eqProcessor;
    juce::OwnedArray<BandControl> bandControls;
    juce::Viewport viewport;
    juce::Component bandsContainer;
    
    juce::Path frequencyResponsePath;
    juce::Array<juce::Point<float>> nodePositions;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(EQEditor16Band)
};