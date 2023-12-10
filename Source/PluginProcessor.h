/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/**
*/
enum SythesiserAlgorithm {
    Noise,
    EECS_simplified
};



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

    void resetImageData(unsigned int image_width, unsigned int image_height);

    std::unique_ptr<juce::FileLogger> m_flogger;

    std::unique_ptr<juce::Image::BitmapData> imageBitmapPtr;
    juce::Image image;

    std::atomic < SythesiserAlgorithm>* algorithm = nullptr;


    std::atomic < float>* algorithmParam = nullptr;

private:
    juce::AudioProcessorValueTreeState parameters;

    unsigned int imageWidth = 0;
    unsigned int imageHeight = 0;
    short unsigned int EECS_it = 0;
    short unsigned int EECS_limit = 35;

    unsigned int widthIt = 0;
    unsigned int heightIt = 0;

    double currentSampleRate = 0.0;

    short unsigned int chords[3] = { 36, 40, 43 };
    double angleDelta[3] = { 0, 0, 0 };
    double currentAngle[3] = { 0, 0, 0 };


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ImageSonificationProcessor)
};
