#pragma once
#include <JuceHeader.h>
#include "PluginProcessor.h"

class ECHeatEditor : public juce::AudioProcessorEditor, public juce::Timer {
public:
    ECHeatEditor(ECHeatProcessor&);
    ~ECHeatEditor() override;
    void paint(juce::Graphics&) override;
    void resized() override;
    void timerCallback() override;
private:
    ECHeatProcessor& processor;
    juce::Slider driveKnob, mixKnob, toneKnob, outputKnob;
    juce::ToggleButton warmBtn, safeBtn, bypassBtn, analogBtn, oversampleBtn;
    juce::TextButton typeA, typeB, typeC, typeD, typeE;
    std::vector<std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>> sliderAtt;
    std::vector<std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment>> buttonAtt;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ECHeatEditor)
};
