/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#include "CommonTypeDefs.h"

#include "Algorithms/PixelByPixel/ImageAsNoiseAlgorithm.h"
#include "Algorithms/PixelByPixel/EECS351WN22algorithm.h"
#include "WindowingAlgorithm.h"
#include "Algorithms/RowByRow/LandscapeAlgorithm.h"


//==============================================================================
/**
*/


class ImageSonificationProcessor : public juce::AudioProcessor
#if JucePlugin_Enable_ARA
    , public juce::AudioProcessorARAExtension
#endif
{
public:
    //==============================================================================
    ImageSonificationProcessor();
    ~ImageSonificationProcessor() override;

    //==============================================================================
    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

#ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported(const BusesLayout& layouts) const override;
#endif

    void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram(int index) override;
    const juce::String getProgramName(int index) override;
    void changeProgramName(int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation(juce::MemoryBlock& destData) override;
    void setStateInformation(const void* data, int sizeInBytes) override;

    void resetBitmap();

    std::unique_ptr<juce::FileLogger> m_flogger;

    juce::Image image;

    std::atomic <float>* algorithmParam = nullptr;
    std::atomic <float>* pixelByPixelDirectionParam = nullptr;

    PixelByPixelDirection directionOfPixelByPixelPlay = static_cast<PixelByPixelDirection>(0);

    bool imageIsBeingLoaded = true;
    
    int WindowSizeSliderValue = 1; // TODO - make it juce::uint8

private:
    juce::AudioProcessorValueTreeState parameters;

    //
    ImageAsNoiseAlgorithm imageAsNoiseAlg;
    EECS351WN22algorithm eecs351wn22Alg;
    //WindowingAlgorithm windowingAlg; OBSOLETE
    TerrainAlgorithm landscapeAlg;

    std::array<AlgorithmBase*, IMEPLEMENTED_ALGORITHMS_COUNT> AlgorithmsArray;



    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ImageSonificationProcessor)
};

