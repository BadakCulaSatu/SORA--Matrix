#include "ParametricEQ.h"

ParametricEQ::ParametricEQ()
{
    // Add initial bands
    bands.add({ EQBand::PEAK, 100.0f, 0.0f, 1.0f, 12 });
    bands.add({ EQBand::PEAK, 1000.0f, 0.0f, 1.0f, 12 });
    bands.add({ EQBand::PEAK, 10000.0f, 0.0f, 1.0f, 12 });
    
    updateFilters();
}

ParametricEQ::~ParametricEQ()
{
}

void ParametricEQ::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    currentSampleRate = sampleRate;
    updateFilters();
}

void ParametricEQ::releaseResources()
{
}

void ParametricEQ::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedLock sl(lock);
    
    const int numChannels = buffer.getNumChannels();
    const int numSamples = buffer.getNumSamples();
    
    // Process each band
    for (auto* filter : filters)
    {
        if (filter != nullptr)
        {
            for (int channel = 0; channel < numChannels; ++channel)
            {
                filter->processSamples(buffer.getWritePointer(channel), numSamples);
            }
        }
    }
}

void ParametricEQ::addBand(const EQBand& band)
{
    juce::ScopedLock sl(lock);
    bands.add(band);
    updateFilters();
}

void ParametricEQ::removeBand(int index)
{
    juce::ScopedLock sl(lock);
    if (index >= 0 && index < bands.size())
    {
        bands.remove(index);
        updateFilters();
    }
}

void ParametricEQ::updateBand(int index, const EQBand& band)
{
    juce::ScopedLock sl(lock);
    if (index >= 0 && index < bands.size())
    {
        bands.set(index, band);
        updateFilters();
    }
}

void ParametricEQ::updateFilters()
{
    filters.clear();
    
    for (const auto& band : bands)
    {
        if (band.isBypassed)
        {
            filters.add(nullptr);
            continue;
        }
        
        auto* filter = new juce::IIRFilter();
        
        switch (band.type)
        {
            case EQBand::PEAK:
                filter->setCoefficients(juce::IIRCoefficients::makePeakFilter(
                    currentSampleRate,
                    band.frequency,
                    band.q,
                    juce::Decibels::decibelsToGain(band.gain)
                ));
                break;
                
            case EQBand::HPF:
                filter->setCoefficients(juce::IIRCoefficients::makeHighPass(
                    currentSampleRate,
                    band.frequency,
                    band.slope / 12.0 // Convert to Q-like value
                ));
                break;
                
            case EQBand::LPF:
                filter->setCoefficients(juce::IIRCoefficients::makeLowPass(
                    currentSampleRate,
                    band.frequency,
                    band.slope / 12.0 // Convert to Q-like value
                ));
                break;
        }
        
        filters.add(filter);
    }
}

juce::AudioProcessorEditor* ParametricEQ::createEditor()
{
    return new juce::GenericAudioProcessorEditor(*this);
}

void ParametricEQ::getStateInformation(juce::MemoryBlock& destData)
{
    juce::MemoryOutputStream stream(destData, true);
    
    stream.writeInt(bands.size());
    for (const auto& band : bands)
    {
        stream.writeInt(static_cast<int>(band.type));
        stream.writeFloat(band.frequency);
        stream.writeFloat(band.gain);
        stream.writeFloat(band.q);
        stream.writeInt(band.slope);
        stream.writeBool(band.isBypassed);
    }
}

void ParametricEQ::setStateInformation(const void* data, int sizeInBytes)
{
    juce::MemoryInputStream stream(data, sizeInBytes, false);
    
    bands.clear();
    int numBands = stream.readInt();
    
    for (int i = 0; i < numBands; ++i)
    {
        EQBand band;
        band.type = static_cast<EQBand::Type>(stream.readInt());
        band.frequency = stream.readFloat();
        band.gain = stream.readFloat();
        band.q = stream.readFloat();
        band.slope = stream.readInt();
        band.isBypassed = stream.readBool();
        
        bands.add(band);
    }
    
    updateFilters();
}

void ParametricEQ::loadPreset(const juce::String& presetName)
{
    // Implementation for preset loading
    // You would typically load from a file or internal storage
}

void ParametricEQ::savePreset(const juce::String& presetName)
{
    // Implementation for preset saving
}

juce::StringArray ParametricEQ::getPresetList() const
{
    return { "Default", "Flat", "Bass Boost", "Treble Boost" };
}