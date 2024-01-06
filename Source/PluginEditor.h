/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "CommonTypeDefs.h"


template <typename T>
struct ToggleButtonInfo {
    ToggleButtonInfo(std::string name, T alg, juce::ToggleButton* button) : name(name), alg(alg), button(button) { }
    std::string name;
    T alg;
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
    juce::ToggleButton toggleAdditive;

    juce::ToggleButton Crawl1Toggle;
    juce::ToggleButton Crawl2Toggle;
    juce::ToggleButton Crawl3Toggle;

    ToggleButtonInfo<enum CrawlingDirection> crawl_direction_buttons[3] = {
        { "From left to right", LeftToRight, &Crawl1Toggle},
        { "From up to down", UpToDown, &Crawl2Toggle},
        { "random", Random, &Crawl3Toggle}
    };

    ToggleButtonInfo<enum SythesiserAlgorithm> buttons[3] = {
        { "Image as noise", NoiseCrawler, &noiseToggle},
        { "EECS 351 WN22", SineChordCrawler, &toggleEECS},
        { "AdditiveCollumn", AdditiveCollumn, &toggleAdditive}
    };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ImageSonificationProcessorEditor)
};
