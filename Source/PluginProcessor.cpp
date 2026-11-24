#include "PluginProcessor.h"
#include "PluginEditor.h"

ECHeatProcessor::ECHeatProcessor()
    : AudioProcessor(BusesProperties().withInput("Input", juce::AudioChannelSet::stereo(), true).withOutput("Output", juce::AudioChannelSet::stereo(), true)),
      apvts(*this, nullptr, "Params", createParams()) {}

ECHeatProcessor::~ECHeatProcessor() {}

juce::AudioProcessorValueTreeState::ParameterLayout ECHeatProcessor::createParams() {
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;
    params.push_back(std::make_unique<juce::AudioParameterFloat>("DRIVE", "Drive", 0.0f, 100.0f, 57.3f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("MIX", "Mix", 0.0f, 100.0f, 66.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("TONE", "Tone", -12.0f, 12.0f, 0.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("OUTPUT", "Output", -24.0f, 12.0f, -3.0f));
    params.push_back(std::make_unique<juce::AudioParameterChoice>("TYPE", "Saturation Type", juce::StringArray{"A","B","C","D","E"}, 3));
    params.push_back(std::make_unique<juce::AudioParameterChoice>("OVERSAMPLE", "Oversample", juce::StringArray{"1x","2x","4x","8x"}, 2));
    params.push_back(std::make_unique<juce::AudioParameterBool>("WARM", "Analog Warm", true));
    params.push_back(std::make_unique<juce::AudioParameterBool>("SAFE", "Safe Clip", true));
    params.push_back(std::make_unique<juce::AudioParameterBool>("BYPASS", "Bypass", false));
    return {params.begin(), params.end()};
}

void ECHeatProcessor::prepareToPlay(double sr, int samples) {
    oversampler.initProcessing((size_t)samples);
    oversampler.setOversamplingFactor(2);
}
void ECHeatProcessor::releaseResources() {}

float ECHeatProcessor::applySaturation(float x, int type, float drive) {
    float d = 1.0f + (drive / 100.0f) * 8.0f;
    float y = x * d;
    switch (type) {
        case 0: return std::tanh(y * 0.6f);
        case 1: return y / (1.0f + std::abs(y));
        case 2: return std::tanh(y) * 0.9f + 0.1f * std::sin(y * 2.0f);
        case 3: return juce::jlimit(-0.9f, 0.9f, y * 0.8f + 0.2f * y*y*y);
        case 4: return juce::jlimit(-1.0f, 1.0f, y);
        default: return std::tanh(y);
    }
}

void ECHeatProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer&) {
    if (*apvts.getRawParameterValue("BYPASS") > 0.5f) return;
    auto drive = apvts.getRawParameterValue("DRIVE")->load();
    auto mix = apvts.getRawParameterValue("MIX")->load() / 100.0f;
    auto tone = apvts.getRawParameterValue("TONE")->load();
    auto output = juce::Decibels::decibelsToGain(apvts.getRawParameterValue("OUTPUT")->load());
    auto type = (int)apvts.getRawParameterValue("TYPE")->load();
    auto warm = apvts.getRawParameterValue("WARM")->load() > 0.5f;
    auto safe = apvts.getRawParameterValue("SAFE")->load() > 0.5f;
    inLevel = buffer.getMagnitude(0, 0, buffer.getNumSamples());
    for (int ch = 0; ch < buffer.getNumChannels(); ++ch) {
        auto* data = buffer.getWritePointer(ch);
        for (int i = 0; i < buffer.getNumSamples(); ++i) {
            float dry = data[i];
            float wet = applySaturation(dry, type, drive);
            if (warm) wet = wet * 1.02f + 0.02f * std::tanh(wet * 2.0f);
            wet = wet + (tone / 24.0f) * wet;
            float out = dry * (1.0f - mix) + wet * mix;
            if (safe) out = std::tanh(out * 0.98f);
            data[i] = out * output;
        }
    }
    outLevel = buffer.getMagnitude(0, 0, buffer.getNumSamples());
}

juce::AudioProcessorEditor* ECHeatProcessor::createEditor() { return new ECHeatEditor(*this); }
void ECHeatProcessor::getStateInformation(juce::MemoryBlock& dest) { auto state = apvts.copyState(); std::unique_ptr<juce::XmlElement> xml(state.createXml()); copyXmlToBinary(*xml, dest); }
void ECHeatProcessor::setStateInformation(const void* data, int size) { std::unique_ptr<juce::XmlElement> xml(getXmlFromBinary(data, size)); if (xml) apvts.replaceState(juce::ValueTree::fromXml(*xml)); }
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter() { return new ECHeatProcessor(); }
