#include "ParametricEQ.h"

ParametricEQ::ParametricEQ()
{
    // Default EQ bands (ISO frequencies)
    const std::array<float, 16> defaultFreqs = {
        31.5f, 63.f, 125.f, 250.f, 500.f, 1000.f, 2000.f,
        4000.f, 8000.f, 16000.f, 31.5f, 63.f, 125.f, 250.f, 500.f, 1000.f
    };

    for (size_t i = 0; i < 16; ++i)
    {
        bandFreqs[i] = defaultFreqs[i];
        bandGains[i] = 0.0f;
        bandQs[i] = 1.0f;
    }

    presetDir = juce::File::getSpecialLocation(juce::File::userApplicationDataDirectory)
        .getChildFile("SORA Matrix/EQ Presets");
    presetDir.createDirectory();
}

void ParametricEQ::prepare(const juce::dsp::ProcessSpec& spec)
{
    currentSpec = spec;
    for (auto& filter : filters) filter.prepare(spec);
    for (size_t i = 0; i < 16; ++i) updateBand(i);
}

void ParametricEQ::process(juce::AudioBuffer<float>& buffer)
{
    juce::dsp::AudioBlock<float> block(buffer);
    for (auto& filter : filters) filter.process(juce::dsp::ProcessContextReplacing<float>(block));
}

void ParametricEQ::reset()
{
    for (auto& filter : filters) filter.reset();
}

void ParametricEQ::setBandParameters(size_t index, float freq, float gain, float q)
{
    jassert(index < 16);
    bandFreqs[index] = juce::jlimit(20.0f, 20000.0f, freq);
    bandGains[index] = juce::jlimit(-24.0f, 24.0f, gain);
    bandQs[index] = juce::jlimit(0.4f, 40.0f, q);
    updateBand(index);
}

void ParametricEQ::updateBand(size_t index)
{
    auto coeffs = juce::dsp::IIR::Coefficients<float>::makePeakFilter(
        currentSpec.sampleRate,
        bandFreqs[index],
        bandQs[index],
        juce::Decibels::decibelsToGain(bandGains[index])
    );
    *filters[index].state = *coeffs;
}

void ParametricEQ::savePreset(const juce::String& name)
{
    juce::var data;
    for (int i = 0; i < 16; ++i)
    {
        data[juce::String(i)] = juce::var(juce::Array<juce::var>{
            bandFreqs[i].load(),
            bandGains[i].load(),
            bandQs[i].load()
        });
    }

    getPresetFile(name).replaceWithText(JSON::toString(data));
}

void ParametricEQ::loadPreset(const juce::String& name)
{
    auto file = getPresetFile(name);
    if (!file.existsAsFile()) return;

    auto data = JSON::parse(file);
    if (auto obj = data.getDynamicObject())
    {
        for (int i = 0; i < 16; ++i)
        {
            auto key = juce::String(i);
            if (obj->hasProperty(key))
            {
                auto arr = obj->getProperty(key).getArray();
                if (arr->size() == 3)
                {
                    bandFreqs[i] = (float)arr->getUnchecked(0);
                    bandGains[i] = (float)arr->getUnchecked(1);
                    bandQs[i] = (float)arr->getUnchecked(2);
                    updateBand(i);
                }
            }
        }
    }
}

juce::StringArray ParametricEQ::getPresetList()
{
    juce::StringArray presets;
    for (auto& file : presetDir.findChildFiles(juce::File::findFiles, false, "*.json"))
        presets.add(file.getFileNameWithoutExtension());
    return presets;
}

juce::File ParametricEQ::getPresetFile(const juce::String& name)
{
    return presetDir.getChildFile(name + ".json");
}
