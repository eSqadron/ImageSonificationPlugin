/*
  ==============================================================================

    PixelByPixelBase.h
    Created: 15 Jan 2024 3:37:37pm
    Author:  j.mazur

  ==============================================================================
*/

#include <JuceHeader.h>
#include "CommonTypeDefs.h"
#pragma once


class PixelByPixelBase {
public:
    PixelByPixelBase(unsigned int& WidthIt, unsigned int& HeightIt, std::shared_ptr<juce::Image::BitmapData> imageBitmapPtr, CrawlingDirection& directionOfPlay);

    void generateNextSamples(float* output_buffer, unsigned int buffer_length);

    std::shared_ptr<juce::Image::BitmapData> imageBitmapPtr;
public:

    void iteratePixel();
    virtual double getSampleFromPixel() = 0;

    unsigned int& WidthIt;
    unsigned int& HeightIt;

    CrawlingDirection& directionOfPlay;
};