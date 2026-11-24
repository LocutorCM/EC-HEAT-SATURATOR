
### 5. `Source/PluginProcessor.h`

```cpp
#pragma once
#include <JuceHeader.h>

class ECHeatProcessor : public juce::AudioProcessor {
public:
    ECHeatProcessor();
    ~ECHeatProcessor() override;

    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;
    void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override { return true; }
    const juce::String getName() const override { return "EC HEAT"; }
    bool acceptsMidi() const override { return false; }
    bool producesMidi() const override { return false; }
    double getTailLengthSeconds() const override { return 0.0; }
    int getNumPrograms() override { return 1; }
    int getCurrentProgram() override { return 0; }
    void setCurrentProgram(int) override {}
    const juce::String getProgramName(int) override { return {}; }
    void changeProgramName(int, const juce::String&) override {}
    void getStateInformation(juce::MemoryBlock&) override;
    void setStateInformation(const void*, int) override;

    juce::AudioProcessorValueTreeState apvts;
    float inLevel = 0.0f;
    float outLevel = 0.0f;

    float applySaturation(float x, int type, float drive);

private:
    juce::AudioProcessorValueTreeState::ParameterLayout createParams();
    juce::dsp::Oversampling<float> oversampler{2, 2, juce::dsp::Oversampling<float>::filterHalfBandFIREquiripple};
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ECHeatProcessor)
};
