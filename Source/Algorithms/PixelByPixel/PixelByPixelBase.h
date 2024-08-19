/*
  ==============================================================================

    PixelByPixelBase.h
    Created: 15 Jan 2024 3:37:37pm
    Author:  j.mazur

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "..\..\CommonTypeDefs.h"
#include "..\AlgorithmBase.h"


class PixelByPixelBase: public AlgorithmBase {
public:
    PixelByPixelBase(CrawlingDirection& directionOfPlay, int& windowSize);

    void generateNextSamples(float* output_buffer, unsigned int buffer_length) override;

    void prepareToPlay(double sampleRate, int samplesPerBlock);

protected:
    unsigned int SampleRate = 0u;

    unsigned int SamplesPerPixel = 1u;
    unsigned int SamplesPerPixelIterator = 0u;

    int& WindowSize;

    virtual void iteratePixelAlgorithmSpecific(juce::Colour) { }

private:
    void iteratePixelCommon();

    juce::Colour Window();

    virtual float getSampleFromPixel(juce::Colour pixel) = 0;


    CrawlingDirection& directionOfPlay;
};