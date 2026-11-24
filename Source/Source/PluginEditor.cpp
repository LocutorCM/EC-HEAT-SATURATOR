#include "PluginEditor.h"

ECHeatEditor::ECHeatEditor(ECHeatProcessor& p) : AudioProcessorEditor(&p), processor(p) {
    setSize(900, 620);
    startTimerHz(30);
    auto styleKnob = [&](juce::Slider& s) {
        s.setSliderStyle(juce::Slider::RotaryVerticalDrag);
        s.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
        s.setColour(juce::Slider::rotarySliderFillColourId, juce::Colour(0xFF4FC3F7));
        addAndMakeVisible(s);
    };
    styleKnob(driveKnob); styleKnob(mixKnob); styleKnob(toneKnob); styleKnob(outputKnob);
    auto makeType = [&](juce::TextButton& b, juce::String t) {
        b.setButtonText(t);
        b.setClickingTogglesState(true);
        b.setColour(juce::TextButton::buttonColourId, juce::Colour(0xFF1A2B4A));
        b.setColour(juce::TextButton::buttonOnColourId, juce::Colour(0xFFB71C1C));
        addAndMakeVisible(b);
    };
    makeType(typeA, "A"); makeType(typeB, "B"); makeType(typeC, "C"); makeType(typeD, "D"); makeType(typeE, "E");
    typeD.setToggleState(true, juce::dontSendNotification);
    warmBtn.setButtonText("WARM"); safeBtn.setButtonText("SAFE CLIP"); bypassBtn.setButtonText("BYPASS"); analogBtn.setButtonText("ANALOG"); oversampleBtn.setButtonText("OVERSAMPLE");
    addAndMakeVisible(warmBtn); addAndMakeVisible(safeBtn); addAndMakeVisible(bypassBtn); addAndMakeVisible(analogBtn); addAndMakeVisible(oversampleBtn);
    sliderAtt.push_back(std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processor.apvts, "DRIVE", driveKnob));
    sliderAtt.push_back(std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processor.apvts, "MIX", mixKnob));
    sliderAtt.push_back(std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processor.apvts, "TONE", toneKnob));
    sliderAtt.push_back(std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processor.apvts, "OUTPUT", outputKnob));
    buttonAtt.push_back(std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(processor.apvts, "WARM", warmBtn));
    buttonAtt.push_back(std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(processor.apvts, "SAFE", safeBtn));
    buttonAtt.push_back(std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(processor.apvts, "BYPASS", bypassBtn));
    auto onType = [&](int idx, juce::TextButton& btn) {
        btn.onClick = [&, idx] {
            *processor.apvts.getRawParameterValue("TYPE") = (float)idx;
            for (auto* b : {&typeA,&typeB,&typeC,&typeD,&typeE}) b->setToggleState(false, juce::dontSendNotification);
            btn.setToggleState(true, juce::dontSendNotification);
        };
    };
    onType(0, typeA); onType(1, typeB); onType(2, typeC); onType(3, typeD); onType(4, typeE);
}
ECHeatEditor::~ECHeatEditor() { stopTimer(); }
void ECHeatEditor::paint(juce::Graphics& g) {
    g.fillAll(juce::Colour(0xFF000000));
    juce::ColourGradient grad(juce::Colour(0xFF0A1A3A), 60, 120, juce::Colour(0xFF1E4A9A), 840, 480, false);
    g.setGradientFill(grad);
    g.fillRoundedRectangle(15, 90, 870, 360, 12.0f);
    g.setColour(juce::Colour(0xFF4FC3F7).withAlpha(0.3f));
    g.drawRoundedRectangle(15, 90, 870, 360, 12.0f, 1.5f);
    g.setColour(juce::Colour(0xFFFF6B6B));
    g.setFont(juce::Font(22.0f, juce::Font::bold));
    g.drawText("EC AUDIO", 60, 25, 180, 30, juce::Justification::left);
    g.setColour(juce::Colour(0xFF90CAF9));
    g.setFont(juce::Font(48.0f, juce::Font::bold));
    g.drawText("EC HEAT", 330, 10, 300, 45, juce::Justification::centred);
    g.setColour(juce::Colours::white);
    g.setFont(18.0f);
    g.drawText("SATURATION TYPE", 70, 110, 150, 20, juce::Justification::centred);
    g.drawText("DRIVE 57.3%", 400, 370, 100, 20, juce::Justification::centred);
    g.drawText("INPUT -12.5 dB  |  OUTPUT -3.0 dB", 40, 580, 400, 20, juce::Justification::left);
    g.drawText("Oversampling 4x  |  v1.2.0", 650, 580, 200, 20, juce::Justification::right);
}
void ECHeatEditor::resized() {
    driveKnob.setBounds(370, 160, 160, 160);
    typeA.setBounds(70, 140, 130, 38); typeB.setBounds(70, 190, 130, 38); typeC.setBounds(70, 240, 130, 38); typeD.setBounds(70, 290, 130, 38); typeE.setBounds(70, 340, 130, 38);
    mixKnob.setBounds(710, 140, 90, 90); toneKnob.setBounds(710, 270, 90, 90); outputKnob.setBounds(710, 390, 90, 90);
    bypassBtn.setBounds(760, 25, 110, 28);
    oversampleBtn.setBounds(70, 500, 170, 32); analogBtn.setBounds(270, 500, 170, 32); warmBtn.setBounds(470, 500, 170, 32); safeBtn.setBounds(670, 500, 170, 32);
}
void ECHeatEditor::timerCallback() { repaint(); }
