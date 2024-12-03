/*
  ==============================================================================

    AlgorithmBase.h
    Created: 26 Jul 2024 2:22:01pm
    Author:  OEM

  ==============================================================================
*/

#include <JuceHeader.h>
#pragma once

class AlgorithmBase {
public:
    virtual void generateNextSamples(float* output_buffer, unsigned int buffer_length) = 0;

    static juce::Image::BitmapData* imageBitmapPtr;
};
