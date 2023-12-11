/*
  ==============================================================================

    ImageAsNoiseAlgorithm.h
    Created: 11 Dec 2023 5:59:17pm
    Author:  OEM

  ==============================================================================
*/

#include <JuceHeader.h>
#pragma once


class ImageAsNoiseAlgorithm {
public:
    ImageAsNoiseAlgorithm(unsigned int& WidthIt, unsigned int& HeightIt, std::shared_ptr<juce::Image::BitmapData> imageBitmapPtr);

    void generate_next_samples(float* output_buffer, unsigned int buffer_length);

    std::shared_ptr<juce::Image::BitmapData> imageBitmapPtr;
private:
    unsigned int& WidthIt;
    unsigned int& HeightIt;
};