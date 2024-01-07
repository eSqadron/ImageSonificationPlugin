/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "ImageAsNoiseAlgorithm.h"
#include "EECS351WN22algorithm.h"
#include "WindowingAlgorithm.h"
#include "TerrainAlgorithm.h"


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

    std::shared_ptr<juce::Image::BitmapData> imageBitmapPtr;
    juce::Image image;

    std::atomic <float>* algorithmParam = nullptr;
    std::atomic <float>* crawlingDirectionParam = nullptr;

    bool imageIsBeingLoaded = true;
    
    int valueOfSlider = 1;

private:
    juce::AudioProcessorValueTreeState parameters;

    unsigned int widthIt = 0;
    unsigned int heightIt = 0;

    //
    ImageAsNoiseAlgorithm imageAsNoiseAlg;
    EECS351WN22algorithm eecs351wn22Alg;
    WindowingAlgorithm windowingAlg;
    TerrainAlgorithm terrainAlg;



    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ImageSonificationProcessor)
};

