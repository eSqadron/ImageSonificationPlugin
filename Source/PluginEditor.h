/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

struct ToggleButtonInfo {
    ToggleButtonInfo(std::string name, SythesiserAlgorithm alg, juce::ToggleButton* button) : name(name), alg(alg), button(button) { }
    std::string name;
    SythesiserAlgorithm alg;
    juce::ToggleButton* button;

    ToggleButtonInfo(const ToggleButtonInfo& other) {
        this->name = other.name;
        this->alg = other.alg;
        this->button = other.button;
    }
};

//==============================================================================
/**
*/
class ImageSonificationProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    ImageSonificationProcessorEditor (ImageSonificationProcessor&);
    ~ImageSonificationProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    juce::ImageComponent imageComponent;

    juce::TextEditor imagePathText;
    ImageSonificationProcessor& audioProcessor;

    juce::ToggleButton noiseToggle;
    juce::ToggleButton toggleEECS;

    ToggleButtonInfo buttons[2] = {
        { "Image as noise", Noise, &noiseToggle},
        { "EECS 351 WN22", EECS_simplified, &toggleEECS}
    };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ImageSonificationProcessorEditor)
};
